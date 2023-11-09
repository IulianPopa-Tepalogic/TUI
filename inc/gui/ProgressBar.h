/*
 * ProgressBar.h
 *
 *  Created on: Aug 3, 2020
 *      Author: IulianPopa
 */

#ifndef GUI_PROGRESSBAR_H_
#define GUI_PROGRESSBAR_H_

#include <gui/Control.h>

#include <gui_theme_constants.h>

class ProgressBar: public Control {
public:
	ProgressBar(bool vertical = false, unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);

	ProgressBar& setCompletedColor(Pixel& color);
	ProgressBar& setRemainColor(Pixel& color);

	ProgressBar& setValue(float level);
	float value() const { return m_Level / 10000.0f; }
	const ProgressBar& operator= (const float value) { setValue(value); return *this; }
	const ProgressBar& operator+= (const float delta) { setValue(delta + value()); return *this; }
	const ProgressBar& operator-= (const float delta) { return *this += -delta; }
	operator float() const { return value(); }

	const bool m_Vertically;

protected:
	void __draw(DrawableCanvas& canvas) override;

	Pixel m_CompletedColor = PBAR_COMPLETED_COLOR;
	Pixel m_RemainColor = PBAR_REMAIN_COLOR;

	uint16_t m_Level = 0;
};

#endif /* GUI_PROGRESSBAR_H_ */
