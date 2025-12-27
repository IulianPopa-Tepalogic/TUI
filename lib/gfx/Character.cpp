/*
 * Character.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#include "gfx/Character.h"

using namespace std;

void Character::setPixel (int h, int v, Pixel pixel) const
{
}


Pixel Character::getPixel (int h, int v) const
{
	if ((h < 0) || (h >= m_Width) || ( v < 0) || (v >= m_Height))
		return Character::WHITE_PIXEL;

	const auto byteIdx = h / 8;
	const auto bitIndex = (7 - h % 8);

	const auto b = m_Bitmap[v * m_WidthBytes + byteIdx];
	return (b & (1 << bitIndex)) != 0 ? BLACK_PIXEL : WHITE_PIXEL;
}


const Pixel Character::BLACK_PIXEL = Pixel (0xFF, 0xFF, 0xFF);
const Pixel Character::WHITE_PIXEL = Pixel (0x00, 0x00, 0x00);
