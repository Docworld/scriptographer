/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Scripting Plugin for Adobe Illustrator
 * http://scriptographer.org/
 *
 * Copyright (c) 2002-2010, Juerg Lehni
 * http://scratchdisk.com/
 *
 * All rights reserved. See LICENSE file for details.
 */

#include "stdHeaders.h"
#include "ScriptographerPlugin.h"
#include "ScriptographerEngine.h"
#include "com_scriptographer_ScriptographerEngine.h"

/*
 * com.scriptographer.ScriptographerEngine
 */

/*
 * java.lang.String nativeReload()
 */
JNIEXPORT jstring JNICALL Java_com_scriptographer_ScriptographerEngine_nativeReload(JNIEnv *env, jclass cls) {
	try {
		return gEngine->reloadEngine();
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetTopDownCoordinates(boolean topDownCoordinates)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ScriptographerEngine_nativeSetTopDownCoordinates(
		JNIEnv *env, jclass cls, jboolean topDownCoordinates) {
	try {
		gEngine->setTopDownCoordinates(topDownCoordinates);
		gEngine->updateCoordinateSystem();
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean launch(java.lang.String filename)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_ScriptographerEngine_launch(JNIEnv *env, jclass cls, jstring filename) {
	if (filename == NULL)
		return false;
	
	char *path = NULL;
	bool result = false;
	
	try {
		path = gEngine->convertString(env, filename);
		if (strlen(path) >= 10 &&
			strncmp(path, "http://", 7) == 0 ||
			strncmp(path, "https://", 8) == 0 ||
			strncmp(path, "mailto://", 9) == 0 ||
			strncmp(path, "ftp://", 6) == 0 ||
			strncmp(path, "file://", 7) == 0) {
/*
#ifdef WIN_ENV
			// sAIURL->OpenURL does not seem to work on Windows, or often fail?
			AIWindowRef handle = NULL;
			sAIAppContext->GetPlatformAppWindow(&handle);
			char test[1024];
			sprintf(test, "url.dll,FileProtocolHandler %s", path);
			HINSTANCE res = ShellExecute(NULL, "open", "rundll32.exe", test, NULL, SW_SHOWNORMAL);
			result = !res;
#else // !WIN_ENV
*/
			result = !sAIURL->OpenURL(path);
/*
#endif // !WIN_ENV
*/
		} else {
			SPPlatformFileSpecification fileSpec;
			if (gPlugin->pathToFileSpec(path, &fileSpec)) {
#if kPluginInterfaceVersion < kAI12
				result = !sAIUser->LaunchApp(&fileSpec, true);
#else
				ai::FilePath filePath(fileSpec);
				result = !sAIUser->LaunchApp(filePath, true);
#endif
			}
		}
	} EXCEPTION_CONVERT(env);
	if (path != NULL)
		delete path;
	return result;
}

/*
 * long getNanoTime()
 */
JNIEXPORT jlong JNICALL Java_com_scriptographer_ScriptographerEngine_getNanoTime(JNIEnv *env, jclass cls) {
	return gPlugin->getNanoTime();
}

/*
 * boolean nativeIsDown(int keyCode)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_ScriptographerEngine_nativeIsDown(JNIEnv *env, jclass cls, jint keyCode) {
	try {
		return gPlugin->isKeyDown(keyCode);
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void nativeSetProgressText(java.lang.String text)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ScriptographerEngine_nativeSetProgressText(JNIEnv *env, jclass cls, jstring text) {
	try {
#if kPluginInterfaceVersion < kAI12
		char *str = gEngine->convertString(env, text);
		sAIUser->SetProgressText(str);
		delete str;
#else
		ai::UnicodeString str = gEngine->convertString_UnicodeString(env, text);
		sAIUser->SetProgressText(str);
#endif
	} EXCEPTION_CONVERT(env);
}


/*
 * boolean nativeUpdateProgress(long current, long max, boolean visible)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_ScriptographerEngine_nativeUpdateProgress(JNIEnv *env, jclass cls, jlong current, jlong max, jboolean visible) {
	try {
		// Check if CMD-. is pressed. That's  META-. on Mac and CONTROL-. on Win
		if (gPlugin != NULL && gPlugin->isKeyDown('.')
#ifdef MAC_ENV
				 && gPlugin->isKeyDown(157))
#endif // MAC_ENV
#ifdef WIN_ENV
				 && gPlugin->isKeyDown(17))
#endif // WIN_ENV
			return false;
		if (visible) {
			sAIUser->UpdateProgress(current, max);
			return !sAIUser->Cancel();
		} else {
			return true;
		}
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void nativeCloseProgress()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ScriptographerEngine_nativeCloseProgress(JNIEnv *env, jclass cls) {
	try {
		sAIUser->CloseProgress();
	} EXCEPTION_CONVERT(env);
}

/*
 * void dispatchNextEvent()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ScriptographerEngine_dispatchNextEvent(JNIEnv *env, jclass cls) {
	try {
		// Manually process event loop events:
#ifdef MAC_ENV
		// http://developer.apple.com/documentation/Carbon/Conceptual/Carbon_Event_Manager/Tasks/chapter_3_section_12.html
		EventRef event;
		if (ReceiveNextEvent(0, NULL, kEventDurationForever, true, &event) == noErr) {
			SendEventToEventTarget (event, GetEventDispatcherTarget());
            ReleaseEvent(event);
		}
#endif
#ifdef WIN_ENV
		// http://msdn2.microsoft.com/en-US/library/aa452701.aspx
		MSG message;
		if (GetMessage(&message, NULL, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
#endif
	} EXCEPTION_CONVERT(env);
}

/*
 * jdouble getIllustratorVersion()
 */
JNIEXPORT jdouble JNICALL Java_com_scriptographer_ScriptographerEngine_getIllustratorVersion(JNIEnv *env, jclass cls) {
	try {
#if kPluginInterfaceVersion >= kAI12
		ASInt32 major = sAIRuntime->GetAppMajorVersion();
		ASInt32 minor = sAIRuntime->GetAppMinorVersion();
#else
		// TODO: how to find this out on CS?
		ASInt32 major = 11;
		ASInt32 minor = 0;
#endif
		return major + minor / 10.0;
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * int getIllustratorRevision()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_ScriptographerEngine_getIllustratorRevision(JNIEnv *env, jclass cls) {
	try {
#if kPluginInterfaceVersion >= kAI12
		return sAIRuntime->GetAppRevisionVersion();
#else
		// TODO: how to find this out on CS?
		return 0;
#endif
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * boolean isActive()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_ScriptographerEngine_isActive(JNIEnv *env, jclass cls) {
	try {
		return gPlugin != NULL && gPlugin->isActive();
	} EXCEPTION_CONVERT(env);
	return false;
}
