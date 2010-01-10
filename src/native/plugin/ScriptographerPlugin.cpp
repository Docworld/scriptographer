/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Plugin for Adobe Illustrator.
 *
 * Copyright (c) 2002-2008 Juerg Lehni, http://www.scratchdisk.com.
 * All rights reserved.
 *
 * Please visit http://scriptographer.com/ for updates and contact.
 *
 * -- GPL LICENSE NOTICE --
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * -- GPL LICENSE NOTICE --
 *
 * $Id$
 */

#include "stdHeaders.h"
#include "ScriptographerPlugin.h"
#include "ScriptographerEngine.h"
#include "resourceIds.h"
#include "AppContext.h"
#include "com_scriptographer_ScriptographerEngine.h"
#include "AIMenuCommandNotifiers.h"

ScriptographerPlugin *gPlugin = NULL;

ScriptographerPlugin::ScriptographerPlugin(SPMessageData *messageData) {
	// set the global sSPBasic pointer only once here, as it may be converted to a glued version
	// if MACHO_CFM_GLUE is defined
	sSPBasic = messageData->basic;
#ifdef MACHO_CFM_GLUE
	// the basic suite is never acquired and therefore needs to be glue manually here
	createGluedSuite((void **) &sSPBasic, sizeof(SPBasicSuite));
#endif
	m_pluginRef = messageData->self;
	m_pluginName = "Scriptographer";
	m_lockCount = 0;
	m_pluginAccess = NULL;
	m_lastError = kNoErr;
	m_supressDuplicateErrors = true;
	m_errorTimeout = 5;		// seconds
	m_lastErrorTime = 0;
	m_appStartedNotifier = NULL;
	m_selectionChangedNotifier = NULL;
	m_documentClosedNotifier = NULL;
	m_afterUndoNotifier = NULL;
	m_afterRedoNotifier = NULL;
	m_beforeRevertNotifier = NULL;
	m_afterRevertNotifier = NULL;
	m_beforeClearNotifier = NULL;
	m_engine = NULL;
	m_loaded = false;
	m_started = false;
	m_reverting = false;
	gPlugin = this;
#ifdef LOGFILE
	m_logFile = NULL;
#endif
}

ScriptographerPlugin::~ScriptographerPlugin() {
	gPlugin = NULL;
#ifdef MACHO_CFM_GLUE
	// the basic suite is never released and therefore needs to be unglue manually here
	disposeGluedSuite(sSPBasic, sizeof(SPBasicSuite));
#endif
}

#ifdef MAC_ENV

OSStatus ScriptographerPlugin::appEventHandler(EventHandlerCallRef handler, EventRef event, void* userData) {
	if (gEngine != NULL) {
		int type = -1;
		switch(GetEventKind(event)) {
			case kEventAppActivated:
				type = com_scriptographer_ScriptographerEngine_EVENT_APP_ACTIVATED;
				break;
			case kEventAppDeactivated:
				type = com_scriptographer_ScriptographerEngine_EVENT_APP_DEACTIVATED;
				break;
		}
		if (type != -1)
			return gEngine->callOnHandleEvent(type);
	}
	return kNoErr;
}

