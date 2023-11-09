/*
 * MemoryCanvas.h
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#ifndef GFX_MEMORYCANVAS_H_
#define GFX_MEMORYCANVAS_H_

#include "gfx/DrawableCanvas.h"

class MemoryImage: public DrawableCanvas {
public:
	MemoryImage(unsigned int width, unsigned int height, Pixel* buffer)
	: DrawableCanvas (width, height),
	  m_Buffer(buffer)
	{}

	virtual ~MemoryImage() = default;

	bool setPixel (int h, int v, Pixel pixel) const override;
	Pixel getPixel (int h, int v) const override;

	void drawHLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& provider) const override;
	void drawHLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& provider, uint8_t dashFill, uint8_t dashSkip) const override;
	void drawVLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& provider) const override;
	void drawVLine (int hfrom, int vfrom, unsigned int length, unsigned int width, const PixelProvider& provider, uint8_t dashFill, uint8_t dashSkip) const override;

	void copyHorizontal(int hfrom, int vfrom, unsigned int width, int hto, int vto) const override;
	void copyVertical(int hfrom, int vfrom, unsigned int height, int hto, int vto) const override;

	void fillColor(Pixel color) const override;

protected:

	mutable Pixel* m_Buffer;
};

#endif /* GFX_MEMORYCANVAS_H_ */
