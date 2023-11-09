/*
 * TextLabel.h
 *
 *  Created on: Jul 18, 2020
 *      Author: IulianPopa
 */

#ifndef GUI_TEXTLABEL_H_
#define GUI_TEXTLABEL_H_

#include <gui/Control.h>

#include "gui_theme_constants.h"

class Label: public Control {
public:
	Label(const USER_CHAR* text, unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);
	virtual ~Label() = default;

	Label& setText(const USER_CHAR* text);
	Label& setFont(Font& font);
	Label& setSize(float size);
	Label& setTextColor(const Pixel color);
	Label& writeVertically(bool vertically);

protected:
	void __draw(DrawableCanvas& canvas) override;

	const USER_CHAR* m_Text;
	Font* m_Font = &LABEL_TEXT_FONT;
	float m_Size = LABEL_TEXT_SIZE;
	Pixel m_Color = LABEL_TEXT_COLOR;
	bool m_Vertically = false;
};

#endif /* GUI_LABEL_H_ */
