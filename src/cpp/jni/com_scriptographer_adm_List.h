/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_scriptographer_adm_List */

#ifndef _Included_com_scriptographer_adm_List
#define _Included_com_scriptographer_adm_List
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_scriptographer_adm_List
 * Method:    nativeSetTrackCallbackEnabled
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_nativeSetTrackCallbackEnabled
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    nativeSetDrawCallbackEnabled
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_nativeSetDrawCallbackEnabled
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    setEntrySize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_setEntrySize
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getEntrySize
 * Signature: ()Ljava/awt/Dimension;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_List_getEntrySize
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    setEntryTextRect
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_setEntryTextRect
  (JNIEnv *, jobject, jint, jint, jint, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getEntryTextRect
 * Signature: ()Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_List_getEntryTextRect
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    createEntry
 * Signature: (I)Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_List_createEntry
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    removeEntry
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_removeEntry
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getEntry
 * Signature: (I)Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_List_getEntry__I
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getEntry
 * Signature: (Ljava/lang/String;)Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_List_getEntry__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getEntry
 * Signature: (II)Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_List_getEntry__II
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getActiveEntry
 * Signature: ()Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_List_getActiveEntry
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getEntries
 * Signature: ()[Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobjectArray JNICALL Java_com_scriptographer_adm_List_getEntries
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getSelectedEntries
 * Signature: ()[Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobjectArray JNICALL Java_com_scriptographer_adm_List_getSelectedEntries
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getNumEntries
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_List_getNumEntries
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    setTrackMask
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_setTrackMask
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    getTrackMask
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_List_getTrackMask
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    nativeSetBGColor
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_nativeSetBGColor
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_List
 * Method:    selectByText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_List_selectByText
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif
