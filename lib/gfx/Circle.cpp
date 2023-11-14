/*
 * Circle.cpp
 *
 *  Created on: May 25, 2020
 *      Author: IulianPopa
 */

#include "gfx/Circle.h"

#include <algorithm>

#include "../MathCalcs.h"

using namespace std;


Circle::Circle(int16_t hcenter, int16_t vcenter, uint16_t radius)
	: Shape()
{
	setHorizontal(hcenter);
	setVertical(vcenter);
	setRadius(radius);
}


void Circle::setHorizontal(int16_t hcenter)
{
	m_HCenter = hcenter;
	updateShapeInternals();
}


void Circle::setVertical(int16_t vcenter)
{
	m_VCenter = vcenter;
	updateShapeInternals();
}

void Circle::setRadius(uint16_t radius)
{
	m_Radius = radius;
	updateShapeInternals();
}


void Circle::updateShapeInternals()
{
	m_HStart = m_HCenter - m_Radius;
	m_VStart = m_VCenter - m_Radius;
	m_Width = m_Height = m_Radius * 2;
}

float Circle::isPositionInActiveRegion(int16_t h, int16_t v)
{
	const int ht = h - m_HCenter;
	const int vt = (v - m_VCenter);

	auto result = (vt * vt + ht * ht) - (m_Radius * m_Radius);

	if (result <= -1.f)
		return 1.0f;

	else if (result > 1.0f)
		return .0f;

	else if (result >= .0f)
		return 1.0 - result;

	return result;
}

void Circle::drawContour(const DrawableCanvas& canvas, const PixelColorProvider& pixel, unsigned int borderWidth)
{
	canvas.drawSemiCircle(m_HCenter, m_VCenter, m_Radius, borderWidth, pixel, .0f, 360.0f);
}
