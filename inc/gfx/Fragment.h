/*
 * Fragment.h
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */
#pragma once

#include <gfx/DrawableCanvas.h>

class Fragment : public DrawableCanvas {

public:
	Fragment(int hfrom = 0, int vfrom = 0, int width = 0, int height = 0, DrawableCanvas* parent = nullptr);
	virtual ~Fragment() = default;

	void setPixel (int h, int v, Pixel pixel) const override;
	Pixel getPixel (int h, int v) const override;

	void UpdateCanvas(DrawableCanvas& canvas);
	void UpdateArea(int hfrom = 0, int vfrom = 0, int width = 0, int height = 0);

	bool overlaps(const Fragment& other) const;
	unsigned int horizontal() const { return m_HFrom; }
	unsigned int vertical() const { return m_VFrom; }

	void startFrameDrawing() override;
	void endFrameDrawing() override;

protected:

	DrawableCanvas* m_Canvas;
	int16_t m_HFrom;
	int16_t m_VFrom;
};
