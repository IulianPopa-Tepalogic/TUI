/*
 * Character.h
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#ifndef GFX_CHARACTER_H_
#define GFX_CHARACTER_H_

#include "gfx/Canvas.h"
#include "gfx/Font.h"


class Character: public Canvas {
public:
	Character(const Font::Character& c)
		: Canvas(c.width, c.height),
		  m_Bitmap (c.data),
		  m_WidthBytes((c.width + 7)/ 8)
	{};
	virtual ~Character() = default;

	bool setPixel (int h, int v, Pixel pixel) const override;
	Pixel getPixel (int h, int v) const override;

	static const Pixel BLACK_PIXEL;
	static const Pixel WHITE_PIXEL;

protected:
	const uint8_t* const m_Bitmap;
	const uint8_t m_WidthBytes;
};

#endif /* GFX_CHARACTER_H_ */
