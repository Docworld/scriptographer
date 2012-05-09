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
 * File created on 19.12.2004.
 */

package com.scriptographer.ai;

import java.awt.geom.Rectangle2D;

import com.scratchdisk.script.ArgumentReader;
import com.scratchdisk.script.ChangeEmitter;
import com.scratchdisk.script.ChangeReceiver;
import com.scriptographer.ScriptographerEngine;

/**
 * A Rectangle specifies an area that is enclosed by it's top-left point (x, y),
 * its width, and its height. It should not be confused with a rectangular path,
 * it is not an item.
 * 
 * @author lehni
 */
public class Rectangle implements ChangeEmitter, ChangeReceiver {
	protected double x;
	protected double y;
	protected double width;
	protected double height;
	
	public Rectangle() {
		x = y = width = height = 0;
	}

	/**
	 * Creates a rectangle object.
	 * 
	 * @param x the left coordinate {@default 0}
	 * @param y the top coordinate if in the top-down coordinate system, bottom
	 *        otherwise. See
	 *        {@link com.scriptographer.sg.Script#getCoordinateSystem() } for
	 *        more information. {@default 0}
	 * @param width {@default 0}
	 * @param height {@default 0}
	 */
	public Rectangle(double x, double y, double width, double height) {
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	}

	public Rectangle(float x, float y, float width, float height) {
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	}

	public Rectangle(Point point, Size size) {
		this(point.x, point.y, size.width, size.height);
	}

	/**
	 * Creates a rectangle object from the passed points. These do not
	 * necessarily need to be the top left and bottom right corners, the
	 * constructor figures out how to fit a rectangle between them.
	 * 
	 * @param point1 The first point defining the rectangle
	 * @param point2 The second point defining the rectangle
	 */
	public Rectangle(Point point1, Point point2) {
		this(point1.x, point1.y, point2.x - point1.x, point2.y - point1.y);
		if (width < 0) {
			x = point2.x;
			width = -width;
		}
		if (height < 0) {
			y = point2.y;
			height = -height;
		}
	}

	/**
	 * Creates a new rectangle object from the passed rectangle object.
	 * @param rt
	 */
	public Rectangle(Rectangle rt) {
		this(rt.x, rt.y, rt.width, rt.height);
	}

	/**
	 * @jshide
	 */
	public Rectangle(Rectangle2D rt) {
		this(rt.getX(), rt.getY(), rt.getWidth(), rt.getHeight());
	}

	/**
	 * @jshide
	 */
	public Rectangle(ArgumentReader reader) {
		this(reader.readDouble("x", 0),
				reader.readDouble("y", 0),
				reader.readDouble("width", 0),
				reader.readDouble("height", 0));
	}

	/**
	 * Changes the boundary properties of the rectangle.
	 * 
	 * @jshide
	 */
	public void set(double x, double y, double width, double height) {
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	}

	/**
	 * The x position of the rectangle.
	 */
	public double getX() {
		return x;
	}

	public void setX(double x) {
		this.x = x;
	}

	/**
	 * The y position of the rectangle. This is the top coordinate if in the
	 * top-down coordinate system, bottom otherwise. See
	 * {@link com.scriptographer.sg.Script#getCoordinateSystem() } for more
	 * information.
	 */
	public double getY() {
		return y;
	}

	public void setY(double y) {
		this.y = y;
	}

	/**
	 * The width of the rectangle.
	 */
	public double getWidth() {
		return width;
	}
	
	public void setWidth(double width) {
		this.width = width;
	}

	/**
	 * The height of the rectangle.
	 */
	public double getHeight() {
		return height;
	}
	
	public void setHeight(double height) {
		this.height = height;
	}

	public Point getPoint() {
		return new Point(x, y);
	}

	public void setPoint(Point point) {
		this.x = point.x;
		this.y = point.y;
	}

	/**
	 * @jshide
	 */
	public void setPoint(double x, double y) {
		this.x = x;
		this.y = y;
	}

	
	public Size getSize() {
		return new Size(width, height);
	}

	public void setSize(Size size) {
		this.width = size.width;
		this.height = size.height;
	}

	/**
	 * @jshide
	 */
	public void setSize(double width, double height) {
		this.width = width;
		this.height = height;
	}

	/**
	 * {@grouptitle Side Positions}
	 * 
	 * The position of the left hand side of the rectangle. Note that this
	 * doesn't move the whole rectangle; the right hand side stays where it was.
	 */
	public double getLeft() {
		return x;
	}

	public void setLeft(double left) {
		// right should not move
		width -= left - x;
		x = left;
	}

	/**
	 * The top coordinate of the rectangle. Note that this doesn't move the
	 * whole rectangle: the bottom won't move.
	 */
	public double getTop() {
		if (ScriptographerEngine.topDownCoordinates) {
			return y;
		} else {
			return y + height;
		}
	}
	
