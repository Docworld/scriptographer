/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_scriptographer_adm_ListEntry */

#ifndef _Included_com_scriptographer_adm_ListEntry
#define _Included_com_scriptographer_adm_ListEntry
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    nativeCreate
 * Signature: (Lcom/scriptographer/adm/List;)I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_ListEntry_nativeCreate
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    nativeDestroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_nativeDestroy
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    getIndex
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_ListEntry_getIndex
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    getList
 * Signature: ()Lcom/scriptographer/adm/List;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_ListEntry_getList
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    setSelected
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_setSelected
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    isSelected
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_ListEntry_isSelected
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    makeInBounds
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_makeInBounds
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    isInBounds
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_ListEntry_isInBounds
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    setEnabled
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_setEnabled
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    isEnabled
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_ListEntry_isEnabled
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    setActive
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_setActive
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    isActive
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_ListEntry_isActive
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    setChecked
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_setChecked
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    isChecked
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_ListEntry_isChecked
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    setSeparator
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_setSeparator
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    isSeparator
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_ListEntry_isSeparator
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    getLocalRect
 * Signature: ()Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_ListEntry_getLocalRect
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    getBounds
 * Signature: ()Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_ListEntry_getBounds
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    localToScreen
 * Signature: (II)Ljava/awt/Point;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_ListEntry_localToScreen__II
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    screenToLocal
 * Signature: (II)Ljava/awt/Point;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_ListEntry_screenToLocal__II
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    localToScreen
 * Signature: (IIII)Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_ListEntry_localToScreen__IIII
  (JNIEnv *, jobject, jint, jint, jint, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    screenToLocal
 * Signature: (IIII)Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_ListEntry_screenToLocal__IIII
  (JNIEnv *, jobject, jint, jint, jint, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    invalidate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_invalidate__
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    invalidate
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_invalidate__IIII
  (JNIEnv *, jobject, jint, jint, jint, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    update
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_update
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    nativeSetPicture
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_nativeSetPicture
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    nativeSetSelectedPicture
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_nativeSetSelectedPicture
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    nativeSetDisabledPicture
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_nativeSetDisabledPicture
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_ListEntry
 * Method:    nativeSetText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_ListEntry_nativeSetText
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
