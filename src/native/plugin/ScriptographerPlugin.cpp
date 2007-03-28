/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Plugin for Adobe Illustrator.
 *
 * Copyright (c) 2002-2007 Juerg Lehni, http://www.scratchdisk.com.
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
	m_engine = NULL;
	m_loaded = false;
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

// ScriptographerPlugin:

ASErr ScriptographerPlugin::createTool(int index, char *title, int iconID, int cursorID, long options, char *sameGroupTool, char *sameToolsetTool) {
	Tool *tool = &m_tools[index];

	AIAddToolData data;
	
	data.title = title;
	data.tooltip = title;
	
	data.icon = sADMIcon->GetFromResource(m_pluginRef, NULL, iconID, 0);
	if (data.icon == NULL || sADMIcon->GetType(data.icon) == kUnknown) throw new ASErrException('!ico');

	ASErr error;
	
	if (sameGroupTool != NULL) {
		error = sAITool->GetToolNumberFromName(sameGroupTool, &data.sameGroupAs);
		if (error) return error;
	} else {
		data.sameGroupAs = kNoTool;
	}
	
	if (sameToolsetTool != NULL) {
		error = sAITool->GetToolNumberFromName(sameToolsetTool, &data.sameToolsetAs);
		if (error) return error;
	} else {
		data.sameToolsetAs = kNoTool;
	}

	error = sAITool->AddTool(m_pluginRef, title, &data, options, &tool->handle);
	if (error) return error;

	error = sAITool->SetToolOptions(tool->handle, options);
	if (error) return error;
	
	tool->cursorID = cursorID;
	
	return kNoErr;
}

Tool *ScriptographerPlugin::getTools(int *count) {
	*count = sizeof(m_tools) / sizeof(Tool);
	return m_tools;
}

ASErr ScriptographerPlugin::startupPlugin(SPInterfaceMessage *message) {
	// aquire only the basic suites that are needed here. the rest is acquired in postStartup.
	ASErr error = acquireSuites(&gStartupSuites);
	if (error) return error;
	
	// determine baseDirectory from plugin location:
	char homeDir[kMaxPathLength];
	SPPlatformFileSpecification fileSpec;
	sSPPlugins->GetPluginFileSpecification(m_pluginRef, &fileSpec);
   	if (!fileSpecToPath(&fileSpec, homeDir))
		return kCantHappenErr;
	
	// now find the last occurence of PATH_SEP_CHR and determine the string there:
	*(strrchr(homeDir, PATH_SEP_CHR) + 1) = '\0';

	#ifdef LOGFILE
		// Create logfile:
		char path[512];
		sprintf(path, "%s" PATH_SEP_STR "scriptographer.log", homeDir);
		m_logFile = fopen(path, "wt");
		log("Starting Scriptographer with home folder: %s", homeDir);
	#endif
		
	error = sSPPlugins->SetPluginName(m_pluginRef, m_pluginName);
	if (error) return error;

	// add app started notifier
	error = sAINotifier->AddNotifier(m_pluginRef, "Scriptographer Started", kAIApplicationStartedNotifier, &m_appStartedNotifier);
	if (error) return error;

	// add selection changed notifier
	error = sAINotifier->AddNotifier(m_pluginRef, "Scriptographer Selection Changed", kAIArtSelectionChangedNotifier, &m_selectionChangedNotifier);
	if (error) return error;
	
	try {
		// try to create the Java Engine:
		// append java to the homeDir
		strcat(homeDir, "java");
		// homeDir now contains the full path to the java stuff
		m_engine = new ScriptographerEngine(homeDir);
	} catch(ScriptographerException *e) {
		e->report(NULL);
		delete e;
		return kCantHappenErr;
	}

	// add the two script tools:
	char *title0 = "Scriptographer Tool 1";
	char *title1 = "Scriptographer Tool 2";
	error = createTool(0, title0, kTool1IconID, kTool1CursorID, kToolWantsToTrackCursorOption | kToolWantsBufferedDraggingOption);
	if (error) return error;

	error = createTool(1, title1, kTool2IconID, kTool2CursorID, kToolWantsToTrackCursorOption | kToolWantsBufferedDraggingOption, title0);
	if (error) return error;

	// make sure the plugin stays in ram all the time and postStartupPlugin gets actually called
	sSPAccess->AcquirePlugin(m_pluginRef, &m_pluginAccess);

	m_loaded = true;
	
	log("startupPlugin exit code: %x", error);

	return error;
}

ASErr ScriptographerPlugin::postStartupPlugin() {
	log("postStartupPlugin. Engine: %x", m_engine);
	if (m_engine == NULL)
		return kCantHappenErr;

	// now accuire the rest of the suites:
	ASErr error = acquireSuites(&gPostStartupSuites);
	if (error) return error;
	
	// and finally initialize the engine:
	m_engine->initEngine();

	log("postStartupPlugin exit code: %x", error);

	return error;
}

