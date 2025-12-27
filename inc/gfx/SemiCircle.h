/*
 * SemiCircle.h
 *
 *  Created on: May 25, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gfx/Shape.h"

class SemiCircle : public Shape {
public:
	SemiCircle(int16_t hcenter, int16_t vcenter, uint16_t radius, float fromDegCW, float endDegCW);
	virtual ~SemiCircle() = default;

	float isPositionInActiveRegion(int16_t h, int16_t v) override;
	void drawContour(const DrawableCanvas& canvas, const PixelColorProvider& pixel, unsigned int borderWidth) override;

	void setHorizontal(int16_t hcenter);
	void setVertical(int16_t vcenter);
	void setRadius(uint16_t radius);
	void setFromAngleCW(float from);
	void setToAngleCW(float to);

protected:
	void updateShapeInternals();

	float m_AngleStart;
	float m_AngleStop;
	int16_t m_HCenter;
	int16_t m_VCenter;
	uint16_t m_Radius;
};