OSStatus ScriptographerPlugin::eventHandler(EventHandlerCallRef handler, EventRef event, void *userData) {
	// Only interfere with short cuts when wearre not in ADM dialogs
	if (GetUserFocusWindow() == ActiveNonFloatingWindow()) {
		AppContext context;
		UInt32 cls = GetEventClass(event);
		UInt32 kind = GetEventKind(event);
		UInt32 when = EventTimeToTicks(GetEventTime(event));
		UInt32 keyCode;
		GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &keyCode);
		UniChar uniChar;
		GetEventParameter(event, kEventParamKeyUnicodes, typeUnicodeText, NULL, sizeof(UniChar), NULL, &uniChar);
		UInt32 modifiers;
		GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifiers);
		Point point;
		GetEventParameter(event, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(Point), NULL, &point);

		bool handled = false;
		switch (cls) {
		case kEventClassKeyboard:
			switch (kind) {
			case kEventRawKeyDown:
				if (uniChar == '\b') // back space
					gEngine->onClear();
				handled = gEngine->callOnHandleKeyEvent(com_scriptographer_ScriptographerEngine_EVENT_KEY_DOWN,
						keyCode, uniChar, modifiers);
				break;
			case kEventRawKeyUp:
				handled = gEngine->callOnHandleKeyEvent(com_scriptographer_ScriptographerEngine_EVENT_KEY_UP,
						keyCode, uniChar, modifiers);
				break;
			case kEventRawKeyRepeat:
				handled = gEngine->callOnHandleKeyEvent(com_scriptographer_ScriptographerEngine_EVENT_KEY_UP,
						keyCode, uniChar, modifiers)
					|| gEngine->callOnHandleKeyEvent(com_scriptographer_ScriptographerEngine_EVENT_KEY_DOWN,
						keyCode, uniChar, modifiers);
				break;
			/*
			case kEventRawKeyModifiersChanged: {
				int i = 0;
				event.m_shiftDown = modifiers & shiftKey;
				event.m_controlDown = modifiers & controlKey;
				event.m_altDown = modifiers & optionKey;
				event.m_metaDown = modifiers & cmdKey;
			}
			break;
			*/
			}
			break;
		case kEventClassMouse:
			static bool dragging = false;
			switch (kind) {
			case kEventMouseDown: {
				WindowRef window = NULL;
				FindWindow(point, &window);
				HIViewRef view;
				HIViewGetViewForMouseEvent(HIViewGetRoot(window), event, &view);
				/*
				ControlPartCode code;
				SetPortWindowPort(window);
				GlobalToLocal(&point);
				ControlRef view = FindControlUnderMouse(point, window, &code);
				*/
				if (view != NULL) {
					CFStringRef viewClass = HIObjectCopyClassID((HIObjectRef) view);
					if (viewClass != NULL) {
						if (CFStringCompare(viewClass, CFSTR("com.adobe.owl.tabgroup"), 0) == 0 ||
							CFStringCompare(viewClass, CFSTR("com.adobe.owl.dock"), 0) == 0) {
							dragging = true;
							gEngine->callOnHandleEvent(com_scriptographer_ScriptographerEngine_EVENT_DRAG_PANEL_BEGIN);
						}
						/*
						const char *str = CFStringGetCStringPtr(viewClass, kCFStringEncodingMacRoman);
						gEngine->println(gEngine->getEnv(), "Mouse Event: #%i, x: %i y: %i, view: %x, class: %s", kind, point.h, point.v, view, str);
						*/
					}
				}
			}
			break;
			case kEventMouseUp:
				if (dragging) {
					dragging = false;
					gEngine->callOnHandleEvent(com_scriptographer_ScriptographerEngine_EVENT_DRAG_PANEL_END);
				}
				break;
			}
			break;
		}
		/* Do not allow preventing of native event processing for now
		if (handled)
			return noErr;
		*/
	}
	// Do not interfere with AI and allow it to further process this event.
	return eventNotHandledErr;
}

#endif

#ifdef WIN_ENV

WNDPROC ScriptographerPlugin::s_defaultAppWindowProc = NULL;

LRESULT CALLBACK ScriptographerPlugin::appWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (gEngine != NULL) {
		int type = -1;
		switch (uMsg) {
		case WM_ACTIVATEAPP:
			switch (LOWORD(wParam)) {
			case WA_INACTIVE:
				type = com_scriptographer_ScriptographerEngine_EVENT_APP_DEACTIVATED;
				break;
			case WA_ACTIVE:
				type = com_scriptographer_ScriptographerEngine_EVENT_APP_ACTIVATED;
				break;
			}
			break;
		}
		if (type != -1)
			gEngine->callOnHandleEvent(type);
	}
	return ::CallWindowProc(s_defaultAppWindowProc, hwnd, uMsg, wParam, lParam);
}

#endif


bool ScriptographerPlugin::isKeyDown(int keycode) {
#ifdef MAC_ENV
	// Table that converts Java keycodes to Mac keycodes:
	// TODO: Check Open JDK about how it translates this
	static unsigned char keycodeToMac[256] = {
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x33,0x30,0x4c,0xff,0xff,0x24,0xff,0xff,
		0x38,0x3b,0x36,0xff,0x39,0xff,0xff,0xff,0xff,0xff,0xff,0x35,0xff,0xff,0xff,0xff,
		0x31,0x74,0x79,0x77,0x73,0x7b,0x7e,0x7c,0x7d,0xff,0xff,0xff,0x2b,0xff,0x2f,0x2c,
		0x1d,0x12,0x13,0x14,0x15,0x17,0x16,0x1a,0x1c,0x19,0xff,0x29,0xff,0x18,0xff,0xff,
		0xff,0x00,0x0b,0x08,0x02,0x0e,0x03,0x05,0x04,0x22,0x26,0x28,0x25,0x2e,0x2d,0x1f,
		0x23,0x0c,0x0f,0x01,0x11,0x20,0x09,0x0d,0x07,0x10,0x06,0x21,0x2a,0x1e,0xff,0xff,
		0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5b,0x5c,0x43,0x45,0xff,0x1b,0x41,0x4b,
		0x7a,0x78,0x63,0x76,0x60,0x61,0x62,0x64,0x65,0x6d,0x67,0x6f,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3a,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0x27,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x32,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
	};
	if (keycode >= 0 && keycode <= 255) {
		keycode = keycodeToMac[keycode];
		if (keycode != 0xff) {
			KeyMap keys;
			GetKeys(keys);
			// return BitTst(&keys, keycode) != 0;
			return (((unsigned char *) keys)[keycode >> 3] & (1 << (keycode & 7))) != 0;
		}
	}
	return false;
#endif
#ifdef WIN_ENV
	return (GetAsyncKeyState(keycode) & 0x8000) ? 1 : 0;
#endif
}

