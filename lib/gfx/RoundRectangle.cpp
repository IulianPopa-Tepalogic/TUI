/*
 * RoundRectangle.cpp
 *
 *  Created on: Jun 4, 2020
 *      Author: IulianPopa
 */

#include "gfx/RoundRectangle.h"

#include <algorithm>

using namespace std;

RoundRectangle::RoundRectangle(uint16_t h, uint16_t v, uint16_t width, uint16_t height, uint8_t radius)
	: Shape(),
	  m_Radius(radius)
{
	setHorizontal(h);
	setVertical(v);
	setWidth(width);
	setHeight(height);
}

float RoundRectangle::isPositionInActiveRegion(int16_t h, int16_t v)
{
	if ((h < m_HStart) || ((m_HStart + m_Width) <= h))
		return false;

	if ((v < m_VStart) || ((m_VStart + m_Height) <= v))
		return false;

	if (h <= m_HStart + m_Radius)
	{
		h = m_HStart + m_Radius - h;
		if (v <= m_VStart + m_Radius)
		{
			v = m_VStart + m_Radius - v;
			return (h * h + v * v) <= (m_Radius * m_Radius);
		}
		else if (m_VStart + (m_Height - 1) - m_Radius <= v)
		{
			v = v - (m_VStart + (m_Height - 1) - m_Radius);
			return (h * h + v * v) <= (m_Radius * m_Radius);
		}
	}
	else if (m_HStart + (m_Width - 1) - m_Radius <= h)
	{
		h = h - (m_HStart + (m_Width - 1) - m_Radius);
		if (v <= m_VStart + m_Radius)
		{
			v = m_VStart + m_Radius - v;
			return (h * h + v * v) <= (m_Radius * m_Radius);
		}
		else if (m_VStart + (m_Height - 1) - m_Radius <= v)
		{
			v = v - (m_VStart + (m_Height - 1) - m_Radius);
			return (h * h + v * v) <= (m_Radius * m_Radius);
		}
	}

	return true;
}

void RoundRectangle::drawContour(const DrawableCanvas& canvas, const PixelColorProvider& pixel, unsigned int borderWidth)
{
	canvas.drawHLine(m_HStart + m_Radius, m_VStart, m_Width - m_Radius * 2, borderWidth, pixel);
	canvas.drawHLine(m_HStart + m_Radius, m_VStart + m_Height - borderWidth, m_Width - m_Radius * 2, borderWidth, pixel);

	canvas.drawVLine(m_HStart, m_VStart + m_Radius, m_Height - m_Radius * 2, borderWidth, pixel);
	canvas.drawVLine(m_HStart + m_Width - borderWidth, m_VStart + m_Radius, m_Height - m_Radius * 2, borderWidth, pixel);

	canvas.drawSemiCircle(m_HStart + m_Radius, m_VStart + m_Radius, m_Radius, borderWidth, pixel, 180.0f, 270.0f);
	canvas.drawSemiCircle(m_HStart + m_Radius, m_VStart + m_Height - m_Radius - 1, m_Radius, borderWidth, pixel, 90.0f, 180.0f);

	canvas.drawSemiCircle(m_HStart + m_Width - m_Radius - 1, m_VStart + m_Radius, m_Radius, borderWidth, pixel, 270.0f, 360.0f);
	canvas.drawSemiCircle(m_HStart + m_Width - m_Radius - 1, m_VStart + m_Height - m_Radius - 1, m_Radius, borderWidth, pixel, 0.0f, 90.0f);
}


void RoundRectangle::fill(const DrawableCanvas& canvas, const PixelColorProvider& color)
{
	canvas.drawHLine(m_HStart + m_Radius, m_VStart, m_Width - m_Radius * 2, m_Height, color);
	canvas.drawVLine(m_HStart, m_VStart + m_Radius, m_Height - m_Radius * 2, m_Radius, color);
	canvas.drawVLine(m_HStart + m_Width - m_Radius, m_VStart + m_Radius, m_Height - m_Radius * 2, m_Radius, color);


	canvas.drawSemiCircle(m_HStart + m_Radius, m_VStart + m_Radius, m_Radius, m_Radius, color, 180.0f, 270.0f);
	canvas.drawSemiCircle(m_HStart + m_Radius, m_VStart + m_Height - m_Radius - 1, m_Radius, m_Radius, color, 90.0f, 180.0f);

	canvas.drawSemiCircle(m_HStart + m_Width - m_Radius - 1, m_VStart + m_Radius, m_Radius, m_Radius, color, 270.0f, 360.0f);
	canvas.drawSemiCircle(m_HStart + m_Width - m_Radius - 1, m_VStart + m_Height - m_Radius - 1, m_Radius, m_Radius, color, 0.0, 90.0);
}


void RoundRectangle::setHorizontal(uint16_t h)
{
	m_HStart = h;
}

void RoundRectangle::setVertical(uint16_t v)
{
	m_VStart = v;
}

void RoundRectangle::setWidth(uint16_t width)
{
	m_Width = width;
	m_Radius = min<uint16_t>(m_Radius, m_Width / 2);
}

void RoundRectangle::setHeight(uint16_t height)
{
	m_Height = height;
	m_Radius = min<uint16_t>(m_Radius, m_Height / 2);
}

void RoundRectangle::setRadius(uint8_t radius)
{
	m_Radius = min<uint16_t>(radius, min(m_Height / 2, m_Width / 2));
}
