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
 * File created on Oct 16, 2006.
 */

package com.scriptographer.ai;

import java.io.File;
import java.io.IOException;
import java.net.URL;

import com.scratchdisk.util.NetUtils;

/**
 * A PlacedFile represents a file which has been placed in an Illustrator
 * document.
 * 
 * @author lehni
 */
public class PlacedFile extends Item {

	protected PlacedFile(int handle, int docHandle, boolean created) {
		super(handle, docHandle, created);
	}

	private static native int nativeCreate(File file);

	/**
	 * Creates a PlacedFile item from a URL.
	 * This blocks until the file is loaded or an error occured.
	 * 
	 * Sample code:
	 * <code>
	 * var url = new java.net.URL('http://www.server.com/image.jpg');
	 * var raster = new PlacedFile(url);
	 * </code>
	 * 
	 * @param url the URL of the file to place
	 * @throws IOException
	 */
	public PlacedFile(URL url) throws IOException {
		this(NetUtils.loadFile(url, "sg_"));
	}


	/**
	 * Creates a PlacedFile from a local file.
	 * 
	 * Sample code:
	 * <code>
	 * var file = new java.io.File('/folder/image.jpg');
	 * var raster = new PlacedFile(file);
	 * </code>
	 * 
	 * @param file the file to be placed
	 */
	public PlacedFile(File file) {
		super(nativeCreate(file));
	}

	/**
	 * The original size of the placed file, ignoring any transformation
	 * matrices applied to it in the document.
	 */
	public native Size getSize();

	/**
	 * The original width of the placed file.
	 */
	public int getWidth() {
		return (int) getSize().width;
	}

	/**
	 * The original height of the placed file.
	 */
	public int getHeight() {
		return (int) getSize().height;
	}
	
	public native Matrix getMatrix();
	
	public native void setMatrix(Matrix matrix);
	
	/**
	 * The file associated with the placed file item.
	 */
	public native File getFile();
	
	/**
	 * Checks whether the placed file item is an eps.
	 * @return {@true if the placed file item is an eps}
	 */
	public native boolean isEps();
	
	/**
	 * Traces the placed file.
	 * @return the newly created tracing item
	 */
	public Tracing trace() {
		return new Tracing(this);
	}
	
	/**
	 * Embeds the placed file within the document.
	 * @param askParams specifies whether to show dialog for user input if needed {@default false}
	 * @return the embedded item
	 */
	public native Item embed(boolean askParams);
	
	public Item embed() {
		return embed(false);
	}
	
	// TODO:
	/*
	Specify the placement options for the object. These options are used when the object
	is scaled or replaced by a new object.
	AIAPI AIErr (*SetPlaceOptions) ( AIArtHandle placed, enum PlaceMethod method, enum PlaceAlignment alignment, ASBoolean clip );
	Get the placement options for the object. These options are used when the object
	is scaled or replaced by a new object.
	AIAPI AIErr (*GetPlaceOptions) ( AIArtHandle placed, enum PlaceMethod *method, enum PlaceAlignment *alignment, ASBoolean *clip );
	AIAPI AIErr (*GetPlacedDimensions) ( AIArtHandle placed, ASRealPoint *size, ASRealRect *viewBounds, AIRealMatrix *viewMatrix,
			ASRealRect *imageBounds, AIRealMatrix *imageMatrix );
	 */
}
