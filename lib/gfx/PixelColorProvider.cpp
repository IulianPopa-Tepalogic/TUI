/*
 * PixelProvider.cpp
 *
 *  Created on: Sep 5, 2020
 *      Author: IulianPopa
 */

#include <gfx/PixelColorProvider.h>
#include <algorithm>
#include <math.h>

#include "gfx/Shape.h"

using namespace std;

Pixel VericalGradient::get(int h, int v) const
{
	const int start = m_Shape->vertical() + m_StartOffset;
	if (v <= start)
		return m_Gradient.m_From;

	const int width = max<int>(m_Width > 0 ? m_Width : m_Shape->height(), 1);
	if (v >= m_Shape->vertical() + width)
		return m_Gradient.m_To;

	return m_Gradient.Interpolate(v - start, width);
}

Pixel HorizontalGradient::get(int h, int v) const
{
	const int start = m_Shape->horizontal() + m_StartOffset;

	if (h <= start)
		return m_Gradient.m_From;

	const int width = max<int>(m_Width > 0 ? m_Width : m_Shape->width(), 1);
	if (h >= m_Shape->horizontal() + width)
		return m_Gradient.m_To;

	return m_Gradient.Interpolate(h - start,  width);
}


Pixel RadiantGradient::get(int h, int v) const
{
	const int centh = (m_Shape->horizontal() + m_Shape->width() / 2);
	const int centv = (m_Shape->vertical() + m_Shape->height() / 2);

	h -= centh;
	v -= centv;

	const unsigned int h2 = h * h;
	const unsigned int v2 = v * v;

	if (h2 + v2 <= m_RadiusOffset * m_RadiusOffset)
		return m_Gradient.m_From;

	const int radius = max<int>(m_Shape->height(), max<int>(m_Shape->width(), 2)) / 2;
	if (h2 + v2 >= radius * radius)
		return m_Gradient.m_To;

	//try not to use sqrt!
	return m_Gradient.Interpolate(sqrt((float)(h2 + v2)),  radius);
}


Pixel CanvasBackground::get(int h, int v) const
{
	int newH = max<int>(h - m_Shape->horizontal(), 0);
	int newV = max<int>(v - m_Shape->vertical(), 0);

	if (m_Stretch)
	{
		const float horFact = (float)m_Shape->width() / (float)m_Pattern.m_Width;
		const float hfact = (float)newH / horFact;
		const int hfactFloor = hfact;
		const float verFact = (float)m_Shape->height() / (float)m_Pattern.m_Height;
		const float vfact = (float)newV / verFact;
		const int vfactFloor = vfact;

		const Pixel p1 = Canvas::combinePixels(m_Pattern.getPixel(hfactFloor, vfactFloor), m_Pattern.getPixel(hfactFloor + 1, vfactFloor), hfact - hfactFloor);
		const Pixel p2 = Canvas::combinePixels(m_Pattern.getPixel(hfactFloor, vfactFloor + 1), m_Pattern.getPixel(hfactFloor + 1, vfactFloor + 1), hfact - hfactFloor);

		return Canvas::combinePixels(p1, p2, vfact - vfactFloor);
	}

	if (m_Pattern.m_Width <= newH)
		newH = m_RepeatedHorizontally ? (newH % m_Pattern.m_Width) : m_Pattern.m_Width - 1;

	if (m_Pattern.m_Height <= newV)
		newV = m_RepeatedVertically ? (newV % m_Pattern.m_Height) : m_Pattern.m_Height - 1;

	return m_Pattern.getPixel(newH, newV);
}




