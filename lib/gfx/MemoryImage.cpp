/*
 * MemoryCanvas.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#include "gfx/MemoryImage.h"

using namespace std;

bool MemoryImage::setPixel (int h, int v, Pixel pixel) const
{
	if ((v < 0) || (h < 0) || (v >= m_Height) || (h >= m_Width))
		return false;

	m_Buffer[pixelPos(h, v)] = pixel;
	return true;
}


Pixel MemoryImage::getPixel (int h, int v) const
{
	return  m_Buffer[pixelPos(h, v)];
}


void MemoryImage::drawHLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& pixel) const
{
	if (hfrom < 0)
	{
		const unsigned int off = -hfrom;
		if (length <= off)
			return ;

		length -= off;
		hfrom = 0;
	}

	length = ((hfrom + length) <= m_Width) ? length : (m_Width - hfrom);
	width = ((vfrom + width) <= m_Height) ? width : (m_Height - vfrom);

	for (unsigned int lineIdx = 0; lineIdx < width; lineIdx++)
		for (unsigned int i = 0;  i < length; ++i)
			m_Buffer[pixelPos(hfrom + i, vfrom + lineIdx)] =  pixel.get(hfrom + i, vfrom + lineIdx);
}

void MemoryImage::drawHLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if (dashFill == 0)
		return ;

	else if (dashSkip == 0)
	{
		drawHLine(hfrom, vfrom, length, width, pixel);
		return ;
	}
	else if (hfrom < 0)
	{
		const unsigned int off = -hfrom;
		if (length <= off)
			return ;

		length -= off;
		hfrom = 0;
	}

	auto fill = dashFill;
	length = ((hfrom + length) <= m_Width) ? length : (m_Width - hfrom);
	width = ((vfrom + width) <= m_Width) ? width : (m_Width - vfrom);

	for (unsigned int lineIdx = 0; lineIdx < width; lineIdx++)
		for (unsigned int i = 0;  i < length; ++i)
		{
			if (fill == 0)
			{
				i += dashSkip - 1;
				fill = dashFill;
				continue;
			}
			else
				fill--;

			m_Buffer[pixelPos(hfrom + i, vfrom + lineIdx)] =  pixel.get(hfrom + i, vfrom + lineIdx);
		}
}

void MemoryImage::drawVLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& pixel) const
{
	if (vfrom < 0)
	{
		const unsigned int off = -vfrom;
		if (length <= off)
			return ;

		length -= off;
		vfrom = 0;
	}

	length = ((vfrom + length) <= m_Height) ? length : (m_Height - hfrom);
	width = ((hfrom + width) <= m_Width) ? width : (m_Width - hfrom);

	for (unsigned int lineIdx = 0; lineIdx < width; ++lineIdx)
		for (unsigned int i = 0;  i < length; ++i)
			m_Buffer[pixelPos(hfrom + lineIdx, vfrom + i)] =  pixel.get(hfrom + lineIdx, vfrom + i);
}

void MemoryImage::drawVLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if (dashFill == 0)
		return ;
	else if (dashSkip == 0)
	{
		drawVLine(hfrom, vfrom, length, width, pixel);
		return ;
	}
	else if (vfrom < 0)
	{
		const unsigned int off = -vfrom;
		if (length <= off)
			return ;

		length -= off;
		vfrom = 0;
	}

	auto fill = dashFill;
	length = ((vfrom + length) <= m_Height) ? length : (m_Height - hfrom);
	width = ((hfrom + width) <= m_Width) ? width : (m_Width - hfrom);
	for (unsigned int lineIdx = 0; lineIdx < width; ++lineIdx)
	for (unsigned int i = 0;  i < length; ++i)
	{
		if (fill == 0)
		{
			i += dashSkip - 1;
			fill = dashFill;
			continue;
		}
		else
			fill--;

		m_Buffer[pixelPos(hfrom + lineIdx, vfrom + i)] =  pixel.get(hfrom + lineIdx, vfrom + i);
	}
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


