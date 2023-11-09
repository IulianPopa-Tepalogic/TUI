/*
 * RoundRectangle.h
 *
 *  Created on: Jun 4, 2020
 *      Author: IulianPopa
 */

#ifndef GFX_ROUNDRECTANGLE_H_
#define GFX_ROUNDRECTANGLE_H_

#include <gfx/Shape.h>

class RoundRectangle: public Shape {
public:
	explicit RoundRectangle(uint16_t h, uint16_t v, uint16_t width, uint16_t height, uint8_t radius);
	virtual ~RoundRectangle() = default;

	bool isPositionInActiveRegion(int16_t h, int16_t v) override;

	void drawContour(const DrawableCanvas& canvas, const PixelProvider& pixel, unsigned int borderWidth) override;
	void fill(const DrawableCanvas& canvas, const PixelProvider& pattern) override;

	void setHorizontal(uint16_t h);
	void setVertical(uint16_t v);
	void setWidth(uint16_t width);
	void setHeight(uint16_t height);
	void setRadius(uint8_t radius);
	uint8_t radius() const { return m_Radius; }

protected:
	uint8_t m_Radius;
};

#endif /* GFX_ROUNDRECTANGLE_H_ */
