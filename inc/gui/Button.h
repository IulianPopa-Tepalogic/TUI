/*
 * Button.h
 *
 *  Created on: Jul 18, 2020
 *      Author: IulianPopa
 */

#ifndef GUI_BUTTON_H_
#define GUI_BUTTON_H_

#include <gui/Control.h>

#include "gui_theme_constants.h"

class Button: public Control {
public:
	Button(const TUI_CHAR* text = nullptr, unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);
	virtual ~Button() = default;

	Button& setText(const TUI_CHAR* text);
	Button& setFont(Font& font);
	Button& setSize(float size, bool pressed);
	Button& setTextColor(const Pixel color, bool pressed);
	Button& setBkColor(Pixel color, bool pressed);
	Button& setBkPattern(const Canvas* pattern, bool pressed);

	bool getAnimatedProperty(Animation::PROPERTY property, float* outValue) override;
	bool getAnimatedProperty(Animation::PROPERTY property, Pixel* outColor) override;
	bool updateAnimatedProperty(Animation::PROPERTY property, float value) override;
	bool updateAnimatedProperty(Animation::PROPERTY property, Pixel color) override;

	bool IsPressed() const { return m_Pressed; };

	bool handleUserInteractionEvent(unsigned int event, int h, int v) override;

protected:
	const TUI_CHAR* m_Text;
	Font* m_Font = &BUTTON_TEXT_FONT;

	const Canvas* m_BkPattern = BUTTON_BK_PATTERN;
	const Canvas* m_BkPressedPattern = BUTTON_PRESSED_BK_PATTERN;
	float m_TextSize = BUTTON_TEXT_SIZE;
	float m_TextPressedSize = BUTTON_PRESSED_TEXT_SIZE;
	Pixel m_TextColor = BUTTON_TEXT_COLOR;
	Pixel m_TextPressedColor = BUTTON_PRESSED_TEXT_COLOR;
	Pixel m_BkColor = BUTTON_BK_COLOR;
	Pixel m_BkPressedColor = BUTTON_PRESSED_BK_COLOR;

protected:
	void __draw(DrawableCanvas& canvas) override;

	bool m_Pressed = false;
};

#endif /* GUI_BUTTON_H_ */