ASErr ScriptographerPlugin::shutdownPlugin(SPInterfaceMessage *message) {
	log("shutdownPlugin");
	sSPAccess->ReleasePlugin(m_pluginAccess);
	m_pluginAccess = NULL;
	delete m_engine;
	m_engine = NULL;
	return kNoErr;
}

ASErr ScriptographerPlugin::unloadPlugin(SPInterfaceMessage *message) {
	log("unloadPlugin");
	releaseSuites(&gStartupSuites);
	releaseSuites(&gPostStartupSuites);
	return kUnloadErr; // tell PluginMain to remove the plugin object after this
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
#ifdef MAC_ENV
	// java needs a posix path on mac, not a Carbon one, as used by Illustrator:
	// Then transform this into a real FSSpec

	// as the file refered to by fileSpec may not exist yet, create a FSSpec for its parent directory add the name afterwards
	unsigned char empty = 0; // 0-length p-string
	FSSpec fsSpec;
	if (FSMakeFSSpec(fileSpec->vRefNum, fileSpec->parID, &empty, &fsSpec) != noErr)
		return false;
	// and from there into a Posix path:
	FSRef fsRef;
	if (FSpMakeFSRef(&fsSpec, &fsRef) != noErr)
		return false;
	if (FSRefMakePath(&fsRef, (unsigned char*) path, kMaxPathLength))
		return false;
	
	// now add the name to it:
	char *name = fromPascal(fileSpec->name);
	strcat(path, PATH_SEP_STR);
	strcat(path, name);
	delete name;
#else
	// On windows, things are easier because we don't have to convert to a posix path.
	// Actually not, because sAIUser->SPPlatformFileSpecification2Path does not handle Unicode
	// properly
	// if (sAIUser->SPPlatformFileSpecification2Path(fileSpec, path))
	//	return false;
//	sAIUser->SPPlatformFileSpecification2Path(fileSpec, path);
	strcpy(path, fileSpec->path);
	return true;
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
	if (FSGetCatalogInfo(&fsRef, kFSCatInfoVolume | kFSCatInfoParentDirID | kFSCatInfoNodeID, &catalogInfo, NULL,  NULL, NULL) != noErr)
		return false;
	
	// and create a FSSpec for the child  with it:
	FSSpec fsSpec;
	OSErr error = FSMakeFSSpec(catalogInfo.volume , catalogInfo.nodeID, toPascal(filename, (unsigned char *) filename), &fsSpec);
	// file not found error is ok:
	if (error != noErr && error != fnfErr)
		return false;

	// copy FSSpec into fileSpec:
	fileSpec->vRefNum = fsSpec.vRefNum;
	fileSpec->parID = fsSpec.parID;
	memcpy(fileSpec->name, fsSpec.name, 64);
#else
	// on windows, things are much easier because we don't have to convert to a posix path:
	if (sAIUser->Path2SPPlatformFileSpecification(path, fileSpec))
		return false;
#endif
	return true;
}

void ScriptographerPlugin::setCursor(int cursorID) {
	ASErr error = kNoErr;
#ifdef MAC_ENV
	CursHandle cursor; 

	cursor = GetCursor(cursorID);
	if (cursor)
		SetCursor(*cursor);
#else ifdef WIN_ENV
	HCURSOR cursor;
	SPAccessRef access;
	SPPlatformAccessInfo accessInfo;

	error = sSPAccess->GetPluginAccess(m_pluginRef, &access);
	if(kNoErr == error)
		error = sSPAccess->GetAccessInfo(access, &accessInfo);
	if(kNoErr == error) {
		cursor = LoadCursor((HINSTANCE) accessInfo.defaultAccess, MAKEINTRESOURCE(cursorID));
		if (cursor)
			SetCursor(cursor);
	}
#endif
	return;
}

