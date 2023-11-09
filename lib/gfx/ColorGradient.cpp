/*
 * ColorGradient.cpp
 *
 *  Created on: May 22, 2020
 *      Author: IulianPopa
 */

#include "gfx/ColorGradient.h"


Pixel ColorGradient::Interpolate(int offset, int wide) const
{
	const auto red = (int)m_From.red + (((int)m_To.red - (int)m_From.red) * offset) / wide;
	const auto green = (int)m_From.green + (((int)m_To.green - (int)m_From.green) * offset) / wide;
	const auto blue = (int)m_From.blue + (((int)m_To.blue - (int)m_From.blue) * offset) / wide;

	return Pixel(red, green, blue);
}


