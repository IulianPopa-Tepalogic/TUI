/*
 * Slider.cpp
 *
 *  Created on: Jul 27, 2020
 *      Author: IulianPopa
 */

#include <gui/Slider.h>

#include <gfx/RoundRectangle.h>
#include <gui/Dialog.h>


 Slider::Slider(bool vertical, unsigned int h, unsigned int v, unsigned int width, unsigned int height)
 	 : Control(h, v, width, height),
	   m_Vertically(vertical)
 {
	 setBorderColor(Pixel(255, 255, 255));
	 setBorderSize(1);
	 setBorderRadius(SLIDER_LEVEL_RECT_RADIUS);
 }

Slider& Slider::setCompletedColor(Pixel& color)
{
	m_CompletedColor = color;
	markDirty();

	return *this;
}

Slider& Slider::setRemainColor(Pixel& color)
{
	m_RemainColor = color;
	markDirty();

	return *this;
}

Slider& Slider::setLevelThickness(uint8_t thick)
{
	m_LevelThickness = thick;
	markDirty();

	return *this;
}

Slider& Slider::setLevelRadius(uint8_t radius)
{
	m_LevelRadius = radius;
	markDirty();

	return *this;
}

Slider& Slider::setIndicatorThickness(uint8_t thick)
{
	m_IndicatorThickness = thick;
	markDirty();

	return *this;
}

Slider& Slider::setIndicatorRadius(uint8_t radius)
{
	m_IndicatorRadius = radius;
	markDirty();

	return *this;
}

Slider& Slider::setIndicatorColor(Pixel color)
{
	m_IndicatorColor = color;
	markDirty();

	return *this;
}

Slider& Slider::setValue(float level)
{
	level = (level < .0f) ? .0f : (1.0f < level ? 1.0f : level);
	const uint16_t temp = level * 10000.0f;
	if (temp == m_Level)
		return *this;

	m_Level = temp;
	markDirty();

	if (m_EventsHandler != nullptr)
		m_EventsHandler->processControlEvent(*this, VALUE_UPDATED);

	return *this;
}

bool Slider::getAnimatedProperty(Animation::PROPERTY property, float* outValue)
{
	if (Control::getAnimatedProperty(property, outValue))
		return true;

	switch (property)
	{
	case Animation::VALUE:
		*outValue = value();
		break;

	default:
		return false;
	}

	return true;
}

bool Slider::getAnimatedProperty(Animation::PROPERTY property, Pixel* outColor)
{
	if (Control::getAnimatedProperty(property, outColor))
		return true;

	switch (property)
	{
	case Animation::COMPLETED_COLOR:
		*outColor = m_CompletedColor;
		break;

	case Animation::REMAIN_COLOR:
		*outColor = m_RemainColor;
		break;

	case Animation::INDICATOR_COLOR:
		*outColor = m_IndicatorColor;
		break;

	default:
		return false;
	}

	return true;
}

bool Slider::updateAnimatedProperty(Animation::PROPERTY property, float value)
{
	if (Control::updateAnimatedProperty(property, value))
		return true;

	switch (property)
	{
	case Animation::VALUE:
		setValue(value);
		break;

	default:
		return false;
	}

	return true;
}

bool Slider::updateAnimatedProperty(Animation::PROPERTY property, Pixel color)
{
	if (Control::updateAnimatedProperty(property, color))
		return true;

	switch (property)
	{
	case Animation::COMPLETED_COLOR:
		setCompletedColor(color);
		break;

	case Animation::REMAIN_COLOR:
		setRemainColor(color);
		break;

	case Animation::INDICATOR_COLOR:
		setIndicatorColor(color);
		break;

	default:
		return false;
	}

	return true;
}

bool Slider::handleUserInteractionEvent(unsigned int event, int h, int v)
{
	const auto prevResult = Control::handleUserInteractionEvent(event, h, v);

	switch (event)
	{
	case GUIApp::TOUCH_CONTACT:
	case GUIApp::TOUCH_DOWN:
		if (m_Vertically)
		{
			v -= vertical();
			setValue(1.0f - (float)v / height());
		}
		else
		{
			h -= horizontal();
			setValue((float)h / width());
		}
		return true;

	case GUIApp::TOUCH_UP:
		return true;

	default:
		return prevResult;
	}

	return true;
}


void Slider::__draw(DrawableCanvas& canvas)
{
	if (m_Vertically)
	{
		RoundRectangle rr((canvas.m_Width - m_LevelThickness) / 2, 0, m_LevelThickness, canvas.m_Height, m_LevelRadius);
		rr.fill(canvas, Unicolor(m_RemainColor));

		const int16_t fillHeight = canvas.m_Height - canvas.m_Height * value();
		for (int v = canvas.m_Height - 1; fillHeight < v; --v)
			for (int h = 0; h < canvas.m_Width; ++h)
			{
				if (rr.isPositionInActiveRegion(h, v))
					canvas.setPixel(h, v, m_CompletedColor);
			}

		rr.drawContour(canvas, Unicolor(m_BorderColor), m_BorderSize);

		RoundRectangle indicator(0, fillHeight  - (m_IndicatorThickness / 2),  canvas.m_Width, m_IndicatorThickness, m_IndicatorRadius);
		indicator.fill(canvas, Unicolor(m_IndicatorColor));
	}
	else
	{
		RoundRectangle rr(0, (canvas.m_Height - m_LevelThickness) / 2, canvas.m_Width, m_LevelThickness, m_LevelRadius);
		rr.fill(canvas, Unicolor(m_RemainColor));

		const int16_t fillWidth = canvas.m_Width * value();
		for (int v = 0; v < canvas.m_Height; ++v)
			for (int h = 0; h < fillWidth; ++h)
			{
				if (rr.isPositionInActiveRegion(h, v))
					canvas.setPixel(h, v, m_CompletedColor);
			}

		rr.drawContour(canvas, Unicolor(m_BorderColor), m_BorderSize);

		RoundRectangle indicator(fillWidth - (m_IndicatorThickness / 2), 0, m_IndicatorThickness, canvas.m_Height, m_IndicatorRadius);
		indicator.fill(canvas, Unicolor(m_IndicatorColor));
	}
}
