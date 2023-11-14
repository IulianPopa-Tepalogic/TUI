/*
 * Rectangle.h
 *
 *  Created on: May 23, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "Shape.h"
#include "ColorGradient.h"

class Rectangle: public Shape {
public:
	explicit Rectangle(uint16_t h, uint16_t v, uint16_t width, uint16_t height);
	virtual ~Rectangle() = default;

	float isPositionInActiveRegion(int16_t h, int16_t v) override;

	void drawContour(const DrawableCanvas& canvas, const PixelColorProvider& pixel, unsigned int borderWidth) override;
	void fill(const DrawableCanvas& canvas, const PixelColorProvider& color) override;

	void setHorizontal(uint16_t h);
	void setVertical(uint16_t v);
	void setWidth(uint16_t width);
	void setHeight(uint16_t height);
};


