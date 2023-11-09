/*
 * Shape.cpp
 *
 *  Created on: May 25, 2020
 *      Author: IulianPopa
 */

#include "gfx/Shape.h"

#include <algorithm>


using namespace std;


void Shape::fill(const DrawableCanvas& canvas, const PixelProvider& pixel)
{
	const int hto = m_HStart + m_Width;
	const int vto = m_VStart + m_Height;

	for (int h = max<int>(m_HStart, 0); h < hto; ++h)
	{
		for (int v = max<int>(m_VStart, 0); v < vto; ++v)
		{
			if (isPositionInActiveRegion(h, v))
				canvas.setPixel(h, v, pixel);
		}
	}
}

#if 0
void Shape::fill(const DrawableCanvas& canvas, const ColorGradient& gradient, bool vertical)
{
	const auto hto = max<int>(0, m_HStart + m_Width);
	const auto vto = max<int>(0, m_VStart + m_Height);

	if(vertical)
	{
		for (int h = max<int>(0, m_HStart); h < hto; ++h)
		{
			for (int v = max<int>(0, m_VStart); v < vto; ++v)
			{
				if (isPositionInActiveRegion(h, v))
					canvas.setPixel(h, v, gradient.Interpolate(h - m_HStart, hto - m_HStart));
			}
		}

	}
	else
	{
		for (int v = max<int>(0, m_VStart); v < vto; ++v)
		{
			for (int h = max<int>(0, m_HStart); h < hto; ++h)
			{
				if (isPositionInActiveRegion(h, v))
					canvas.setPixel(h, v, gradient.Interpolate(v - m_VStart, vto - m_VStart));
			}
		}
	}
}

void Shape::fill(const DrawableCanvas& canvas, const Canvas& pattern)
{
	const int hto = m_HStart + m_Width;
	const int vto = m_VStart + m_Height;

	for (int h = max<int>(m_HStart, 0), hp = 0; h < hto; ++h, hp = ((hp + 1) % pattern.m_Height))
	{
		for (int v = max<int>(m_VStart, 0), vp = 0; v < vto; ++v, vp = ((vp + 1) % pattern.m_Width))
		{
			if (isPositionInActiveRegion(h, v))
				canvas.setPixel(h, v, pattern.getPixel(hp, vp));
		}
	}
}
#endif



