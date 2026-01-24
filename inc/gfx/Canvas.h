/*
 * Canvas.h
 *
 *  Created on: May 21, 2020
 *      Author: IulianPopa
 */

#pragma once

#include <gfx/PixelColorProvider.h>
#include "tui_gui_base.h"

#include <math.h>
#include <algorithm>

class Canvas
{
public:
	explicit Canvas(unsigned int width, unsigned int height)
		: m_Width(width)
		, m_Height(height)
	{}

	virtual ~Canvas() = default;

	virtual void setPixel (int h, int v, const Pixel& pixel) const = 0;
	virtual Pixel getPixel (int h, int v) const = 0;

	void setPixel (int h, int v, Pixel pixel, float luminescence) const
	{
		const Pixel bkg = getPixel(h, v);
		return setPixel(h, v, maskPixel(pixel, bkg, luminescence));
	}

	void setPixel (int h, int v, Pixel pixel, uint8_t luminescence) const
	{
		const Pixel bkg = getPixel(h, v);
		return setPixel(h, v, maskPixel(pixel, bkg, luminescence));
	}

	void setPixel (int h, int v, const PixelColorProvider& provider) const { return setPixel (h, v, provider.get(h, v)); }
	void setPixel (int h, int v, const PixelColorProvider& provider, float luminescence) const { return setPixel(h, v, provider.get(h, v), luminescence); }

	const uint16_t m_Width;
	const uint16_t m_Height;

	static Pixel maskPixel(const Pixel fgColor, const Pixel bkColor, const float factor) { return Canvas::combinePixels(bkColor, fgColor, factor); }
	static Pixel maskPixel(const Pixel fgColor, const Pixel bkColor, const uint8_t luminescence) { return Canvas::combinePixels(bkColor, fgColor, luminescence); }

	static Pixel combinePixels(const Pixel p1, const Pixel p2, const float frac)
	{
		const float red = (1.0f -  frac) * p1.red + frac * p2.red;
		const float green = (1.0f -  frac) * p1.green + frac * p2.green;
		const float blue = (1.0f -  frac) * p1.blue + frac * p2.blue;

		return Pixel(red, green, blue);
	}

	static Pixel combinePixels(const Pixel p1, const Pixel p2, uint8_t luminescence)
	{
		const uint8_t red = (uint8_t)(((255 - luminescence) * p1.red + luminescence * p2.red) / 255);
		const uint8_t green = (uint8_t)(((255 - luminescence) * p1.green + luminescence * p2.green) / 255);
		const uint8_t blue = (uint8_t)(((255 - luminescence) * p1.blue + luminescence * p2.blue) / 255);

		return Pixel(red, green, blue);
	}
};
