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

	void setPixel (int h, int v, Pixel pixel) const override;
	Pixel getPixel (int h, int v) const override;

	void copyHorizontal(int hfrom, int vfrom, unsigned int width, int hto, int vto) const override;
	void copyVertical(int hfrom, int vfrom, unsigned int height, int hto, int vto) const override;

	void fillColor(Pixel color) const override;

protected:

	mutable Pixel* m_Buffer;
};

#endif /* GFX_MEMORYCANVAS_H_ */