long ScriptographerPlugin::getNanoTime() {
#ifdef MAC_ENV
		Nanoseconds nano = AbsoluteToNanoseconds(UpTime());
		return UnsignedWideToUInt64(nano);
#endif
#ifdef WIN_ENV
		static int scaleFactor = 0;
		if (scaleFactor == 0) {
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency (&frequency);
			scaleFactor = frequency.QuadPart;
		}
		LARGE_INTEGER counter;
		QueryPerformanceCounter (& counter);
		return counter.QuadPart * 1000000 / scaleFactor;
#endif
}

// ScriptographerPlugin:

ASErr ScriptographerPlugin::onStartupPlugin(SPInterfaceMessage *message) {
	// Aquire only the basic suites that are needed here. the rest is acquired in postStartup.
	ASErr error;
	RETURN_ERROR(acquireSuites(&gStartupSuites));
	
	// Make sure the plugin stays in ram all the time and onPostStartupPlugin gets actually called
	sSPAccess->AcquirePlugin(m_pluginRef, &m_pluginAccess);
	
	// Add app started notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer Started", kAIApplicationStartedNotifier, &m_appStartedNotifier));
	
	// Add selection changed notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer Selection Changed", kAIArtSelectionChangedNotifier, &m_selectionChangedNotifier));

	// Add document closed notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer Document Closed", kAIDocumentClosedNotifier, &m_documentClosedNotifier));

	// Add after undo menu notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer After Undo", "AI Command Notifier: After Undo", &m_afterUndoNotifier));

	// Add after redo menu notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer After Redo", "AI Command Notifier: After Redo", &m_afterRedoNotifier));

	// Add before revert menu notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer Before Revert", "AI Command Notifier: Before Revert To Saved", &m_beforeRevertNotifier));

	// Add after revert menu notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer After Revert", "AI Command Notifier: After Revert To Saved", &m_afterRevertNotifier));

	// Add before clear menu notifier
	RETURN_ERROR(sAINotifier->AddNotifier(m_pluginRef, "Scriptographer Before Clear", "AI Command Notifier: Before Clear", &m_beforeClearNotifier));
	
	// Determine baseDirectory from plugin location:
	char pluginPath[kMaxPathLength];
	SPPlatformFileSpecification fileSpec;
	sSPPlugins->GetPluginFileSpecification(m_pluginRef, &fileSpec);
	if (!fileSpecToPath(&fileSpec, pluginPath))
		return kCantHappenErr;
	
	// Now find the last occurence of PATH_SEP_CHR and determine the string there:
	*(strrchr(pluginPath, PATH_SEP_CHR) + 1) = '\0';

	#ifdef LOGFILE
		// Create logfile:
		char path[512];
		sprintf(path, "%s" PATH_SEP_STR "log" PATH_SEP_STR "native.log", pluginPath);
		m_logFile = fopen(path, "wt");
		log("Starting Scriptographer with plugin path: %s", pluginPath);
	#endif
		
	RETURN_ERROR(sSPPlugins->SetPluginName(m_pluginRef, m_pluginName));
	
	try {
		// Try to create the Java Engine:
		m_engine = new ScriptographerEngine(pluginPath);
	} catch(ScriptographerException *e) {
		e->report(NULL);
		delete e;
		return kCantHappenErr;
	}

	m_loaded = true;
	return error;
}

