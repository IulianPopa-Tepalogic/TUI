/*
 * Slider.h
 *
 *  Created on: Jul 27, 2020
 *      Author: IulianPopa
 */

#ifndef GUI_SLIDER_H_
#define GUI_SLIDER_H_

#include <gui/Control.h>
#include <gfx/ColorGradient.h>

#include "gui_theme_constants.h"

class Slider: public Control {
public:
	Slider(bool vertical = false, unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);
	virtual ~Slider() = default;

	Slider& setCompletedColor(Pixel& color);
	Slider& setRemainColor(Pixel& color);
	Slider& setLevelThickness(uint8_t thick);
	Slider& setLevelRadius(uint8_t radius);
	Slider& setIndicatorThickness(uint8_t thick);
	Slider& setIndicatorRadius(uint8_t radius);
	Slider& setIndicatorColor(Pixel color);

	Slider& setValue(float level);
	float value() const { return m_Level / 10000.0f; }
	const Slider& operator= (const float value) { setValue(value); return *this; }
	const Slider& operator+= (const float delta) { setValue(delta + value()); return *this; }
	const Slider& operator-= (const float delta) { return *this += -delta; }
	operator float() const { return value(); }

	bool getAnimatedProperty(Animation::PROPERTY property, float* outValue) override;
	bool getAnimatedProperty(Animation::PROPERTY property, Pixel* outColor) override;
	bool updateAnimatedProperty(Animation::PROPERTY property, float value) override;
	bool updateAnimatedProperty(Animation::PROPERTY property, Pixel color) override;

	bool handleUserInteractionEvent(unsigned int event, int h, int v) override;

	const bool m_Vertically;

protected:
	void __draw(DrawableCanvas& canvas) override;

	Pixel m_CompletedColor = SLIDER_LEVEL_COMPLETED_COLOR;
	Pixel m_RemainColor = SLIDER_LEVEL_REMAIN_COLOR;
	Pixel m_IndicatorColor = SLIDER_INDICATOR_COLOR;
	uint8_t m_LevelThickness = SLIDER_LEVEL_RECT_THICK;
	uint8_t m_LevelRadius  = SLIDER_LEVEL_RECT_RADIUS;
	uint8_t m_IndicatorThickness = SLIDER_INDICATOR_RECT_THICK;
	uint8_t m_IndicatorRadius = SLIDER_INDICATOR_RECT_RADIUS;

	uint16_t m_Level = 0;
};

#endif /* GUI_SLIDER_H_ */
