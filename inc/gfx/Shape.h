/*
 * Shape.h
 *
 *  Created on: May 22, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "tui_gui_base.h"

#include "DrawableCanvas.h"
#include "ColorGradient.h"

class Shape {
	friend class Canvas;

public:
	enum DRAW_MODE {
		CONTUR = 0,
		PIXEL_FILL,
		VERTICAL_GRADIENT_FILL,
		HORIZONTAL_GRADIENT_FILL,
	};

	explicit Shape() = default;
	virtual ~Shape() = default;

	Shape(const Shape& s) = delete;
	const Shape& operator=(const Shape& s) = delete;

	virtual void drawContour(const DrawableCanvas& canvas, const PixelColorProvider& provider, unsigned int borderWidth) = 0;
	virtual void fill(const DrawableCanvas& canvas, const PixelColorProvider& provider);

	virtual float isPositionInActiveRegion(int16_t h, int16_t v) = 0;

	int16_t horizontal() const { return m_HStart; }
	int16_t vertical() const { return m_VStart; }
	int16_t height() const { return m_Height; }
	int16_t width() const { return m_Width; }

protected:
	int16_t m_HStart = 0;
	int16_t m_VStart = 0;

	uint16_t m_Width = SCREEN_WIDTH_SIZE;
	uint16_t m_Height = SCREEN_HEIGHT_SIZE;
};
