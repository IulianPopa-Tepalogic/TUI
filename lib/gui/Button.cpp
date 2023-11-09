/*
 * Button.cpp
 *
 *  Created on: Jul 18, 2020
 *      Author: IulianPopa
 */

#include "gui/Button.h"
#include <algorithm>
#include <math.h>

#include "gfx/RoundRectangle.h"
#include "gui/GUIApp.h"
#include "gui/Dialog.h"


using namespace std;

Button::Button(const USER_CHAR* text, unsigned int h, unsigned int v, unsigned int width, unsigned int height)
	: Control(h, v, width, height),
	  m_Text((text != nullptr) ? text : "")
{
	if (width <= 0)
	{
		auto w = (m_Text != nullptr) ? m_Font->getStringWidth(m_Text) : 0;
		w *= max(m_TextPressedSize, m_TextSize);
		w += 10.0f;

		setWidth(ceil(w));
	}

	if (height <= 0)
	{
		auto h = (m_Text != nullptr) ? m_Font->getStringWidth(m_Text) : 0;
		h *= max(m_TextPressedSize, m_TextSize);
		h += 5.0f;

		setHeight(ceil(h));
	}
}

Button& Button::setText(const USER_CHAR* text)
{
	m_Text = text;
	markDirty();

	if (m_EventsHandler != nullptr)
		m_EventsHandler->processControlEvent(*this, TEXT_CHANGED);

	return *this;
}

Button& Button::setFont(Font& font)
{
	m_Font = &font;
	markDirty();

	return *this;
}

Button& Button::setSize(float size, bool pressed)
{
	float* const s = pressed ? &m_TextPressedSize : &m_TextSize;
	*s = size;

	markDirty();

	return *this;
}

Button& Button::setTextColor(const Pixel color, bool pressed)
{
	Pixel* const c = pressed ? &m_TextPressedColor : &m_TextColor;
	*c = color;

	markDirty();

	return *this;
}

Button& Button::setBkColor(Pixel color, bool pressed)
{
	Pixel* const c = pressed ? &m_BkPressedColor : &m_BkColor;
	*c = color;

	markDirty();

	return *this;
}

Button& Button::setBkPattern(const Canvas* pattern, bool pressed)
{
	const Canvas** const c = pressed ? &m_BkPressedPattern : &m_BkPattern;
	*c = pattern;

	markDirty();

	return *this;
}

bool Button::handleUserInteractionEvent(unsigned int event, int h, int v)
{
	switch (event)
	{
	case GUIApp::TOUCH_CONTACT:
	case GUIApp::TOUCH_DOWN:
		if (m_Pressed != true)
		{
			m_Pressed = true;
			markDirty();

			if (m_EventsHandler != nullptr)
				m_EventsHandler->processControlEvent(*this, TOGGLED_CHANGED);
		}

		if (m_EventsHandler != nullptr)
			m_EventsHandler->processControlEvent(*this, PRESSED);

		break;

	case GUIApp::LOST_FOCUS:
	case GUIApp::TOUCH_UP:
		if (m_Pressed)
		{
			m_Pressed = false;
			markDirty();

			if (m_EventsHandler != nullptr)
				m_EventsHandler->processControlEvent(*this, TOGGLED_CHANGED);
		}

		if (m_EventsHandler != nullptr)
			m_EventsHandler->processControlEvent(*this, RELEASED);

		break;

	default:
		return Control::handleUserInteractionEvent(event, h, v);
	}

	return true;
}

bool Button::getAnimatedProperty(Animation::PROPERTY property, float* outValue)
{
	if (Control::getAnimatedProperty(property, outValue))
		return true;

	switch (property)
	{
	case Animation::TEXT_SIZE:
		*outValue = m_TextSize;
		break;

	case Animation::BT_TEXT_SIZE_PRESSED:
		*outValue = m_TextPressedSize;
		break;

	default:
		return false;
	}

	return true;
}

bool Button::getAnimatedProperty(Animation::PROPERTY property, Pixel* outColor)
{
	if (Control::getAnimatedProperty(property, outColor))
		return true;

	switch (property)
	{
	case Animation::TEXT_COLOR:
		*outColor = m_TextColor;
		break;

	case Animation::BT_TEXT_COLOR_PRESSED:
		*outColor = m_TextPressedColor;
		break;

	case Animation::BKG_COLOR:
		*outColor = m_BkColor;
		break;

	case Animation::BT_BK_COLOR_PRESSED:
		*outColor = m_BkPressedColor;
		break;

	default:
		return false;
	}

	return true;
}

bool Button::updateAnimatedProperty(Animation::PROPERTY property, float value)
{
	if (Control::updateAnimatedProperty(property, value))
		return true;

	switch (property)
	{
	case Animation::TEXT_SIZE:
		setSize(value, false);
		break;

	case Animation::BT_TEXT_SIZE_PRESSED:
		setSize(value, true);
		break;

	default:
		return false;
	}

	return true;
}

bool Button::updateAnimatedProperty(Animation::PROPERTY property, Pixel color)
{
	if (Control::updateAnimatedProperty(property, color))
		return true;

	switch (property)
	{
	case Animation::TEXT_COLOR:
		setTextColor(color, false);
		break;

	case Animation::BT_TEXT_COLOR_PRESSED:
		setTextColor(color, true);
		break;

	case Animation::BKG_COLOR:
		setBkColor(color, false);
		break;

	case Animation::BT_BK_COLOR_PRESSED:
		setBkColor(color, true);
		break;

	default:
		return false;
	}

	return true;
}


void Button::__draw(DrawableCanvas& canvas)
{
	RoundRectangle controlShape(0, 0, canvas.m_Width, canvas.m_Height, m_BorderRadius);

	const Canvas* bkPattern = m_Pressed ? m_BkPressedPattern : m_BkPattern;
	if (bkPattern != nullptr)
		controlShape.fill(canvas, PatternPixelProvider(*bkPattern, true, true, controlShape));
	else
		controlShape.fill(canvas, ColorPixelProvider(m_Pressed ? m_BkPressedColor : m_BkColor));

	if (m_BorderSize > 0)
		controlShape.drawContour(canvas, ColorPixelProvider(m_BorderColor), m_BorderSize);

	if (m_Text == nullptr)
		return ;

	const auto textSize  = (m_Pressed ? m_TextPressedSize : m_TextSize);
	const auto textWidth = m_Font->getStringWidth(m_Text) * textSize;
	const auto textHeight = m_Font->getStringMaxHeight(m_Text) * textSize;

	const int htext = (m_Width - textWidth) / 2;
	const int vtext = (m_Height - textHeight) / 2;

	canvas.writeText(m_Text, *m_Font, ColorPixelProvider(m_Pressed ? m_TextPressedColor : m_TextColor), htext, vtext, textSize, false);
}
