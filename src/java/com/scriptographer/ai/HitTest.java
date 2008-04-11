/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Plugin for Adobe Illustrator.
 *
 * Copyright (c) 2002-2008 Juerg Lehni, http://www.scratchdisk.com.
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
 * File created on 11.01.2005.
 *
 * $Id$
 */

package com.scriptographer.ai;

/**
 * HitTest objects are returned by {@link Document#hitTest} and
 * {@link Path#hitTest}. They represent the result of a hit test, which is
 * reflected in the object's properties as described bellow.
 * 
 * @author lehni
 */
public class HitTest {
	// AIHitRequest
	
	/**
	 * Any object hits anywhere.
	 */
	public static final int TEST_ALL = 0;
		
	/**
	 * Only hits on bezier points.
	 */
	public static final int TEST_POINTS = 1;

	/**
	 * Only first or last bezier point hits on path.
	 */
	public static final int TEST_END_POINTS = 2;

	/**
	 * Only guide object hits.
	 */
	public static final int TEST_GUIDES = 3;

	/**
	 * Only hits on points on paths or any guides.
	 */
	public static final int TEST_PATHS = 4;

	/**
	 * Only hits on text objects.
	 */
	public static final int TEST_TEXTS = 5;

	/**
	 * Only hits already-selected objects.
	 */
	public static final int TEST_SELECTION = 6;

	/**
	 * Only hits paintable objects (non-guide paths & text)
	 */
	public static final int TEST_PAINTABLES = 7;

	/**
	 * Same as all but doesn't test against object fills
	 */
	public static final int TEST_ALL_EXCEPT_FILLS = 8;

	/**
	 * Same as paint but doesn't test against object fills
	 */
	public static final int TEST_PAINTABLES_EXCEPT_FILLS = 9;

	/**
	 * Same as all but doesn't test against direction line endpoints (the in and
	 * out handles of a bezier)
	 */
	public static final int TEST_ALL_EXCEPT_HANDLES = 10;

	/**
	 * Same as paint but no locked objects
	 */
	public static final int TEST_PAINTABLES_EXCEPT_LOCKED = 11;
	
	// AIHitType
	/**
	 * hit an anchor point
	 */
	public static final int HIT_ANCHOR = 0;

	/**
	 * hit a leading direction line endpoint
	 */
	public static final int HIT_HANDLE_IN = 1;
	
	/**
	 * hit a trailing direction line endpoint
	 */
	public static final int HIT_HANDLE_OUT = 2;
	
	/**
	 * hit a segment interior
	 */
	public static final int HIT_CURVE = 3;
	
	/**
	 * hit generated by infill during preview mode
	 */
	public static final int HIT_FILL = 4;
	
	/**
	 * hit the center point of an object with show center point on
	 */
	public static final int HIT_CENTER = 5;

	/**
	 * hit the intersection of two guides
	 */
	public static final int HIT_GUIDE_INTERSECTION = 6;
	
	// fake HIT values for Text, added from AITextPart + 10
	
	/**
	 * right on text
	 **/
	public static final int HIT_TEXT = 7;
	
	/**
	 * in port of either AreaText or PathText
	 **/
	public static final int HIT_TEXT_IN_PORT = 8;
	
	/**
	 * out port of either AreaText or PathText
	 **/
	public static final int HIT_TEXT_OUT_PORT = 9;
	
	/**
	 * start point of PathText
	 **/
	public static final int HIT_TEXT_START = 10;
	
	/**
	 * middle point of PathText
	 **/
	public static final int HIT_TEXT_MIDDLE	= 11;
	
	/**
	 * end point of PathText
	 **/
	public static final int HIT_TEXT_END = 12;

	/**
	 * 
	 */
	public static final float DEFAULT_TOLERANCE = 2.0f;
	
	private int type;
	private Curve curve;
	private Item item;
	private Point point;
	private float parameter;

	/**
	 * 
	 * @param type HitTest.HIT_*
	 * @param curve
	 * @param parameter
	 * @param point
	 */
	protected HitTest(int type, Curve curve, float parameter, Point point) {
		this.type = type;
		this.curve = curve;
		this.item = curve.getPath();
		this.parameter = parameter;
		this.point = point;
	}

	protected HitTest(Curve curve, float parameter) {
		// passing null for point only calls curve.getPoint(t) if the point is requested, see HitTest
		this(
			parameter > 0 && parameter < 1 ? HitTest.HIT_CURVE : HitTest.HIT_ANCHOR,
			curve,
			parameter,
			null
		);
	}
	
	/**
	 * To be called from the native environment
	 */
	protected HitTest(int type, Item item, int index, float parameter, Point point) {
		this.type = type;
		this.item = item;
		this.parameter = parameter;
		this.point = point;
		this.curve = null;
		if (item instanceof Path && type < HIT_FILL) {
			Path path = (Path) item;
			CurveList curves = path.getCurves();
			// calculate the curve index in the curve list according to the segment index:
			// curve = segment - 1, if curve < 0, curve += segmentCount
			index--;
			if (index < 0)
				index += curves.size();
			if (index < curves.size()) {
				this.curve = (Curve) curves.get(index);
				// if parameter == -1 and index is valid, we're hitting
				// a segment point. just set parameter to 0 and the
				// curve / parameter pair is valid
				if (parameter == -1)
					this.parameter = 0;
			}
		}
	}

	/**
	 * Returns the item which was hit.
	 * @return
	 */
	public Item getItem() {
		return item;
	}

	public Curve getCurve() {
		return curve;
	}
	
	public int getCurveIndex() {
		if (curve != null)
			return curve.getIndex();
		else
			return -1;
	}
	
	public float getParameter() {
		return parameter;
	}
	
	public Point getPoint() {
		if (point == null && curve != null)
			point = curve.getPoint(parameter);
		return point;
	}
	
	public int getType() {
		return type;
	}
	
	public String toString() {
		return " { type: " + this.type + ", item: " + item + ", index: " + getCurveIndex() + ", parameter: " + parameter + " }";
	}
}
