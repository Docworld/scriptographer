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
 * File created on 03.01.2005.
 *
 * $RCSfile: RadioButton.java,v $
 * $Author: lehni $
 * $Revision: 1.1 $
 * $Date: 2005/02/23 22:00:59 $
 */

package com.scriptographer.adm;

import java.awt.geom.Rectangle2D;
import java.io.IOException;

public class RadioButton extends PictureItem {

	// ADMRadioButtonStyle
	public final static int
		STYLE_ONE_ALWAYS_SET = 0,
		STYLE_ALLOW_NONE_SET = 2;

	public RadioButton(Dialog dialog, Rectangle2D bounds, String text, int style) {
		super(dialog, Item.TYPE_TEXT_RADIOBUTTON, bounds, text, style);
	}
	
	public RadioButton(Dialog dialog, Rectangle2D bounds, String text) {
		this(dialog, bounds, text, STYLE_ONE_ALWAYS_SET);
	}
	
	public RadioButton(Dialog dialog, Rectangle2D bounds, Image image, int style) {
		super(dialog, Item.TYPE_PICTURE_RADIOBUTTON, bounds, null, style);
		try {
			setPicture(image);
		} catch (IOException e) {
			// will never happen with type Image
		}
	}
	
	public RadioButton(Dialog dialog, Rectangle2D bounds, Image image) {
		this(dialog, bounds, image, STYLE_ONE_ALWAYS_SET);
	}
	
	public Object getValue() {
		return new Boolean(getBooleanValue());
	}
	
	public void setValue(Object value) {
		setBooleanValue(value);
	}
}
