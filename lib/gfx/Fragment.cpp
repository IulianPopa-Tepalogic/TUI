/*
 * Fragment.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#include "gfx/Fragment.h"


Fragment::Fragment(int hfrom, int vfrom, int width, int height, DrawableCanvas* parent)
	: DrawableCanvas(width, height)
{
	if (parent != nullptr)
		UpdateCanvas(*parent);

	UpdateArea(hfrom, vfrom, width, height);
}

bool Fragment::setPixel (int h, int v, Pixel pixel) const
{
	if ((v < 0 ) || (h < 0) || (v >= m_Height) || (h >= m_Width) || (m_Canvas == nullptr))
		return false;

	return m_Canvas->setPixel(h + m_HFrom, v + m_VFrom, pixel);
}

Pixel Fragment::getPixel (int h, int v) const
{
	if (m_Canvas == nullptr)
		return Pixel(0, 0, 0);

	return m_Canvas->getPixel(h + m_HFrom, v + m_VFrom);
}

void Fragment::UpdateCanvas(DrawableCanvas& canvas)
{
	m_Canvas = &canvas;
	UpdateArea(m_HFrom, m_VFrom, m_Width, m_Height);
}

void Fragment::UpdateArea(int hfrom, int vfrom, int width, int height)
{
	if (m_Canvas == nullptr)
	{
		m_HFrom = hfrom;
		m_VFrom = vfrom;
		const_cast<uint16_t&>(m_Width) = width;
		const_cast<uint16_t&>(m_Height) = height;
	}

	if ((m_HFrom >= m_Canvas->m_Width)
		|| (m_VFrom >= m_Canvas->m_Height))
	{
		m_HFrom = m_VFrom =  0;
		const_cast<uint16_t&>(m_Width) = 0;
		const_cast<uint16_t&>(m_Height) = 0;
		return ;
	}

	m_HFrom = hfrom;
	m_VFrom = vfrom;

	if (width <= 0)
		width = m_Canvas->m_Width - m_HFrom;

	if (height <= 0)
		height = m_Canvas->m_Height - m_VFrom;

	const_cast<uint16_t&>(m_Width) = (m_HFrom + width <= m_Canvas->m_Width) ? width : (m_Canvas->m_Width - m_HFrom);
	const_cast<uint16_t&>(m_Height) = (m_VFrom + height <= m_Canvas->m_Height) ? height : (m_Canvas->m_Height - m_VFrom);
}

bool Fragment::overlaps(const Fragment& other) const
{
	if (m_Canvas != other.m_Canvas)
		return false;

	if ((m_HFrom <= other.m_HFrom)
		&& (other.m_HFrom <= m_HFrom + m_Width)
		&& (m_VFrom <= other.m_VFrom)
		&& (other.m_VFrom <= m_VFrom + m_Height))
	{
		return true;
	}

	if ((other.m_HFrom <= m_HFrom)
		&& (m_HFrom <= other.m_HFrom + other.m_Width)
		&& (other.m_VFrom <= m_VFrom)
		&& (m_VFrom <= other.m_VFrom + other.m_Height))
	{
		return true;
	}

	return true;
}

void Fragment::startFrameDrawing()
{
	if (m_Canvas != nullptr)
		m_Canvas->startFrameDrawing();
}

void Fragment::endFrameDrawing()
{
	if (m_Canvas != nullptr)
		m_Canvas->endFrameDrawing();
}

