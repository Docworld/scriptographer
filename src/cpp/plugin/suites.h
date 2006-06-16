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
 * $RCSfile: suites.h,v $
 * $Author: lehni $
 * $Revision: 1.2 $
 * $Date: 2006/06/16 16:18:29 $
 */

// Sweet Pea Suites
#include "SPBlocks.h"
#include "SPAccess.h"
#include "SPInterf.h"
#include "SPRuntme.h" 
#include "SPSuites.h"
#include "SPFiles.h"

// ADM Suites
#include "ADMBasic.h"
#include "ADMDialog.h"
#include "ADMHost.h"
#include "ADMItem.h"
#include "ADMIcon.h"
#include "ADMImage.h"
#include "ADMList.h"
#include "ADMHierarchyList.h"
#include "ADMDialogGroup.h"
#include "ADMNotifier.h"
#include "ADMEntry.h"
#include "ADMListEntry.h"
#include "ADMTracker.h"
#include "ADMDrawer.h"
#include "ADMResource.h"

// System Suites
#include "AIPlugin.h"
#include "AIMDMemory.h"

// General Suites
#include "AIAnnotator.h"
#include "AIArray.h"
#include "AIArt.h"
#include "AIArtSet.h"
#include "AIBlock.h"
#include "AIOverrideColorConversion.h"
#include "AIColorConversion.h"
#include "AIContext.h"
#include "AICursorSnap.h"
#include "AICustomColor.h"
#include "AIDocument.h"
#include "AIDocumentList.h"
#include "AIDocumentView.h"
#include "AIFileFormat.h"
#include "AIFilter.h"
#include "AIGroup.h"
#include "AIHitTest.h"
#include "AILayer.h"
#include "AILiveEffect.h"
#include "AIMask.h"
#include "AIMatchingArt.h"
#include "AIMenu.h"
#include "AIMenuGroups.h"
#include "AINotifier.h"
#include "AIPaintStyle.h"
#include "AIPath.h"
#include "AIPathStyle.h"
#include "AIPathConstruction.h"
#include "AIPathfinder.h"
#include "AIPathInterpolate.h"
#include "AIPluginGroup.h"
#include "AIRandom.h"
#include "AIRaster.h"
#include "AIRasterize.h"
#include "AIRealMath.h"
#include "AIRuntime.h"
#include "AIShapeConstruction.h"
#include "AITabletData.h"
#include "AITag.h"
#include "AITimer.h"
#include "AITool.h"
#include "AITransformArt.h"
#include "AIUser.h"
#include "AIUndo.h"
#include "AIURL.h"

#include "AIContext.h"
#include "AIPreference.h"

#if kPluginInterfaceVersion >= kAI11
#include "AITextFrame.h"
#include "IText.h"
#include "AIATEPaint.h"
#else
#include "AIText.h"
#include "AITextFaceStyle.h"
#include "AITextLine.h"
#include "AITextPath.h"
#include "AITextRun.h"
#include "AITextStream.h"
#endif

typedef struct {
	char *name;
	int version;
	void *suite;
	int size;
} ImportSuite;

typedef struct {
	ImportSuite* suites;
	ASBoolean acquired;
} ImportSuites;

extern ImportSuites gStartupSuites;
extern ImportSuites gPostStartupSuites;

#if kPluginInterfaceVersion >= kAI11
	#include "ATETextSuitesExtern.h"
#endif

// ADM Suites default to the oldest versions.
// Define symbols that point to the newest here und use these bellow:

#define _kADMBasicSuiteVersion kADMBasicSuiteVersion8
#define _kADMDialogSuiteVersion kADMDialogSuiteVersion8
#define _kADMItemSuiteVersion kADMItemSuiteVersion8
#define _kADMIconSuiteVersion kADMIconSuiteVersion2
#define _kADMImageSuiteVersion kADMImageSuiteVersion2
#define _kADMListSuiteVersion kADMListSuiteVersion4
#define _kADMHierarchyListSuiteVersion kADMHierarchyListSuiteVersion5
#define _kADMDialogGroupSuiteVersion kADMDialogGroupSuiteVersion3
#define _kADMNotifierSuiteVersion kADMNotifierSuiteVersion1
#define _kADMEntrySuiteVersion kADMEntrySuiteVersion5
#define _kADMListEntrySuiteVersion kADMListEntrySuiteVersion4
#define _kADMTrackerSuiteVersion kADMTrackerSuiteVersion1
#define _kADMDrawerSuiteVersion kADMDrawerSuiteVersion5

#define _ADMBasicSuite ADMBasicSuite8			
#define _ADMDialogSuite ADMDialogSuite8
#define _ADMItemSuite ADMItemSuite8
#define _ADMIconSuite ADMIconSuite2
#define _ADMImageSuite ADMImageSuite2
#define _ADMListSuite ADMListSuite4
#define _ADMHierarchyListSuite ADMHierarchyListSuite5
#define _ADMDialogGroupSuite ADMDialogGroupSuite3
#define _ADMNotifierSuite ADMNotifierSuite1
#define _ADMEntrySuite ADMEntrySuite5
#define _ADMListEntrySuite ADMListEntrySuite4
#define _ADMTrackerSuite ADMTrackerSuite1
#define _ADMDrawerSuite ADMDrawerSuite5