ASErr ScriptographerPlugin::onPostStartupPlugin() {
	log("onPostStartupPlugin. Engine: %x", m_engine);
	if (m_engine == NULL)
		return kCantHappenErr;
	
	// Accuire the rest of the suites:
	ASErr error;
	RETURN_ERROR(acquireSuites(&gPostStartupSuites));
	
	// And finally initialize the engine:
	m_engine->initEngine();
	m_started = true;
#ifdef MAC_ENV
	// Install App Events
	static EventTypeSpec appEvents[] = {
		{ kEventClassApplication, kEventAppActivated },
		{ kEventClassApplication, kEventAppDeactivated }
	};
	// TODO: Figure out if this needs DEFINE_CALLBACK_PROC / CALLBACK_PROC as well?
	RETURN_ERROR(InstallApplicationEventHandler(NewEventHandlerUPP(appEventHandler),
			sizeof(appEvents) / sizeof(EventTypeSpec), appEvents, this, NULL));

	// Install Events
	static EventTypeSpec events[] = {
		{ kEventClassKeyboard, kEventRawKeyDown },
		{ kEventClassKeyboard, kEventRawKeyUp },
		{ kEventClassKeyboard, kEventRawKeyRepeat },
//		{ kEventClassKeyboard, kEventRawKeyModifiersChanged },
		{ kEventClassMouse, kEventMouseDown },
		{ kEventClassMouse, kEventMouseUp }
	};
	// TODO: Figure out if this needs DEFINE_CALLBACK_PROC / CALLBACK_PROC as well?
	RETURN_ERROR(InstallEventHandler(GetEventDispatcherTarget(), NewEventHandlerUPP(eventHandler),
			sizeof(events) / sizeof(EventTypeSpec), events, this, NULL));
#endif
#ifdef WIN_ENV
	HWND hWnd = (HWND) sADMWinHost->GetPlatformAppWindow();
	s_defaultAppWindowProc = (WNDPROC) ::SetWindowLong(hWnd, GWL_WNDPROC, (LONG) appWindowProc);
	// If the app is active (focus on splasher), send WA_ACTIVE message again, since it was received
	// before installing the WindowProc.
	// CAUTION: Installing WindowProc in onStartupPlugin does not seem to work on Windows,
	// something seems to override it again after.
	if (hWnd == ::GetParent(::GetForegroundWindow()))
		appWindowProc(hWnd, WM_ACTIVATEAPP, WA_ACTIVE, 0);
#endif
	return error;
}

ASErr ScriptographerPlugin::onShutdownPlugin(SPInterfaceMessage *message) {
	log("onShutdownPlugin");
#ifdef WIN_ENV
	// If we have overridden the default WindowProc, set it back now, since ours wont 
	// exist anymore after unloading and that will lead to a crash.
	if (s_defaultAppWindowProc != NULL) {
		HWND hWnd = (HWND) sADMWinHost->GetPlatformAppWindow();
		::SetWindowLong(hWnd, GWL_WNDPROC, (LONG) s_defaultAppWindowProc);
	}
#endif
	m_engine->onShutdown();
	sSPAccess->ReleasePlugin(m_pluginAccess);
	m_pluginAccess = NULL;
	delete m_engine;
	m_engine = NULL;
	return kNoErr;
}

ASErr ScriptographerPlugin::onUnloadPlugin(SPInterfaceMessage *message) {
	log("onUnloadPlugin");
	releaseSuites(&gStartupSuites);
	releaseSuites(&gPostStartupSuites);
	return kUnloadErr; // Tell PluginMain to remove the plugin object after this
}

unsigned char *ScriptographerPlugin::toPascal(const char *src, unsigned char *dst) {
	int len = strlen(src);
	
	if (len > 255)
		return NULL;
	
	if (dst == NULL)
		dst = new unsigned char[len + 1];
	
	memmove(dst + 1, src, len);
	dst[0] = (unsigned char) len;
	
	return dst;
}

char *ScriptographerPlugin::fromPascal(const unsigned char *src, char *dst) {
	int len = src[0];

	if (dst == NULL)
		dst = new char[len + 1];
	
	memmove(dst, src + 1, len);
	dst[len] = '\0';
	
	return dst;
}

/*
 * Similar to sAIUser->SPPlatformFileSpecification2Path, but creates a posix path on Mac OS X, because that's what's needed for java
 */
bool ScriptographerPlugin::fileSpecToPath(SPPlatformFileSpecification *fileSpec, char *path) {
	// TODO: consider using AIFilePath.h instead of the hacks bellow!
#ifdef MAC_ENV
	// Java needs a posix path on mac, not a Carbon one, as used by Illustrator:
	// Then transform this into a real FSSpec

	// As the file refered to by fileSpec may not exist yet, create a FSSpec for its parent directory add the name afterwards
	unsigned char empty = 0; // 0-length p-string
	FSSpec fsSpec;
	if (FSMakeFSSpec(fileSpec->vRefNum, fileSpec->parID, &empty, &fsSpec) != noErr)
		return false;
	// And from there into a Posix path:
	FSRef fsRef;
	if (FSpMakeFSRef(&fsSpec, &fsRef) != noErr)
		return false;
	if (FSRefMakePath(&fsRef, (unsigned char*) path, kMaxPathLength))
		return false;
	// Now add the name to it:
	char *name = fromPascal(fileSpec->name);
	strcat(path, PATH_SEP_STR);
	strcat(path, name);
	delete name;
#else
	// On windows, things are easier because we don't have to convert to a posix path.
	// if (sAIUser->SPPlatformFileSpecification2Path(fileSpec, path))
	//	return false;
	// Actually not, because sAIUser->SPPlatformFileSpecification2Path does not handle Unicode
	// properly. But fileSpec->path seems to contain the short path already, which allways
	// seems to point to the right file, so for the time being, just copy:
	strcpy(path, fileSpec->path);
	/*
	If Unicode full paths would be needed, this would be the way to go:
	// Convert ANSI 8.3 path to Unicode 8.3 path.
	int lenA = lstrlenA(fileSpec->path);
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, fileSpec->path, lenA, NULL, 0);
	if (lenW > 0) {
		BSTR path = ::SysAllocStringLen(0, lenW);
		::MultiByteToWideChar(CP_ACP, 0, fileSpec->path, lenA, path, lenW);
		// now get Unicode full path from 8.3 path:
		int len = GetLongPathNameW(path, NULL, 0);
		if (len > 0) {
			BSTR fullPath = ::SysAllocStringLen(0, len);
			GetLongPathNameW(path, fullPath, len);
			::SysFreeString(fullPath);
		}
		::SysFreeString(path);
	}
	*/
#endif
	return true;
}

bool ScriptographerPlugin::pathToFileSpec(const char *path, SPPlatformFileSpecification *fileSpec) {
#ifdef MAC_ENV
	// as FSRef can only be created for existing files and directories, this is a bit complicated:
	// create an FSRef for the path's parent dir and from there create a FSSpec for the child: 
	int len = strlen(path);
	int dirPos = len - 1;
	if (path[dirPos] == '/') // skip trailing '/' for folders
		dirPos--;
	// get the path for the parent folder:
	while (dirPos >= 0 && path[dirPos] != '/')
		dirPos--;
		
	// path found?
	if (dirPos < 0)
		return false;
	
	dirPos++;
	
	// now split into directory and file:
	char dirPath[kMaxPathLength];
	char filename[kMaxPathLength];
	memcpy(dirPath, path, dirPos);
	dirPath[dirPos] = '\0';
	int fileLen = len - dirPos;
	memcpy(filename, &path[dirPos], fileLen);
	filename[fileLen] = '\0';
	// now convert the parent directory to a FSRef:
	FSRef fsRef;
	Boolean isDir;
	if (FSPathMakeRef((unsigned char*) dirPath, &fsRef, &isDir) != noErr)
		return false;
	
	// get the information of the parent dir:
	FSCatalogInfo catalogInfo;
	if (FSGetCatalogInfo(&fsRef, kFSCatInfoVolume | kFSCatInfoParentDirID | kFSCatInfoNodeID, &catalogInfo, NULL, NULL, NULL) != noErr)
		return false;
	
	// and create a FSSpec (== SPPlatformFileSpecification) for the child with it:
	OSErr error = FSMakeFSSpec(catalogInfo.volume, catalogInfo.nodeID, toPascal(filename, (unsigned char *) filename), (FSSpec *) fileSpec);
	// file not found error is ok:
	if (error != noErr && error != fnfErr)
		return false;
#else
	// on windows, things are much easier because we don't have to convert to a posix path:
	if (sAIUser->Path2SPPlatformFileSpecification(path, fileSpec))
		return false;
#endif
	return true;
}

void ScriptographerPlugin::setCursor(int cursorID) {
	sADMBasic->SetPlatformCursor(m_pluginRef, cursorID);
}

