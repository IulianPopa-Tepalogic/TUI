/*
 * Canvas.cpp
 *
 *  Created on: Aug 26, 2020
 *      Author: IulianPopa
 */

#include "gfx/Canvas.h"

#include <math.h>
#include <algorithm>

using namespace std;

bool Canvas::setPixel(int h, int v, Pixel pixel, float luminescence) const
{
	if (h < 0 || v < 0)
		return false;

	const Pixel bkg = getPixel(h, v);
	return setPixel(h, v, maskPixel(pixel, bkg, max(min(luminescence, 1.0f), .0f)));
}


Pixel Canvas::maskPixel(const Pixel fgColor, const Pixel bkColor, const float factor)
{
	static constexpr Pixel allWhite(0xFF, 0xFF, 0xFF);

	const float red = (1.0f - factor) * bkColor.red + factor * fgColor.red;
	const float green = (1.0f - factor) * bkColor.green + factor * fgColor.green;
	const float blue = (1.0f - factor) * bkColor.blue + factor * fgColor.blue;

	return Pixel(red, green, blue);
}


Pixel Canvas::combinePixels(const Pixel p1, const Pixel p2, const float frac)
{
	const float red = (1.0f -  frac) * p1.red + frac * p2.red;
	const float green = (1.0f -  frac) * p1.green + frac * p2.green;
	const float blue = (1.0f -  frac) * p1.blue + frac * p2.blue;

	return Pixel(red, green, blue);
}