ASErr ScriptographerPlugin::handleMessage(char *caller, char *selector, void *message) {
	ASErr error = kUnhandledMsgErr;
	
	log("handleMessage: %s %s", caller, selector);

	// Sweet Pea messages
	if (sSPBasic->IsEqual(caller, kSPAccessCaller)) {
		if (sSPBasic->IsEqual(selector, kSPAccessUnloadSelector)) {
			error = unloadPlugin(static_cast<SPInterfaceMessage *>(message));
		} else if (sSPBasic->IsEqual(selector, kSPAccessReloadSelector)) {
			// There is no need to handle reload messages, as the plugin is persistent
			// Through the use of sSPAccess->AcquirePlugin();
			error = kNoErr;
		}
	} else if (sSPBasic->IsEqual(caller, kSPInterfaceCaller))  {	
		if (sSPBasic->IsEqual(selector, kSPInterfaceAboutSelector)) {
			error = gEngine->displayAbout();
		} else if (sSPBasic->IsEqual(selector, kSPInterfaceStartupSelector)) {
			error = startupPlugin(static_cast<SPInterfaceMessage *>(message));
		} else if (sSPBasic->IsEqual(selector, kSPInterfaceShutdownSelector)) {
			error = shutdownPlugin(static_cast<SPInterfaceMessage *>(message));
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
	// Some common AI messages
	else if (sSPBasic->IsEqual(caller, kCallerAINotify)) {
		AIAppContextHandle appContext = NULL;
		sAIAppContext->PushAppContext(m_pluginRef, &appContext);

		AINotifierMessage *msg = (AINotifierMessage *)message;
		if (msg->notifier == m_selectionChangedNotifier) {
			error = gEngine->selectionChanged();
		} else if (msg->notifier == m_appStartedNotifier) {
			error = postStartupPlugin();
		}
		/* is this needed?
		if (!error || error == kUnhandledMsgErr) {
			if (sSPBasic->IsEqual(selector, kSelectorAINotify)) {
				error = notify(msg);
			}
		}
		*/
		sAIAppContext->PopAppContext(appContext);
	} else if (sSPBasic->IsEqual(caller, kCallerAIMenu)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGoMenuItem)) {
			error = gEngine->menuItemExecute((AIMenuMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIUpdateMenuItem)) {
			long inArtwork, isSelected, isTrue;
			sAIMenu->GetUpdateFlags(&inArtwork, &isSelected, &isTrue);
			error = gEngine->menuItemUpdate((AIMenuMessage *) message, inArtwork, isSelected, isTrue);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIFilter)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGetFilterParameters)) {
			error = getFilterParameters((AIFilterMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIGoFilter)) {
			error = goFilter((AIFilterMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIPluginGroup)) {
		if (sSPBasic->IsEqual(selector, kSelectorAINotifyEdits)) {
			error = pluginGroupNotify((AIPluginGroupMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIUpdateArt)) {
			error = pluginGroupUpdate((AIPluginGroupMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIFileFormat)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGetFileFormatParameters)) {
			error = getFileFormatParameters((AIFileFormatMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIGoFileFormat)) {
			error = goFileFormat((AIFileFormatMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAICheckFileFormat)) {
			error = checkFileFormat((AIFileFormatMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAITool)) {
		error = gEngine->toolHandleEvent(selector, (AIToolMessage *) message);
		/*
			TODO: Add cursor handling
			if (sSPBasic->IsEqual(selector, kSelectorAITrackToolCursor)) {
			// the plugin keeps track of the tool cursorIds, so set it directly here
			AIToolHandle handle = ((AIToolMessage* ) message)->tool;
			for (int i = 0; i < sizeof(m_tools) / sizeof(Tool); i++) {
				if (m_tools[i].handle == handle) {
					setCursor(m_tools[i].cursorID);
					break;
				}
			}
		*/
	} else if (sSPBasic->IsEqual(caller, kCallerAILiveEffect)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIEditLiveEffectParameters)) {
			error = gEngine->liveEffectEditParameters((AILiveEffectEditParamMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIGoLiveEffect)) {
			error = gEngine->liveEffectCalculate((AILiveEffectGoMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAILiveEffectInterpolate)) {
			error = gEngine->liveEffectInterpolate((AILiveEffectInterpParamMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAILiveEffectInputType)) {
			error = gEngine->liveEffectGetInputType((AILiveEffectInputTypeMessage *) message);
		}

	} else if (sSPBasic->IsEqual(caller, kCallerAITimer)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIGoTimer)) {
			error = gEngine->timerExecute((AITimerMessage *) message);
		}
	} else if (sSPBasic->IsEqual(caller, kCallerAIAnnotation)) {
		if (sSPBasic->IsEqual(selector, kSelectorAIDrawAnnotation)) {
			error = gEngine->annotatorDraw((AIAnnotatorMessage *) message);
		} else if (sSPBasic->IsEqual(selector, kSelectorAIInvalAnnotation)) {
			error = gEngine->annotatorInvalidate((AIAnnotatorMessage *) message);
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
					if (error < 16385) {  // Then probably a plain ol' number
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
			// non numeric 4 byte err code.  (e.g.) '!sel'.
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
	// use UInt32 for representation of a pointer, as it has the same size on CFM
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
		ImportSuite *list = suites->suites;
		for (int i = 0; list[i].name != NULL; i++) {
			ASErr error = acquireSuite(&list[i]);
			if (error) return error;	
		}
	}
	return kNoErr;
}

ASErr ScriptographerPlugin::releaseSuites(ImportSuites *suites) {
	if (suites->acquired) {
		suites->acquired = false;
		ImportSuite *list = suites->suites;
		for (int i = 0; list[i].name != NULL; i++) {
			ASErr error = releaseSuite(&list[i]);
			if (error) return error;
		}
	}
	return kNoErr;
}

ASErr ScriptographerPlugin::acquireSuite(ImportSuite *suite) {
	ASErr error = kNoErr;
	char message[256];

	if (suite->suite != NULL) {
		if (suite->name == kADMItemSuite) {
			int i = 0;
		}
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