// the basic suite doesn't need to be loaded:
extern "C" SPBasicSuite 					*sSPBasic;

// suite globals
extern "C" SPBlocksSuite					*sSPBlocks;
extern "C" AIMdMemorySuite					*sAIMDMemory;
extern "C" SPAccessSuite					*sSPAccess;
extern "C" SPFilesSuite						*sSPFiles;
extern "C" SPPluginsSuite 					*sSPPlugins;

extern "C" AIAnnotatorSuite					*sAIAnnotator;
extern "C" AIArraySuite						*sAIArray;
extern "C" AIArtSuite						*sAIArt;
extern "C" AIArtSetSuite					*sAIArtSet;
extern "C" AIATEPaintSuite					*sAIATEPaint;
extern "C" AIAppContextSuite				*sAIAppContext;
extern "C" AIBlendStyleSuite				*sAIBlendStyle;
extern "C" AIColorConversionSuite			*sAIColorConversion;
extern "C" AIOverrideColorConversionSuite	*sAIOverrideColorConversion;
extern "C" AICursorSnapSuite				*sAICursorSnap;
extern "C" AIDictionarySuite				*sAIDictionary;
extern "C" AIDictionaryIteratorSuite		*sAIDictionaryIterator;
extern "C" AIDocumentSuite					*sAIDocument;
extern "C" AIDocumentListSuite				*sAIDocumentList;
extern "C" AIDocumentViewSuite				*sAIDocumentView;
extern "C" AIEntrySuite						*sAIEntry;
extern "C" AIFileFormatSuite				*sAIFileFormat;
extern "C" AIFilterSuite					*sAIFilter;
extern "C" AIFontSuite						*sAIFont;
extern "C" AIGroupSuite						*sAIGroup;
extern "C" AIHitTestSuite					*sAIHitTest;
extern "C" AILayerSuite						*sAILayer;
extern "C" AILiveEffectSuite				*sAILiveEffect;
extern "C" AIMatchingArtSuite				*sAIMatchingArt;
extern "C" AIMenuSuite						*sAIMenu;
extern "C" AINotifierSuite					*sAINotifier;
extern "C" AIPaintStyleSuite				*sAIPaintStyle;
extern "C" AIPathConstructionSuite			*sAIPathConstruction;
extern "C" AIPathfinderSuite				*sAIPathfinder;
extern "C" AIPathInterpolateSuite			*sAIPathInterpolate;
extern "C" AIPathStyleSuite					*sAIPathStyle;
extern "C" AIPathSuite						*sAIPath;
extern "C" AIPluginGroupSuite				*sAIPluginGroup;
extern "C" AIPreferenceSuite				*sAIPreference;
extern "C" AIRasterSuite					*sAIRaster;
extern "C" AIRasterizeSuite					*sAIRasterize;
extern "C" AIRealBezierSuite				*sAIRealBezier;
extern "C" AIRealMathSuite					*sAIRealMath;
extern "C" AIShapeConstructionSuite			*sAIShapeConstruction;
extern "C" AITabletDataSuite				*sAITabletData;
extern "C" AITagSuite						*sAITag;
extern "C" AITextFrameHitSuite				*sAITextFrameHit;
extern "C" AIToolSuite						*sAITool;
extern "C" AITimerSuite						*sAITimer;
extern "C" AITransformArtSuite				*sAITransformArt;
extern "C" AIUserSuite						*sAIUser;
extern "C" AIUndoSuite						*sAIUndo;
extern "C" AIURLSuite						*sAIURL;

extern "C" _ADMBasicSuite					*sADMBasic;
extern "C" _ADMDialogSuite 					*sADMDialog;
extern "C" _ADMItemSuite					*sADMItem;
extern "C" _ADMIconSuite					*sADMIcon;
extern "C" _ADMImageSuite					*sADMImage;
extern "C" _ADMListSuite					*sADMList;
extern "C" _ADMHierarchyListSuite			*sADMHierarchyList;
extern "C" _ADMDialogGroupSuite				*sADMDialogGroup;
extern "C" _ADMNotifierSuite				*sADMNotifier;
extern "C" _ADMEntrySuite					*sADMEntry;
extern "C" _ADMListEntrySuite				*sADMListEntry;
extern "C" _ADMTrackerSuite					*sADMTracker;
extern "C" _ADMDrawerSuite					*sADMDrawer;

#ifdef WIN_ENV
extern "C" ADMWinHostSuite					*sADMWinHost;
#endif

#if kPluginInterfaceVersion >= kAI11
extern "C" AITextFrameSuite					*sAITextFrame;
#else
extern "C" AITextSuite						*sAIText;
extern "C" AITextFaceStyleSuite				*sAITextFaceStyle;
extern "C" AITextLineSuite					*sAITextLine;
extern "C" AITextPathSuite					*sAITextPath;
extern "C" AITextRunSuite					*sAITextRun;
extern "C" AITextStreamSuite				*sAITextStream;
#endif

#if kPluginInterfaceVersion >= kAI12
extern "C" AIFilePathSuite					*sAIFilePath;
extern "C" AIUnicodeStringSuite				*sAIUnicodeString;
#endif