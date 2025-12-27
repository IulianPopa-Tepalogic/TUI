/*
 * TextLabel.cpp
 *
 *  Created on: Jul 18, 2020
 *      Author: IulianPopa
 */

#include <gui/Label.h>

#include <math.h>

#include "gfx/RoundRectangle.h"

using namespace std;
using namespace tui;

Label::Label(const TUI_CHAR* text, unsigned int h, unsigned int v, unsigned int width, unsigned int height)
	: Control(h, v, width, height),
	  m_Text(text)
{
	if (width <= 0)
		setWidth(ceil(m_Font->getStringWidth(text) * m_Size + 10.0f));

	if (height <= 0)
		setHeight(ceil(m_Font->getStringMaxHeight(text) * m_Size + 3.0f));
}

Label& Label::setText(const TUI_CHAR* text)
{
	m_Text = text;
	markDirty();

	if (m_EventsHandler != nullptr)
		m_EventsHandler->processControlEvent(*this, TEXT_CHANGED);

	return *this;
}

Label& Label::setFont(Font& font)
{
	m_Font = &font;
	markDirty();

	return *this;
}

Label& Label::setSize(float size)
{
	m_Size = size;
	markDirty();

	return *this;
}

Label& Label::setTextColor(const Pixel color)
{
	m_Color = color;
	markDirty();

	return *this;
}

Label& Label::writeVertically(bool vertically)
{
	m_Vertically = vertically;
	markDirty();

	return *this;
}

void Label::__draw(DrawableCanvas& canvas)
{
	const auto pos = (m_BorderSize > 0) ? m_BorderSize + m_BorderRadius : 0;
	canvas.writeText(m_Text, *m_Font, Unicolor(m_Color), pos, pos, m_Size, m_Vertically);

	if (m_BorderSize > 0)
	{
		RoundRectangle controlShape(0, 0, canvas.m_Width, canvas.m_Height, m_BorderRadius);
		controlShape.drawContour(canvas, Unicolor(m_BorderColor), m_BorderSize);
	}
}