ASErr ScriptographerPlugin::handleMessage(char *caller, char *selector, void *message) {
	ASErr error = kUnhandledMsgErr;
	
	log("handleMessage: %s %s", caller, selector);

	// Common AI messages
	if (sSPBasic->IsEqual(caller, kCallerAINotify)) {
		// TODO: Is AppContext needed here?
		AppContext context;

		AINotifierMessage *msg = (AINotifierMessage *) message;
		if (msg->notifier == m_selectionChangedNotifier) {
			error = gEngine->onSelectionChanged();
		} else if (msg->notifier == m_afterUndoNotifier) {
			error = gEngine->onUndo();
		} else if (msg->notifier == m_afterRedoNotifier) {
			error = gEngine->onRedo();
		} else if (msg->notifier == m_beforeClearNotifier) {
			error = gEngine->onClear();
		} else if (msg->notifier == m_beforeRevertNotifier) {
			m_reverting = true;
		} else if (msg->notifier == m_afterRevertNotifier) {
			error = gEngine->onRevert();
			m_reverting = false;
		} else if (msg->notifier == m_documentClosedNotifier) {
			// Only send onClose events if we're not reverting the document.
			if (!m_reverting)
				error = gEngine->onDocumentClosed((AIDocumentHandle) msg->notifyData);
		} else if (msg->notifier == m_appStartedNotifier) {
			error = onPostStartupPlugin();
		}
		/* Is this needed?
		if (!error || error == kUnhandledMsgErr) {
			if (sSPBasic->IsEqual(selector, kSelectorAINotify)) {
				error = notify(msg);
			}
		}
		*/
	} else if (sSPBasic->IsEqual(caller, kCallerAIMenu)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGoMenuItem)) {
			error = gEngine->MenuItem_onSelect((AIMenuMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIUpdateMenuItem)) {
			long inArtwork, isSelected, isTrue;
			sAIMenu->GetUpdateFlags(&inArtwork, &isSelected, &isTrue);
			error = gEngine->MenuItem_onUpdate((AIMenuMessage *) message, inArtwork, isSelected, isTrue);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIFilter)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGetFilterParameters)) {
			error = getFilterParameters((AIFilterMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIGoFilter)) {
			error = onExecuteFilter((AIFilterMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIPluginGroup)) {
		if (sSPBasic->IsEqual(selector, kSelectorAINotifyEdits)) {
			error = onPluginGroupNotify((AIPluginGroupMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIUpdateArt)) {
			error = onPluginGroupUpdate((AIPluginGroupMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIFileFormat)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGetFileFormatParameters)) {
			error = onGetFileFormatParameters((AIFileFormatMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIGoFileFormat)) {
			error = onExecuteFileFormat((AIFileFormatMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAICheckFileFormat)) {
			error = onCheckFileFormat((AIFileFormatMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAITool)) {
		error = gEngine->Tool_onHandleEvent(selector, (AIToolMessage *) message);
	} else if (sSPBasic->IsEqual(caller, kCallerAILiveEffect)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIEditLiveEffectParameters)) {
			error = gEngine->LiveEffect_onEditParameters((AILiveEffectEditParamMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIGoLiveEffect)) {
			error = gEngine->LiveEffect_onCalculate((AILiveEffectGoMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAILiveEffectInterpolate)) {
			error = gEngine->LiveEffect_onInterpolate((AILiveEffectInterpParamMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAILiveEffectInputType)) {
			error = gEngine->LiveEffect_onGetInputType((AILiveEffectInputTypeMessage *) message);
		}

	} else if (sSPBasic->IsEqual(caller, kCallerAITimer)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGoTimer)) {
			error = gEngine->Timer_onExecute((AITimerMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIAnnotation)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIDrawAnnotation)) {
			error = gEngine->Annotator_onDraw((AIAnnotatorMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIInvalAnnotation)) {
			error = gEngine->Annotator_onInvalidate((AIAnnotatorMessage *) message);
		}
	}
	// Sweet Pea messages
	else if (sSPBasic->IsEqual(caller, kSPAccessCaller)) {
		if (sSPBasic->IsEqual(selector, kSPAccessUnloadSelector)) {
			error = onUnloadPlugin(static_cast<SPInterfaceMessage *>(message));
		} else if (sSPBasic->IsEqual(selector, kSPAccessReloadSelector)) {
			// There is no need to handle reload messages, as the plugin is persistent
			// Through the use of sSPAccess->AcquirePlugin();
			error = kNoErr;
		}
	} else if (sSPBasic->IsEqual(caller, kSPInterfaceCaller)) {	
		if (sSPBasic->IsEqual(selector, kSPInterfaceAboutSelector)) {
			error = gEngine->callOnHandleEvent(com_scriptographer_ScriptographerEngine_EVENT_APP_ABOUT);
		} else if (sSPBasic->IsEqual(selector, kSPInterfaceStartupSelector)) {
			error = onStartupPlugin(static_cast<SPInterfaceMessage *>(message));
		} else if (sSPBasic->IsEqual(selector, kSPInterfaceShutdownSelector)) {
			error = onShutdownPlugin(static_cast<SPInterfaceMessage *>(message));
		}
	} else if (sSPBasic->IsEqual(caller, kSPCacheCaller)) {	
		if (sSPBasic->IsEqual(selector, kSPPluginPurgeCachesSelector)) {
			error = purge() ? kSPPluginCachesFlushResponse : kSPPluginCouldntFlushResponse;
		}
	} else if (sSPBasic->IsEqual(caller, kSPPropertiesCaller)) {
		if (sSPBasic->IsEqual(selector, kSPPropertiesAcquireSelector)) {
			error = acquireProperty((SPPropertiesMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSPPropertiesReleaseSelector)) {
			error = releaseProperty((SPPropertiesMessage *) message);
		}
	}
	
	// We should probably handle some ADM messages too, but I don't know
	// which ones right now...

	return error;
}

#ifdef LOGFILE
void ScriptographerPlugin::log(const char *str, ...) {
	if (m_logFile != NULL) {
		va_list args;
		va_start(args, str);
		vfprintf(m_logFile, str, args);
		fputs(NATIVE_NEWLINE, m_logFile);
		va_end(args);
		fflush(m_logFile);
	}
}
#endif

void ScriptographerPlugin::reportError(const char* str, ...) {
	ASBoolean gotBasic = false;
	if (sADMBasic == NULL && sSPBasic != NULL) {
		sSPBasic->AcquireSuite(kADMBasicSuite, kADMBasicSuiteVersion, (const void **) &sADMBasic);
#ifdef MACHO_CFM_GLUE
		createGluedSuite((void **) &sADMBasic, sizeof(ADMBasicSuite));
#endif
		if (sADMBasic != NULL)
			gotBasic = true;
	}
	if (sADMBasic != NULL) {
		char *text = new char[1024];
		va_list args;
		va_start(args, str);
		vsprintf(text, str, args);
		va_end(args);
		sADMBasic->ErrorAlert(text);
		delete text;
	}
	if (gotBasic) {
		sSPBasic->ReleaseSuite(kADMBasicSuite, kADMBasicSuiteVersion);
		sADMBasic = NULL;
	}
}

void ScriptographerPlugin::reportError(ASErr error) {
	if (!filterError(error)) {
		unsigned long now = time(NULL);
		
		if (error != m_lastError || !m_supressDuplicateErrors ||
			now >= m_lastErrorTime + m_errorTimeout) {

			m_lastError = error;
			m_lastErrorTime = now;

			char msg[128];
			char *m;
			m = findMsg(error, msg, sizeof(msg));
			if (m != NULL) {
				char mbuf[128];

				if (strlen(m) < 120) {
					char errString[10];
					if (error < 16385) { // Then probably a plain ol' number
						sprintf(errString, "%d", error);

					} else {	// Yucky 4-byte string
						int i;
						for (i = 3; i >= 0; i--) {
							errString[i] = (char) ((unsigned long) error) & 0xff;
							error = ((unsigned long) error) >> 8;
						}
						errString[4] = '\0';
					}

					sprintf(mbuf, m, errString);
					m = mbuf;
				}
				reportError(m);
			}
		}
	}
}

char *ScriptographerPlugin::getMsgString(int n, char *buf, int len) {
	ASErr err = sADMBasic->GetIndexString(m_pluginRef, 16050, n, buf, len);
	if (err || buf[0] == '\0')
		return NULL;
	else
		return buf;
}

char *ScriptographerPlugin::findMsg(ASErr error, char *buf, int len) {
	int n = 1;
	while (true) {
		char code[10];
		ASErr err = sADMBasic->GetIndexString(m_pluginRef, 16050, n, code, sizeof(code));
		// If we got an error, back off and use the last string, which should be
		// the default message
		if (err || code[0] == '\0') {
			if (n == 1)
				return NULL;		// no error strings found
			else
				return getMsgString(n--, buf, len);
		}

		if (code[0] == '-' || (code[0] >= '0' && code[0] <= '9')) {
			// This is a number, so see if it matches
			int c = atoi(code);
			if (c == error)
				return getMsgString(n++, buf, len);

		} else {
			// non numeric 4 byte err code. (e.g.) '!sel'.
			int	c, i;
			c = 0;

			for (i = 0; i < 4; i++)
				c = (c << 8) + code[i];

			if (c == error)
				return getMsgString(n++, buf, len);
		}
		n += 2;
	}
}

ASBoolean ScriptographerPlugin::filterError(ASErr error) {
	static ASErr errors[] = {
		kRefusePluginGroupReply,
		kWantsAfterMsgPluginGroupReply,
		kMarkValidPluginGroupReply,
		kDontCarePluginGroupReply,
		kToolCantTrackCursorErr,
		kSPPluginCachesFlushResponse,
		kSPSuiteNotFoundError,
		kSPCantAcquirePluginError
	};

	for (int i = 0; i < sizeof(errors) / sizeof(ASErr); i++) {
		if (errors[i] == error)
			return true;
	}
	return false;
}

#ifdef MACHO_CFM_GLUE

void ScriptographerPlugin::createGluedSuite(void **suite, int size) {
	// Use UInt32 for representation of a pointer, as it has the same size on CFM
	UInt32 *origSuite = (UInt32 *) *suite;
	size /= 4;
	UInt32 *gluedSuite = new UInt32[size];
	*suite = gluedSuite;
	while (size--)
		*(gluedSuite++) = (UInt32) createMachOGlue((void *) *(origSuite++));
}

void ScriptographerPlugin::disposeGluedSuite(void *suite, int size) {
	UInt32 *gluedSuite = (UInt32 *) suite;
	size /= 4;
	while (size--)
		disposeMachOGlue((void *) *(gluedSuite++));
	delete[] (UInt32 *) suite;
}

#endif

ASErr ScriptographerPlugin::acquireSuites(ImportSuites *suites) {
	if (!suites->acquired) {
		suites->acquired = true;
		ASErr error;
		ImportSuite *list = suites->suites;
		for (int i = 0; list[i].name != NULL; i++) {
			RETURN_ERROR(acquireSuite(&list[i]));
		}
	}
	return kNoErr;
}

ASErr ScriptographerPlugin::releaseSuites(ImportSuites *suites) {
	if (suites->acquired) {
		suites->acquired = false;
		ASErr error;
		ImportSuite *list = suites->suites;
		for (int i = 0; list[i].name != NULL; i++) {
			RETURN_ERROR(releaseSuite(&list[i]));
		}
	}
	return kNoErr;
}

ASErr ScriptographerPlugin::acquireSuite(ImportSuite *suite) {
	ASErr error = kNoErr;
	char message[256];

	if (suite->suite != NULL) {
		error = sSPBasic->AcquireSuite(suite->name, suite->version, (const void **) suite->suite);
#ifdef MACHO_CFM_GLUE
		if (!error)
			createGluedSuite((void **) suite->suite, suite->size);
#endif
		if (error && sADMBasic != NULL) {
			sprintf(message, "Error: %d, suite: %s, version: %d!", error, suite->name, suite->version);
			sADMBasic->MessageAlert(message);
		}
	}
	return error;
}

ASErr ScriptographerPlugin::releaseSuite(ImportSuite *suite) {
	ASErr error = kNoErr;

	if (suite->suite != NULL) {
		void **s = (void **)suite->suite;
		if (*s != NULL) {
#ifdef MACHO_CFM_GLUE
			disposeGluedSuite(*((void **) suite->suite), suite->size);
#endif
			error = sSPBasic->ReleaseSuite(suite->name, suite->version);
			*s = NULL;
		}
	}
	return error;
}

// The main entry point:

static bool loaded = false;

DLLExport SPAPI int main(char *caller, char *selector, void *message) {	
	SPErr error = kNoErr;
	
	SPMessageData *messageData = static_cast<SPMessageData *>(message);
	ScriptographerPlugin *plugin = static_cast<ScriptographerPlugin *>(messageData->globals);

	bool remove = false;

	if (plugin != NULL && !loaded) {
		plugin->log("Plugin object is created, but the loaded flag is not set.");
		error = kBadParameterErr;
	} else {
		if (plugin == NULL) {
			plugin = new ScriptographerPlugin(messageData);
			if (plugin != NULL)	{
				messageData->globals = plugin;
				loaded = true;
			} else {
				error = kOutOfMemoryErr;
			}
		}

		if (plugin != NULL)
			error = plugin->handleMessage(caller, selector, message);
	}
	
	if (error == kUnloadErr) {
		remove = true;
		error = kNoErr;
	}
	
	if (remove) {
		delete plugin;
		messageData->globals = NULL;
		loaded = false;
	}
	
	return error;
}