	public void setTop(double top) {
		if (ScriptographerEngine.topDownCoordinates) {
			// bottom should not move
			height -= top - y;
			y = top;
		} else {
			height = top - y;
		}
	}

	/**
	 * The position of the right hand side of the rectangle. Note that this
	 * doesn't move the whole rectangle; the left hand side stays where it was.
	 */
	public double getRight() {
		return x + width;
	}

	public void setRight(double right) {
		width = right - x;
	}

	/**
	 * The bottom coordinate of the rectangle. Note that this doesn't move the
	 * whole rectangle: the top won't move.
	 */
	public double getBottom() {
		if (ScriptographerEngine.topDownCoordinates) {
			return y + height;
		} else {
			return y;
		}
	}
	
	public void setBottom(double bottom) {
		if (ScriptographerEngine.topDownCoordinates) {
			height = bottom - y;
		} else {
			// top should not move
			height -= bottom - y;
			y = bottom;
		}
	}

	private double getCenterX() {
		return x + width * 0.5f;
	}
	
	private double getCenterY() {
		return y + height * 0.5f;
	}

	private void setCenterX(double x) {
		this.x = x - width * 0.5f;
	}

	private void setCenterY(double y) {
		this.y = y - height * 0.5f;
	}
	
	/**
	 * {@grouptitle Corner and Center Point Positions}
	 * 
	 * The center point of the rectangle.
	 */
	public Point getCenter() {
		return new Point(getCenterX(), getCenterY());
	}

	public void setCenter(Point center) {
		setCenterX(center.x);
		setCenterY(center.y);
	}

	/**
	 * The top left point of the rectangle.
	 */
	public Point getTopLeft() {
		return new Point(getLeft(), getTop());
	}

	public void setTopLeft(Point pt) {
		setLeft(pt.x);
		setTop(pt.y);
	}

	/**
	 * The top right point of the rectangle.
	 */
	public Point getTopRight() {
		return new Point(getRight(), getTop());
	}

	public void setTopRight(Point pt) {
		setRight(pt.x);
		setTop(pt.y);
	}

	/**
	 * The bottom left point of the rectangle.
	 */
	public Point getBottomLeft() {
		return new Point(getLeft(), getBottom());
	}

	public void setBottomLeft(Point pt) {
		setLeft(pt.x);
		setBottom(pt.y);
	}

	/**
	 * The bottom right point of the rectangle.
	 */
	public Point getBottomRight() {
		return new Point(getRight(), getBottom());
	}

	public void setBottomRight(Point pt) {
		setRight(pt.x);
		setBottom(pt.y);
	}

	public Point getLeftCenter() {
		return new Point(getLeft(), getCenterY());
	}
	
	public void setLeftCenter(Point pt) {
		setLeft(pt.x);
		setCenterY(pt.y);
	}

	public Point getTopCenter() {
		return new Point(getCenterX(), getTop());
	}
	
	public void setTopCenter(Point pt) {
		setCenterX(pt.x);
		setTop(pt.y);
	}

	public Point getRightCenter() {
		return new Point(getRight(), getCenterY());
	}
	
	public void setRightCenter(Point pt) {
		setRight(pt.x);
		setCenterY(pt.y);
	}

	public Point getBottomCenter() {
		return new Point(getCenterX(), getBottom());
	}
	
	public void setBottomCenter(Point pt) {
		setCenterX(pt.x);
		setBottom(pt.y);
	}

	/**
	 * Clones the rectangle.
	 */
	public Object clone() {
		return new Rectangle(this);
	}

	public boolean equals(Object object) {
		if (object instanceof Rectangle) {
			Rectangle rt = (Rectangle) object;
			return rt.x == x && rt.y == y &&
					rt.width == width && rt.height == height;
		}
		// TODO: support other rect types?
		return false;
	}

	/**
	 * Returns {@true if the rectangle is empty}
	 */
	public boolean isEmpty() {
		return width == 0 || height == 0;
	}

	/**
	 * {@grouptitle Geometric Tests}
	 * 
	 * Tests if the specified point is inside the boundary of the rectangle.
	 * 
	 * @param point the specified point
	 * @return {@true if the point is inside the rectangle's
	 *         boundary}
	 */
	public boolean contains(Point point) {
		return contains(point.x, point.y);
	}

	/**
	 * Tests if specified coordinates are inside the boundary of the rectangle.
	 * 
	 * @param x, y the coordinates to test
	 * @return {@true if the specified coordinates are inside the
	 *         boundary of the rectangle}
	 * 
	 * @jshide
	 */
	public boolean contains(double x, double y) {
		return x >= this.x  && y >= this.y
				&& x <= this.x + width && y <= this.y + height;
	}
	
