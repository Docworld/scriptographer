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
#include "ScriptographerEngine.h"
#include "ScriptographerPlugin.h"
#include "AppContext.h"
#include "com_scriptographer_ai_Timer.h"

/*
 * com.scriptographer.ai.Timer
 */

#ifdef WIN_ENV
void CALLBACK Dialog_onTimer(HWND wnd, UINT msg, UINT_PTR timerId, DWORD time) {
#endif // WIN_ENV

#ifdef MAC_ENV

void Dialog_onTimer(EventLoopTimerRef timerId, void* data) {
#endif // MAC_ENV
	// Establish an application context for undoing.
	AppContext context;
	JNIEnv *env = gEngine->getEnv();
	try {
		if (gEngine->callStaticBooleanMethod(env, gEngine->cls_ai_Timer,
				gEngine->mid_ai_Timer_onExecute, (jint) timerId)) {
			// If onExecute returns true, we are asked to redraw the current
			// document. We can call RedrawDocument() even if there is no
			// document, in which case it will just bail out.
#if defined(MAC_ENV) && kPluginInterfaceVersion < kAI15
			// Unfortunately, calling Redraw on Mac CS4 and below lets the mouse
			// cursor flicker a lot. So backup the cursor and restore it again
			// after, in a rather complicated procedure that still leads to some
			// flicker...
			// Use a sort of double bufferning teqnique of two alternating named
			// pixmap cursors, to avoid flicker even more...
			static int cursorVersion = 0;
			static const char *cursorNames[] = {
				"Scriptographer Cursor 1",
				"Scriptographer Cursor 2"
			};
			const char *name = cursorNames[cursorVersion];
			cursorVersion = !cursorVersion;
			PixMapHandle data;
			Point point;
			QDGetCursorData(true, &data, &point);
			QDUnregisterNamedPixMapCursor(name);
			QDRegisterNamedPixMapCursor(data, NULL, point, name);
			DisposePtr((**data).baseAddr);
			DisposePixMap(data);
			sAIDocument->RedrawDocument();
			QDSetNamedPixMapCursor(name);
#else // !MAC_ENV || kPluginInterfaceVersion >= kAI15 
			sAIDocument->RedrawDocument();
#endif // !MAC_ENV || kPluginInterfaceVersion >= kAI15 
		}
	} EXCEPTION_CATCH_REPORT(env);
}

/*
 * int nativeCreate(int period)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_ai_Timer_nativeCreate(
		JNIEnv *env, jobject obj, jint period) {
	try {
#ifdef WIN_ENV
		return (jint) SetTimer(NULL, NULL, period, Dialog_onTimer);
#endif // WIN_ENV
#ifdef MAC_ENV
		DEFINE_CALLBACK_PROC(Dialog_onTimer);
		static EventLoopTimerUPP timerUPP = NewEventLoopTimerUPP(
				(EventLoopTimerProcPtr) CALLBACK_PROC(Dialog_onTimer));
		EventLoopTimerRef timer;
		InstallEventLoopTimer(GetMainEventLoop(), 0,
                kEventDurationMillisecond * period, timerUPP, NULL, &timer);
		return (jint) timer;
#endif // MAC_ENV
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeAbort(int handle)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_Timer_nativeAbort(
		JNIEnv *env, jobject obj, jint handle) {
	try {
#ifdef WIN_ENV
		KillTimer(NULL, (UINT_PTR) handle);
#endif // WIN_ENV
#ifdef MAC_ENV
		RemoveEventLoopTimer((EventLoopTimerRef) handle);
#endif // MAC_ENV
	} EXCEPTION_CONVERT(env);
}
