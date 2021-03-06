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
 * 
 * File created on 23.10.2005.
 */

package com.scriptographer.ai;

import com.scratchdisk.list.ReadOnlyList;
import com.scratchdisk.util.IntegerEnumUtils;
import com.scriptographer.CommitManager;

/**
 * The TextItem type allows you to access and modify the text items in
 * Illustrator documents. Its functionality is inherited by different text item
 * types such as {@link PointText}, {@link PathText} and {@link AreaText}. They
 * each add a layer of functionality that is unique to their type, but share the
 * underlying properties and functions that they inherit from TextItem.
 * 
 * @author lehni
 */
public abstract class TextItem extends Item implements TextStoryProvider {

	// AITextType
	protected static final short
		TEXTTYPE_UNKNOWN = -1,
		TEXTTYPE_POINT = 0,
		TEXTTYPE_AREA = 1,
		TEXTTYPE_PATH = 2;

	TextRange range = null;
	TextRange visibleRange = null;

	protected TextItem(int handle, int docHandle, boolean created) {
		super(handle, docHandle, created);
	}

	protected TextItem(int handle) {
		super(handle);
	}

	protected boolean commitAndInvalidate(boolean invalidate) {
		// Commit changes to anything depending on the item's story as well.
		CommitManager.commit(getStory());
		return super.commitAndInvalidate(invalidate);
	}

	private native int nativeGetOrientation();
	private native void nativeSetOrientation(int orientation);

	/**
	 * The orientation of the text in the text item.
	 */
	public TextOrientation getOrientation() {
		return IntegerEnumUtils.get(TextOrientation.class,
				nativeGetOrientation());
	}

	public void setOrientation(TextOrientation orientation) {
		nativeSetOrientation(orientation.value);
	}

	/**
	 * Specifies whether to use optical alignment within the text item. Optical
	 * alignment hangs punctuation outside the edges of a text item.
	 * 
	 * @return {@true if the text item uses optical alignment}
	 */
	public native boolean getOpticalAlignment();

	public native void setOpticalAlignment(boolean active);
	
	// TODO:
	 // AIAPI AIErr (*DoTextFrameHit)	( const AIHitRef hitRef, TextRangeRef* textRange );

	private native Item nativeCreateOutline();

	/**
	 * Converts the text in the text item to outlines. Unlike the Illustrator
	 * 'Create Outlines' action, this won't remove the text item.
	 * 
	 * @return a {@link Group} item containing the outlined text as {@link Path}
	 *         and {@link CompoundPath} items.
	 */
	public Item createOutline() {
		// Apply changes and reflow the layout before creating outlines
		// All styles regarding this story need to be committed, as
		// CharacterStyle uses Story as the commit key.
		CommitManager.commit(this.getStory());
		// This should not be needed since TextRange takes care of it
		// when committing already:
		// document.reflowText();
		return nativeCreateOutline();
	}

	/**
	 * {@grouptitle Text Item Linking}
	 * 
	 * Links the supplied text item to this one.
	 * 
	 * @param next The text item that will be linked
	 * @return {@true if the text item was linked}
	 */
	public native boolean link(TextItem next);

	/**
	 * Returns {@true if the text item is linked}
	 */
	public native boolean isLinked();
	
	private native boolean nativeUnlink(boolean before, boolean after);

	/**
	 * Unlinks the text item from its current story.
	 * 
	 * @return {@true if the operation as successful}
	 */
	public boolean unlink() {
		return nativeUnlink(true, true);
	}

	/**
	 * Unlinks the text item from its current story and breaks up the story
	 * into two parts before the text item.
	 * 
	 * @return {@true if the operation as successful}
	 */
	public boolean unlinkBefore() {
		return nativeUnlink(true, false);
	}

	/**
	 * Unlinks the text item from its current story and breaks up the story
	 * into two parts after the text item.
	 * 
	 * @return {@true if the operation as successful}
	 */
	public boolean unlinkAfter() {
		return nativeUnlink(false, true);
	}

	/**
	 * {@grouptitle Hierarchy}
	 * 
	 * Returns the index of the text item in its {@link TextItem#getStory()}.
	 */
	public native int getIndex();

	/**
	 * Returns this text item's story's index in the document's stories array.
	 */
	private native int getStoryIndex();

	/*
	 * @see TextStoryProvider.
	 */
	public native int getStoryHandle();

	/**
	 * Returns the story that the text item belongs to.
	 */
	public TextStory getStory() {
		// don't wrap directly. always go through StoryList
		// to make sure we're not getting more than one reference
		// to the same Story, so things can be cached there:
		int index = getStoryIndex();
		TextStoryList list = document.getStories(this, true);
		if (index >= 0 && index < list.size())
			return list.get(index);
		return null;
	}

	private TextItem getTextItem(int index) {
		TextStory story = getStory();
		if (story != null) {
			ReadOnlyList<TextItem> list = story.getTextItems();
			if (index >= 0 && index < list.size())
				return list.get(index);
		}
		return null;
	}

