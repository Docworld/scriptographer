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
#include "aiGlobals.h"
#include "com_scriptographer_ai_ParagraphStyle.h"

/*
 * com.scriptographer.ai.ParagraphStyle
 */

using namespace ATE;

#define PARAGRAPHSTYLE_GET(NAME, TYPE, CLASS, JTYPE) \
	try { \
		ParaFeaturesRef features = gEngine->getParaFeaturesHandle(env, obj); \
		ATEBool8 isAssigned; \
		TYPE value; \
		if (!sParaFeatures->Get##NAME(features, &isAssigned, &value) && isAssigned) \
			return gEngine->newObject(env, gEngine->cls_##CLASS, gEngine->cid_##CLASS, (JTYPE) value); \
	} EXCEPTION_CONVERT(env); \
	return NULL;

#define PARAGRAPHSTYLE_SET_CLEAR(NAME, CLEAR, TYPE, METHOD_TYPE, METHOD_NAME) \
	try { \
		ParaFeaturesRef features = gEngine->getParaFeaturesHandle(env, obj); \
		ASErr err; \
		if (value == NULL) \
			err = sParaFeatures->Clear##CLEAR(features); \
		else \
			err = sParaFeatures->Set##NAME(features, (TYPE) gEngine->call##METHOD_TYPE##Method(env, value, gEngine->METHOD_NAME)); \
		if (!err) \
			gEngine->callVoidMethod(env, obj, gEngine->mid_ai_ParagraphStyle_markSetStyle); \
	} EXCEPTION_CONVERT(env);

#define PARAGRAPHSTYLE_SET(NAME, TYPE, METHOD_TYPE, METHOD_NAME) \
	PARAGRAPHSTYLE_SET_CLEAR(NAME, NAME, TYPE, METHOD_TYPE, METHOD_NAME)

#define PARAGRAPHSTYLE_GET_FLOAT(NAME) \
	PARAGRAPHSTYLE_GET(NAME, ASReal, Float, jfloat)

#define PARAGRAPHSTYLE_SET_FLOAT(NAME) \
	PARAGRAPHSTYLE_SET(NAME, ASReal, Float, mid_Number_floatValue)

#define PARAGRAPHSTYLE_SET_FLOAT_CLEAR(NAME, CLEAR) \
	PARAGRAPHSTYLE_SET_CLEAR(NAME, CLEAR, ASReal, Float, mid_Number_floatValue)

#define PARAGRAPHSTYLE_GET_BOOLEAN(NAME) \
	PARAGRAPHSTYLE_GET(NAME, ATEBool8, Boolean, jboolean)

#define PARAGRAPHSTYLE_SET_BOOLEAN(NAME) \
	PARAGRAPHSTYLE_SET(NAME, ATEBool8, Boolean, mid_Boolean_booleanValue)

#define PARAGRAPHSTYLE_GET_INTEGER(NAME) \
	PARAGRAPHSTYLE_GET(NAME, ASInt32, Integer, jint)

#define PARAGRAPHSTYLE_SET_INTEGER(NAME) \
	PARAGRAPHSTYLE_SET(NAME, ASInt32, Int, mid_Number_intValue)

#define PARAGRAPHSTYLE_GET_ENUM(NAME, TYPE) \
	PARAGRAPHSTYLE_GET(NAME, TYPE, Integer, jint)

#define PARAGRAPHSTYLE_SET_ENUM(NAME, TYPE) \
	PARAGRAPHSTYLE_SET(NAME, TYPE, Int, mid_Number_intValue)


/*
 * int nativeCreate()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeCreate(JNIEnv *env, jclass cls) {
	try {
		ParaFeaturesRef features;
		if (!sParaFeatures->Initialize(&features)) {
			// add reference to the handle, which will be released in ParagraphStyle.finalize
			sParaFeatures->AddRef(features);
			return (jint) features;
		}
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * int nativeClone()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeClone(JNIEnv *env, jobject obj) {
	try {
		ParaFeaturesRef features = gEngine->getParaFeaturesHandle(env, obj);
		ParaFeaturesRef clone;
		if (!sParaFeatures->Clone(features, &clone)) {
			// add reference to the handle, which will be released in ParagraphStyle.finalize
			sParaFeatures->AddRef(clone);
			return (jint) clone;
		}
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeSetStyle(int handle, int rangeHandle)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeSetStyle(JNIEnv *env, jobject obj, jint handle, jint rangeHandle) {
	try {
		sTextRange->SetLocalParaFeatures((TextRangeRef) rangeHandle, (ParaFeaturesRef) handle);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeRelease()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeRelease(JNIEnv *env, jobject obj, jint handle) {
	try {
		if (handle)
			sParaFeatures->Release((ParaFeaturesRef) handle);
	} EXCEPTION_CONVERT(env);
}

/*
 * java.lang.Integer nativeGetJustification()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeGetJustification(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_ENUM(Justification, ParagraphJustification)
}

/*
 * void nativeSetJustification(java.lang.Integer value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeSetJustification(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_ENUM(Justification, ParagraphJustification)
}

/*
 * java.lang.Float getFirstLineIndent()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getFirstLineIndent(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(FirstLineIndent)
}

/*
 * void setFirstLineIndent(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setFirstLineIndent(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(FirstLineIndent)
}

/*
 * java.lang.Float getStartIndent()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getStartIndent(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(StartIndent)
}

/*
 * void setStartIndent(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setStartIndent(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(StartIndent)
}

/*
 * java.lang.Float getEndIndent()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getEndIndent(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(EndIndent)
}

/*
 * void setEndIndent(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setEndIndent(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(EndIndent)
}

/*
 * java.lang.Float getSpaceBefore()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getSpaceBefore(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(SpaceBefore)
}

/*
 * void setSpaceBefore(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setSpaceBefore(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(SpaceBefore)
}

/*
 * java.lang.Float getSpaceAfter()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getSpaceAfter(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(SpaceAfter)
}

/*
 * void setSpaceAfter(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setSpaceAfter(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(SpaceAfter)
}

/*
 * java.lang.Boolean getHyphenation()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getHyphenation(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_BOOLEAN(AutoHyphenate)
}

/*
 * void setHyphenation(java.lang.Boolean value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setHyphenation(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_BOOLEAN(AutoHyphenate)
}

/*
 * java.lang.Integer getHyphenatedWordSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getHyphenatedWordSize(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_INTEGER(HyphenatedWordSize)
}

/*
 * void setHyphenatedWordSize(java.lang.Integer value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setHyphenatedWordSize(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_INTEGER(HyphenatedWordSize)
}

/*
 * java.lang.Integer getPreHyphenSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getPreHyphenSize(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_INTEGER(PreHyphenSize)
}

/*
 * void setPreHyphenSize(java.lang.Integer value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setPreHyphenSize(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_INTEGER(PreHyphenSize)
}

/*
 * java.lang.Integer getPostHyphenSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getPostHyphenSize(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_INTEGER(PostHyphenSize)
}

/*
 * void setPostHyphenSize(java.lang.Integer value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setPostHyphenSize(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_INTEGER(PostHyphenSize)
}

/*
 * java.lang.Integer getConsecutiveHyphenLimit()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getConsecutiveHyphenLimit(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_INTEGER(ConsecutiveHyphenLimit)
}

/*
 * void setConsecutiveHyphenLimit(java.lang.Integer value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setConsecutiveHyphenLimit(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_INTEGER(ConsecutiveHyphenLimit)
}

/*
 * java.lang.Float getHyphenationZone()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getHyphenationZone(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(HyphenationZone)
}

/*
 * void setHyphenationZone(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setHyphenationZone(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(HyphenationZone)
}

/*
 * java.lang.Boolean getHyphenateCapitalized()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getHyphenateCapitalized(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_BOOLEAN(HyphenateCapitalized)
}

/*
 * void setHyphenateCapitalized(java.lang.Boolean value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setHyphenateCapitalized(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_BOOLEAN(HyphenateCapitalized)
}

/*
 * java.lang.Float getHyphenationPreference()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getHyphenationPreference(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(HyphenationPreference)
}

/*
 * void setHyphenationPreference(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setHyphenationPreference(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(HyphenationPreference)
}

/*
 * java.lang.Float getDesiredWordSpacing()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getDesiredWordSpacing(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(DesiredWordSpacing)
}

/*
 * void setDesiredWordSpacing(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setDesiredWordSpacing(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(DesiredWordSpacing, WordSpacing)
}

/*
 * java.lang.Float getMaxWordSpacing()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getMaxWordSpacing(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(MaxWordSpacing)
}

/*
 * void setMaxWordSpacing(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setMaxWordSpacing(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(MaxWordSpacing, WordSpacing)
}

/*
 * java.lang.Float getMinWordSpacing()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getMinWordSpacing(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(MinWordSpacing)
}

/*
 * void setMinWordSpacing(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setMinWordSpacing(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(MinWordSpacing, WordSpacing)
}

/*
 * java.lang.Float getDesiredLetterSpacing()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getDesiredLetterSpacing(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(DesiredLetterSpacing)
}

/*
 * void setDesiredLetterSpacing(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setDesiredLetterSpacing(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(DesiredLetterSpacing, LetterSpacing)
}

/*
 * java.lang.Float getMaxLetterSpacing()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getMaxLetterSpacing(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(MaxLetterSpacing)
}

/*
 * void setMaxLetterSpacing(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setMaxLetterSpacing(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(MaxLetterSpacing, LetterSpacing)
}

/*
 * java.lang.Float getMinLetterSpacing()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getMinLetterSpacing(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(MinLetterSpacing)
}

/*
 * void setMinLetterSpacing(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setMinLetterSpacing(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(MinLetterSpacing, LetterSpacing)
}

/*
 * java.lang.Float getDesiredGlyphScaling()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getDesiredGlyphScaling(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(DesiredGlyphScaling)
}

/*
 * void setDesiredGlyphScaling(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setDesiredGlyphScaling(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(DesiredGlyphScaling, GlyphScaling)
}

/*
 * java.lang.Float getMaxGlyphScaling()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getMaxGlyphScaling(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(MaxGlyphScaling)
}

/*
 * void setMaxGlyphScaling(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setMaxGlyphScaling(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(MaxGlyphScaling, GlyphScaling)
}

/*
 * java.lang.Float getMinGlyphScaling()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getMinGlyphScaling(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(MinGlyphScaling)
}

/*
 * void setMinGlyphScaling(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setMinGlyphScaling(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT_CLEAR(MinGlyphScaling, GlyphScaling)
}

/*
 * java.lang.Integer nativeGetSingleWordJustification()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeGetSingleWordJustification(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_ENUM(SingleWordJustification, ParagraphJustification)
}

/*
 * void nativeSetSingleWordJustification(java.lang.Integer value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeSetSingleWordJustification(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_ENUM(SingleWordJustification, ParagraphJustification)
}

/*
 * java.lang.Float getAutoLeadingPercentage()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getAutoLeadingPercentage(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(AutoLeadingPercentage)
}

/*
 * void setAutoLeadingPercentage(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setAutoLeadingPercentage(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_FLOAT(AutoLeadingPercentage)
}

/*
 * java.lang.Integer nativeGetLeading()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeGetLeading(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_ENUM(LeadingType, LeadingType)
}

/*
 * void nativeSetLeading(java.lang.Integer value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_nativeSetLeading(JNIEnv *env, jobject obj, jobject value) {
	PARAGRAPHSTYLE_SET_ENUM(LeadingType, LeadingType)
}

/*
 * java.lang.Float getDefaultTabWidth()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_ai_ParagraphStyle_getDefaultTabWidth(JNIEnv *env, jobject obj) {
	PARAGRAPHSTYLE_GET_FLOAT(DefaultTabWidth)
}

/*
 * void setDefaultTabWidth(java.lang.Float value)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_ai_ParagraphStyle_setDefaultTabWidth(JNIEnv *env, jobject obj, jobject value) {
	try {
		// no macro here as clearing is not possible for this value
		ParaFeaturesRef features = gEngine->getParaFeaturesHandle(env, obj);
		if (value != NULL && !sParaFeatures->SetDefaultTabWidth(features, (ASReal) gEngine->callFloatMethod(env, value, gEngine->mid_Number_floatValue)))
			gEngine->callVoidMethod(env, obj, gEngine->mid_ai_ParagraphStyle_markSetStyle);
	} EXCEPTION_CONVERT(env);
}
