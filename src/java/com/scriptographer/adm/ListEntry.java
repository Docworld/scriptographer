package com.scriptographer.adm;

import java.io.IOException;
import java.awt.*;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

import com.scriptographer.js.Unsealed;
import com.scriptographer.js.WrappableObject;

public class ListEntry extends WrappableObject implements Unsealed {
	private int entryRef = 0;
	
	private String text;

	private Image picture;
	private Image selectedPicture;
	private Image disabledPicture;

	protected ListEntry(int entryRef) {
		this.entryRef = entryRef;
	}

	/*
	 * entry creation/destruction
	 * 
	 */

	/* 
	 * TODO: implement these?, implement constructor?
	 */
	private native int nativeCreate(List list);
	private native void nativeDestroy();


	/* 
	 * contaer accessors
	 * 
	 */
	
	public native int getIndex();
	public native List getList();

	/* 
	 * entry ID
	 * 
	 */
	/*
	public native void setID(int entryID);
	public native int getID();
	*/
	
	/* 
	 * entry selection segmentFlags
	 * 
	 */

	public native void setSelected(boolean select);
	public native boolean isSelected();

	/* 
	 * entry visibility
	 * 
	 */
	
	public native void makeInBounds();
	public native boolean isInBounds();

	/* 
	 * entry state accessors
	 * 
	 */

	public native void setEnabled(boolean enable);
	public native boolean isEnabled();
	
	public native void setActive(boolean activate);
	public native boolean isActive();
	
	public native void setChecked(boolean check);
	public native boolean isChecked();
	
	public native void setSeparator(boolean separator);
	public native boolean isSeparator();
/* 
	 * entry bounds accessors
	 * 
	 */

	public native Rectangle getLocalRect();
	public native Rectangle getBounds();
/* 
	 * coordate transformations
	 * 
	 */

	public native Point localToScreen(int x, int y);
	public native Point screenToLocal(int x, int y);

	public native Rectangle localToScreen(int x, int y, int width, int height);
	public native Rectangle screenToLocal(int x, int y, int width, int height);

	public Point localToScreen(Point2D pt) {
		return localToScreen((int) pt.getX(), (int) pt.getY());
	}

	public Point screenToLocal(Point2D pt) {
		return screenToLocal((int) pt.getX(), (int) pt.getY());
	}

	public Rectangle localToScreen(Rectangle2D rt) {
		return localToScreen((int) rt.getX(), (int) rt.getY(), (int) rt.getWidth(), (int) rt.getHeight());
	}

	public Rectangle screenToLocal(Rectangle2D rt) {
		return screenToLocal((int) rt.getX(), (int) rt.getY(), (int) rt.getWidth(), (int) rt.getHeight());
	}

	/* 
	 * redraw requests
	 * 
	 */
		
	public native void invalidate();
	public native void invalidate(int x, int y, int width, int height);
	public native void update();

	public void invalidate(Rectangle2D rt) {
		invalidate((int) rt.getX(), (int) rt.getY(), (int) rt.getWidth(), (int) rt.getHeight());
	}

	/* 
	 * entry picture accessors
	 * 
	 */

	private native void nativeSetPicture(int iconRef);
	private native void nativeSetSelectedPicture(int iconRef);
	private native void nativeSetDisabledPicture(int iconRef);

	public Image getPicture() {
		return picture;
	}
		
	public void setPicture(Object obj) throws IOException {
		picture = Image.getImage(obj);
		nativeSetPicture(picture != null ? picture.createIconRef() : 0);
	}
	
	public Image getSelectedPicture() {
		return selectedPicture;
	}
	
	public void setSelectedPicture(Object obj) throws IOException {
		selectedPicture = Image.getImage(obj);
		nativeSetSelectedPicture(selectedPicture != null ? selectedPicture.createIconRef() : 0);
	}

	public Image getDisabledPicture() {
		return disabledPicture;
	}

	public void setDisabledPicture(Object obj) throws IOException {
		disabledPicture = Image.getImage(obj);
		nativeSetDisabledPicture(disabledPicture != null ? disabledPicture.createIconRef() : 0);
	}

	/* 
	 * entry text accessors
	 * 
	 */

	private native void nativeSetText(String text);
	
	public String getText() {
		return text;
	}
	
	public void setText(String text) {
		this.text = text;
		nativeSetText(text);
	}
	
	/*
	 *  entry timer accessors
	 *
	 */

	/*
	public native ADMTimerRef createTimer(ADMUt32 milliseconds,
				ADMActionMask abortMask, ADMEntryTimerProc timerProc,
				ADMEntryTimerAbortProc timerAbortProc, ADMt32 options);

	public native void abortTimer(ADMTimerRef timer);
	*/

	/* 
	 * entry checkmark accessors
	 * 
	 */

	/*
	public native void setCheckGlyph(ADMStandardCheckGlyphID checkGlyph);
	public native ADMStandardCheckGlyphID getCheckGlyph();
	*/

}
