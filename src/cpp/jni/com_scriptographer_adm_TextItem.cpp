/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Plugin for Adobe Illustrator.
 *
 * Copyright (c) 2002-2005 Juerg Lehni, http://www.scratchdisk.com.
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
 * $RCSfile: com_scriptographer_adm_TextItem.cpp,v $
 * $Author: lehni $
 * $Revision: 1.2 $
 * $Date: 2005/03/07 13:42:29 $
 */
 
#include "stdHeaders.h"
#include "ScriptographerEngine.h"
#include "com_scriptographer_adm_TextItem.h"

/*
 * com.scriptographer.adm.TextItem
 */

/*
 * void setFont(int font)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_TextItem_setFont(JNIEnv *env, jobject obj, jint font) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		sADMItem->SetFont(item, (ADMFont)font);
	} EXCEPTION_CONVERT(env)
}

/*
 * int getFont()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_TextItem_getFont(JNIEnv *env, jobject obj) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		return sADMItem->GetFont(item);
	} EXCEPTION_CONVERT(env)
	return 0;
}

/*
 * void nativeSetText(java.lang.String text)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_TextItem_nativeSetText(JNIEnv *env, jobject obj, jstring text) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		const jchar *chars = env->GetStringChars(text, NULL);
		if (chars == NULL) EXCEPTION_CHECK(env)
		sADMItem->SetTextW(item, chars);
		env->ReleaseStringChars(text, chars);
	} EXCEPTION_CONVERT(env)
}

/*
 * java.lang.String nativeGetText()
 */
JNIEXPORT jstring JNICALL Java_com_scriptographer_adm_TextItem_nativeGetText(JNIEnv *env, jobject obj) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		long len = sADMItem->GetTextLength(item);
		jchar *chars = new jchar[len];
		sADMItem->GetTextW(item, chars, len);
		jstring res = env->NewString(chars, len);
		if (res == NULL) EXCEPTION_CHECK(env)
		delete chars;
		return res;
	} EXCEPTION_CONVERT(env)
	return NULL;
}

/*
 * void setJustify(int Justify)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_TextItem_setJustify(JNIEnv *env, jobject obj, jint justify) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		sADMItem->SetJustify(item, (ADMJustify)justify);
	} EXCEPTION_CONVERT(env)
}

/*
 * int getJustify()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_TextItem_getJustify(JNIEnv *env, jobject obj) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		return sADMItem->GetJustify(item);
	} EXCEPTION_CONVERT(env)
	return 0;
}

/*
 * void setUnits(int units)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_TextItem_setUnits(JNIEnv *env, jobject obj, jint units) {
	try {
		ADMItemRef item = gEngine->getItemRef(env, obj);
		sADMItem->SetUnits(item, (ADMUnits)units);
	} EXCEPTION_CONVERT(env)
}

/*
 * int getUnits()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_TextItem_getUnits(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemRef(env, obj);
		return sADMItem->GetUnits(item);
	} EXCEPTION_CONVERT(env)
	return 0;
}

/*
 * void setShowUnits(boolean showUnits)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_TextItem_setShowUnits(JNIEnv *env, jobject obj, jboolean showUnits) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		sADMItem->ShowUnits(item, showUnits);
	} EXCEPTION_CONVERT(env)
}

/*
 * boolean getShowUnits()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_TextItem_getShowUnits(JNIEnv *env, jobject obj) {
	try {
	    ADMItemRef item = gEngine->getItemRef(env, obj);
		return sADMItem->GetShowUnits(item);
	} EXCEPTION_CONVERT(env)
	return JNI_FALSE;
}
