/*
 * SemiCircle.cpp
 *
 *  Created on: May 25, 2020
 *      Author: IulianPopa
 */

#include "gfx/SemiCircle.h"

#include <algorithm>

#include "../MathCalcs.h"

using namespace std;


SemiCircle::SemiCircle(int16_t hcenter, int16_t vcenter, uint16_t radius, float fromDegCW, float endDegCW)
	: Shape()
{
	setHorizontal(hcenter);
	setVertical(vcenter);
	setRadius(radius);
	setFromAngleCW(fromDegCW);
	setToAngleCW(endDegCW);
}


void SemiCircle::setHorizontal(int16_t hcenter)
{
	m_HCenter = hcenter;
	updateShapeInternals();
}


void SemiCircle::setVertical(int16_t vcenter)
{
	m_VCenter = vcenter;
	updateShapeInternals();
}

void SemiCircle::setRadius(uint16_t radius)
{
	m_Radius = radius;
	updateShapeInternals();
}

void SemiCircle::setFromAngleCW(float start)
{
	m_AngleStart = MathCalcs::normalizeDeg(start);
}

void SemiCircle::setToAngleCW(float end)
{
	m_AngleStop = MathCalcs::normalizeDeg(end);
}


void SemiCircle::updateShapeInternals()
{
	m_HStart = m_HCenter - m_Radius;
	m_VStart = m_VCenter - m_Radius;
	m_Width = m_Height = m_Radius * 2;
}

float SemiCircle::isPositionInActiveRegion(int16_t h, int16_t v)
{
	const float ht = h - m_HCenter;
	const float vt = (v - m_VCenter) != .0f ? (v - m_VCenter) : .00001f;

	if ((m_Radius * m_Radius) < (vt * vt + ht * ht))
		return false;

	float angle = MathCalcs::rad2deg(MathCalcs::atanRad(vt / ht));

	if (.0f <= angle)
		angle += (vt < .0f) ? 180.0f : .0f;
	else
		angle -= (.0f < vt) ? 180.0f : .0f;

	angle = MathCalcs::normalizeDeg(angle);

	if (m_AngleStart < m_AngleStop)
		return (m_AngleStart <= angle) && (angle <= m_AngleStop);

	return (m_AngleStart <= angle) || (angle <= m_AngleStop);
}

void SemiCircle::drawContour(const DrawableCanvas& canvas, const PixelColorProvider& pixel, unsigned int borderWidth)
{
		canvas.drawSemiCircle(m_HCenter, m_VCenter, m_Radius, borderWidth, pixel, m_AngleStart, m_AngleStop);
}
