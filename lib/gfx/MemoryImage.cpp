/*
 * MemoryCanvas.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#include "gfx/MemoryImage.h"

using namespace std;

void MemoryImage::setPixel (int h, int v, Pixel pixel) const
{
	if ((v < 0) | (v >= m_Height) | (h < 0) | (h >= m_Width))
		return;

	m_Buffer[pixelPos(h, v)] = pixel;
}


Pixel MemoryImage::getPixel (int h, int v) const
{
	if ((v < 0) | (v >= m_Height) | (h < 0) | (h >= m_Width))
		return Pixel(0, 0, 0);

	return  m_Buffer[pixelPos(h, v)];
}

void MemoryImage::copyHorizontal(int hfrom, int vfrom, unsigned int width, int hto, int vto) const
{
	width = ((hfrom + width) <= m_Width) ? width : (m_Width - hfrom);
	width = ((hto + width) <= m_Width) ? width : (m_Width - hto);

	const unsigned int last = pixelPos(hto + width, vto);
	unsigned int idx_from = pixelPos(hfrom, vfrom);
	for (unsigned int idx_to = vto * m_Width + hto; idx_to < last; ++idx_to)
	{
		m_Buffer[idx_to] = m_Buffer[idx_from];
		idx_from++;
	}
}

void  MemoryImage::copyVertical(int hfrom, int vfrom, unsigned int height, int hto, int vto) const
{
	height = ((vfrom + height) <= m_Height) ? height : (m_Height - vfrom);
	height = ((vto + height) <= m_Height) ? height : (m_Height - vto);

	unsigned int idx_from = pixelPos(hfrom, vfrom);
	unsigned int idx_to = pixelPos(hto, vto);
	for (unsigned int i = 0; i < height; ++i)
	{
		m_Buffer[idx_to] = m_Buffer[idx_from];
		idx_from += m_Width, idx_to += m_Width;
	}
}

void MemoryImage::fillColor(Pixel color) const
{
	const unsigned int count = (unsigned int)m_Width * m_Height;
	for (unsigned int idx = 0; idx < count; ++idx)
		m_Buffer[idx] = color;
}


