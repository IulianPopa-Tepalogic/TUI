/*
 * Rectangle.cpp
 *
 *  Created on: May 23, 2020
 *      Author: IulianPopa
 */

#include "gfx/Rectangle.h"

Rectangle::Rectangle(uint16_t h, uint16_t v, uint16_t width, uint16_t height)
	: Shape()

{
	setHorizontal(h);
	setVertical(v);
	setWidth(width);
	setHeight(height);
}


float Rectangle::isPositionInActiveRegion(int16_t h, int16_t v)
{
	if ( h < 0 || m_Width <= h)
		return .0f;

	if ( v < 0 || m_Height <= v)
		return .0f;

	return 1.0f;
}

void Rectangle::drawContour(const DrawableCanvas& canvas, const PixelColorProvider& pixel, unsigned int borderWidth)
{
	if (borderWidth <= 0)
		return;

	canvas.drawHLine(m_HStart, m_VStart, m_Width, borderWidth, pixel);
	canvas.drawHLine(m_HStart, m_VStart + m_Height - borderWidth, m_Width, borderWidth, pixel);

	canvas.drawVLine(m_HStart, m_VStart, m_Height, borderWidth, pixel);
	canvas.drawVLine(m_HStart + m_Width - borderWidth, m_VStart, m_Height, borderWidth, pixel);
}

void Rectangle::setHorizontal(uint16_t h)
{
	m_HStart = h;
}

void Rectangle::setVertical(uint16_t v)
{
	m_VStart = v;
}

void Rectangle::setWidth(uint16_t width)
{
	m_Width = width;
}

void Rectangle::setHeight(uint16_t height)
{
	m_Height = height;
}


void Rectangle::fill(const DrawableCanvas& canvas, const PixelColorProvider& color)
{
	canvas.drawHLine(m_HStart, m_VStart, m_Width, m_Height, color);

}
