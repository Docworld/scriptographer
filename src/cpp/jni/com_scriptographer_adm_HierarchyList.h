/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_scriptographer_adm_HierarchyList */

#ifndef _Included_com_scriptographer_adm_HierarchyList
#define _Included_com_scriptographer_adm_HierarchyList
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    setEntrySize
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_setEntrySize
  (JNIEnv *, jobject, jint, jint, jboolean);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    setEntryTextRect
 * Signature: (IIIIZ)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_setEntryTextRect
  (JNIEnv *, jobject, jint, jint, jint, jint, jboolean);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getNonLeafEntryWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_HierarchyList_getNonLeafEntryWidth
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    setNonLeafEntryTextRect
 * Signature: (IIIIZ)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_setNonLeafEntryTextRect
  (JNIEnv *, jobject, jint, jint, jint, jint, jboolean);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getNonLeafEntryTextRect
 * Signature: ()Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_getNonLeafEntryTextRect
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getLeafEntry
 * Signature: (II)Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_getLeafEntry
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getActiveLeafEntry
 * Signature: ()Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_getActiveLeafEntry
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getAllSelectedEntries
 * Signature: ()[Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobjectArray JNICALL Java_com_scriptographer_adm_HierarchyList_getAllSelectedEntries
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getAllUnnestedSelectedEntries
 * Signature: ()[Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobjectArray JNICALL Java_com_scriptographer_adm_HierarchyList_getAllUnnestedSelectedEntries
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getParentEntry
 * Signature: ()Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_getParentEntry
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getLocalRect
 * Signature: ()Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_getLocalRect
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    localToScreen
 * Signature: (II)Ljava/awt/Point;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_localToScreen
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    screenToLocal
 * Signature: (II)Ljava/awt/Point;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_screenToLocal
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    localToGlobal
 * Signature: (II)Ljava/awt/Point;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_localToGlobal__II
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    globalToLocal
 * Signature: (II)Ljava/awt/Point;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_globalToLocal__II
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    localToGlobal
 * Signature: (IIII)Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_localToGlobal__IIII
  (JNIEnv *, jobject, jint, jint, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    globalToLocal
 * Signature: (IIII)Ljava/awt/Rectangle;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_globalToLocal__IIII
  (JNIEnv *, jobject, jint, jint, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    setIndentationWidth
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_setIndentationWidth
  (JNIEnv *, jobject, jint, jboolean);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getIndentationWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_HierarchyList_getIndentationWidth
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    setLocalLeftMargin
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_setLocalLeftMargin
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getLocalLeftMargin
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_HierarchyList_getLocalLeftMargin
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getGlobalLeftMargin
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_HierarchyList_getGlobalLeftMargin
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    setDivided
 * Signature: (ZZ)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_setDivided
  (JNIEnv *, jobject, jboolean, jboolean);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    isDivided
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_HierarchyList_isDivided
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    setFlags
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_setFlags
  (JNIEnv *, jobject, jint, jboolean);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getFlags
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_HierarchyList_getFlags
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    invalidate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_invalidate
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getLeafEntries
 * Signature: ()[Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobjectArray JNICALL Java_com_scriptographer_adm_HierarchyList_getLeafEntries
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getLeafIndex
 * Signature: (Lcom/scriptographer/adm/ListEntry;)I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_HierarchyList_getLeafIndex
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    swapEntries
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_swapEntries
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    insertEntry
 * Signature: (Lcom/scriptographer/adm/ListEntry;I)Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_insertEntry
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    unlinkEntry
 * Signature: (I)Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_HierarchyList_unlinkEntry
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    deselectAll
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_HierarchyList_deselectAll
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getExpandedEntries
 * Signature: ()[Lcom/scriptographer/adm/ListEntry;
 */
JNIEXPORT jobjectArray JNICALL Java_com_scriptographer_adm_HierarchyList_getExpandedEntries
  (JNIEnv *, jobject);

/*
 * Class:     com_scriptographer_adm_HierarchyList
 * Method:    getExpandedIndex
 * Signature: (Lcom/scriptographer/adm/ListEntry;)I
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_HierarchyList_getExpandedIndex
  (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif
