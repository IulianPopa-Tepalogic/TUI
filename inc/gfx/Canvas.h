/*
 * Canvas.h
 *
 *  Created on: May 21, 2020
 *      Author: IulianPopa
 */

#pragma once

#include <gfx/PixelColorProvider.h>
#include "tui_gui_base.h"


class Canvas
{
public:
	explicit Canvas(unsigned int width, unsigned int height)
		: m_Width(width)
		, m_Height(height)
	{}
	virtual ~Canvas() = default;

	virtual void setPixel (int h, int v, Pixel pixel) const = 0;
	virtual Pixel getPixel (int h, int v) const = 0;

	void setPixel (int h, int v, Pixel pixel, float luminescence) const;
	void setPixel (int h, int v, const PixelColorProvider& provider) const { return setPixel (h, v, provider.get(h, v)); }
	void setPixel (int h, int v, const PixelColorProvider& provider, float luminescence) const { return setPixel(h, v, provider.get(h, v), luminescence); }


	const uint16_t m_Width;
	const uint16_t m_Height;

	static Pixel maskPixel(const Pixel fgColor, const Pixel bkColor, const float factor);
	static Pixel combinePixels(const Pixel p1, const Pixel p2, const float frac);
};