	/**
	 * Returns the next text item in a story of various linked text items.
	 */
	public TextItem getNextTextItem() {
		return getTextItem(getIndex() + 1);
	}

	/**
	 * Returns the previous text item in a story of various linked text items.
	 */
	public TextItem getPreviousTextItem() {
		return getTextItem(getIndex() - 1);
	}

	// ATE

	/**
	 * {@grouptitle Range Properties}
	 * 
	 * The text contents of the text item.
	 */
	public String getContent() {
		return getRange().getContent();
	}

	public void setContent(String text) {
		getRange().setContent(text);
	}
	
	/**
	 * @jshide
	 */
	public native int nativeGetRange(boolean includeOverflow);

	/**
	 * Returns a text range for all the characters, even the invisible ones
	 * outside the container.
	 */
	public TextRange getRange() {
		// once a range object is created, always return the same reference
		// and swap handles instead. like this references in JS remain...
		if (range == null) {
			range = new TextRange(nativeGetRange(true), document);
		} else if (range.version != CommitManager.version) {
			range.changeHandle(nativeGetRange(true));
		}
		return range;
	}

	/**
	 * In case there's an overflow in the text, this only returns a range over
	 * the visible characters, while {@link TextItem#getRange()} returns one
	 * over the whole text.
	 */
	public TextRange getVisibleRange() {
		// once a range object is created, always return the same reference
		// and swap handles instead. like this references in JS remain...
		if (visibleRange == null) {
			visibleRange = new TextRange(nativeGetRange(false), document);
		} else if (visibleRange.version != CommitManager.version) {
			visibleRange.changeHandle(nativeGetRange(false));
		}
		return visibleRange;
	}

	/**
	 * Returns the selected text of the text item as a text range.
	 * 
	 * @jshide
	 */
	public native TextRange getSelectedRange();
	
	/**
	 * Returns the index of the first visible character of the text item. (this
	 * is the equivalent of calling TextItem.visibleRange.start)
	 */
	public int getStart() {
		return getVisibleRange().getStart();
	}

	/**
	 * Returns the index of the last visible character of the text item. (this
	 * is the equivalent of calling TextItem.visibleRange.end)
	 */
	public int getEnd() {
		return getVisibleRange().getEnd();
	}

	/**
	 * {@grouptitle Sub Ranges}
	 * 
	 * The text ranges of the words contained within the text item. Note that
	 * the returned text range includes the trailing whitespace characters of
	 * the words.
	 * 
	 * Sample code:
	 * <code>
	 * var text = new PointText(new Point(0,0));
	 * text.content = 'The contents of the point text.';
	 * var word = text.words[1];
	 * print(word.content) // 'contents ' - note the space after 'contents';
	 * </code>
	 */
	public ReadOnlyList<TextRange> getWords() {
		return getRange().getWords();
	}

	/**
	 * The text ranges of the paragraphs contained within the text item. Note
	 * that the returned text range includes the trailing paragraph (\r)
	 * characters of the paragraphs.
	 * 
	 * Sample code:
	 * <code>
	 * var text = new PointText(new Point(0,0));
	 * 
	 * // ('\r' is the escaped character that specifies a new paragraph)
	 * text.content = 'First paragraph\rSecond paragraph';
	 * var paragraph = text.paragraphs[1];
	 * print(paragraph.content) //returns 'Second paragraph';
	 * </code>
	 */
	public ReadOnlyList<TextRange> getParagraphs() {
		return getRange().getParagraphs();
	}

	/**
	 * The text ranges of the characters contained within the text item.
	 * 
	 * Sample code:
	 * <code>
	 * var text = new PointText(new Point(0,0));
	 * text.content = 'abc';
	 * var character = text.characters[1];
	 * print(character.content) //returns 'b';
	 * </code>
	 */	
	public ReadOnlyList<TextRange> getCharacters() {
		return getRange().getCharacters();
	}

	/**
	 * {@grouptitle Style Properties}
	 * 
	 * The character style of the text item.
	 */
	public CharacterStyle getCharacterStyle() {
		return getRange().getCharacterStyle();
	}

	public void setCharacterStyle(CharacterStyle style) {
		getRange().setCharacterStyle(style);
	}

	/**
	 * The paragraph style of the text item.
	 */
	public ParagraphStyle getParagraphStyle() {
		return getRange().getParagraphStyle();
	}

	public void setParagraphStyle(ParagraphStyle style) {
		getRange().setParagraphStyle(style);
	}

	public native boolean equals(Object obj);

	// TODO:
	//	ATEErr (*GetTextLinesIterator) ( TextFrameRef textframe, TextLinesIteratorRef* ret);

	//	ATEErr (*GetLineOrientation) ( TextFrameRef textframe, LineOrientation* ret);
	//	ATEErr (*SetLineOrientation) ( TextFrameRef textframe, LineOrientation lineOrientation);

	/* Check if this frame is selected.  To set the selection, you have to use application specific
	API for that.  In Illustrator case, you can use AIArtSuite to set the selection.
	*/
}