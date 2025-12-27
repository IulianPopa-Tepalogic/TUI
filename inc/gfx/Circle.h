/*
 * Circle.h
 *
 *  Created on: May 25, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gfx/Shape.h"

class Circle : public Shape {
public:
	Circle(int16_t hcenter, int16_t vcenter, uint16_t radius);
	virtual ~Circle() = default;

	float isPositionInActiveRegion(int16_t h, int16_t v) override;

	void drawContour(const DrawableCanvas&, const PixelColorProvider& pixel, unsigned int borderWidth) override;

	void setHorizontal(int16_t hcenter);
	void setVertical(int16_t vcenter);
	void setRadius(uint16_t radius);

protected:
	void updateShapeInternals();

	int16_t m_HCenter;
	int16_t m_VCenter;
	uint16_t m_Radius;
};