	/**
	 * Tests if the interior of the rectangle entirely contains the specified
	 * rectangle.
	 * 
	 * @param rect The specified rectangle
	 * @return {@true if the rectangle entirely contains the
	 *         specified rectangle}
	 */
	public boolean contains(Rectangle rect) {
		return rect.x >= x && rect.y >= y
				&& rect.x + rect.width <= x + width
				&& rect.y + rect.height <= y + height;
	}

	/**
	 * Tests if the interior of this rectangle intersects the interior of
	 * another rectangle. Rectangles just touching each other are considered as
	 * non-intersecting.
	 * 
	 * @param rect the specified rectangle
	 * @return {@true if the rectangle and the specified rectangle intersect
	 *         each other}
	 */
	public boolean intersects(Rectangle rect) {
		return rect.x + rect.width > this.x
				&& rect.y + rect.height > this.y
				&& rect.x < this.x + this.width
				&& rect.y < this.y + this.height;
	}

	/**
	 * {@grouptitle Boolean Operations}
	 * 
	 * Returns a new rectangle representing the intersection of this rectangle
	 * with the specified rectangle.
	 * 
	 * @param rect The rectangle to be intersected with this rectangle
	 * @return The largest rectangle contained in both the specified rectangle
	 *         and in this rectangle.
	 */
	public Rectangle intersect(Rectangle rect) {
		double x1 = Math.max(x, rect.x);
		double y1 = Math.max(y, rect.y);
		double x2 = Math.min(x + width, rect.x + rect.width);
		double y2 = Math.min(y + height, rect.y + rect.height);
		return new Rectangle(x1, y1, x2 - x1, y2 - y1);
	}

	/**
	 * Returns a new rectangle representing the union of this rectangle with the
	 * specified rectangle.
	 * 
	 * @param rect the rectangle to be combined with this rectangle
	 * @return the smallest rectangle containing both the specified rectangle
	 *         and this rectangle.
	 */
	public Rectangle unite(Rectangle rect) {
		double x1 = Math.min(x, rect.x);
		double y1 = Math.min(y, rect.y);
		double x2 = Math.max(x + width, rect.x + rect.width);
		double y2 = Math.max(y + height, rect.y + rect.height);
		return new Rectangle(x1, y1, x2 - x1, y2 - y1);
	}

	/**
	 * Adds a point, specified by the arguments {@code x}
	 * and {@code y}, to the rectangle. The resulting rectangle is the
	 * smallest rectangle that contains both the original rectangle and the
	 * specified point.
	 * 
	 * After adding a point, a call to {@code contains} with the added
	 * point as an argument does not necessarily return {@code true}.
	 * The {@code contains} method does not return {@code true}
	 * for points on the right or bottom edges of a rectangle. Therefore, if the
	 * added point falls on the left or bottom edge of the enlarged rectangle,
	 * {@code contains} returns {@code false} for that point.
	 * 
	 * @param px the x coordinate of the point
	 * @param py the y coordinate of the point
	 * 
	 * @jshide
	 */
	public Rectangle include(double px, double py) {
		double x1 = Math.min(x, px);
		double y1 = Math.min(y, py);
		double x2 = Math.max(x + width, px);
		double y2 = Math.max(y + height, py);
		return new Rectangle(x1, y1, x2 - x1, y2 - y1);
	}

	/**
	 * Adds a point to this rectangle. The resulting rectangle is the
	 * smallest rectangle that contains both the original rectangle and the
	 * specified point.
	 * 
	 * After adding a point, a call to {@code contains} with the added
	 * point as an argument does not necessarily return {@code true}.
	 * The {@code contains} method does not return {@code true}
	 * for points on the right or bottom edges of a rectangle. Therefore, if the
	 * added point falls on the left or bottom edge of the enlarged rectangle,
	 * {@code contains} returns {@code false} for that point.
	 * 
	 * @param point
	 */
	public Rectangle include(Point point) {
		return include(point.x, point.y);
	}

	/**
	 * @deprecated
	 */
	public Rectangle unite(double px, double py) {
		return include(px, py);
	}

	/**
	 * @deprecated
	 */
	public Rectangle unite(Point point) {
		return include(point);
	}

	protected Rectangle2D toRectangle2D() {
		return new Rectangle2D.Double(x, y, width, height);
	}

	public String toString() {
		StringBuffer buf = new StringBuffer(128);
		buf.append("{ x: ").append(ScriptographerEngine.numberFormat.format(x));
		buf.append(", y: ").append(ScriptographerEngine.numberFormat.format(y));
		buf.append(", width: ").append(
				ScriptographerEngine.numberFormat.format(width));
		buf.append(", height: ").append(
				ScriptographerEngine.numberFormat.format(height));
		buf.append(" }");
		return buf.toString();
	}
}
