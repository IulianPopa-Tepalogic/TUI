/*
 * Keyboard.h
 *
 *  Created on: Jan 2, 2021
 *      Author: IulianPopa
 */

#pragma once

#include "gui/ControlsContainers.h"
#include "gui/Button.h"

class NumericKeyboardModel : public ControlsContainerModel {
public:
	NumericKeyboardModel() = default;
	virtual ~NumericKeyboardModel() = default;

	bool showDecimalPoint() const { return m_ShowDecimalPoint; }
	void showDecimalPoint(bool show);

	bool showNegate() const { return m_ShowNegatePoint; }
	void showNegate(bool show);

	int controlsCount() override;
	Control* getControl(unsigned int index) override;

	unsigned int verticalSpacing() const { return m_VerticalSpacing; }
	void verticalSpacing(unsigned int spacing);

	unsigned int horizontalSpacing() const { return m_HorizontalSpacing; }
	void horizontalSpacing(unsigned int spacing);

protected:
	bool drawControl(DrawableCanvas& canvas, int control) override;
	void cacheCanvasSize(int width, int height) override;

	Button m_Digits[10];
	Button m_DecimalPoint;
	Button m_Delete;
	Button m_Negate;

	uint8_t m_VerticalSpacing = 1;
	uint8_t m_HorizontalSpacing = 1;

	bool m_ShowDecimalPoint = true;
	bool m_ShowNegatePoint = true;

private:

};



class Keyboard: public ControlsContainer {
public:
	Keyboard(unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);
	~Keyboard() override = default;

protected:
	void __draw(DrawableCanvas& canvas) override;

	uint8_t m_Spacing = 0;
};

