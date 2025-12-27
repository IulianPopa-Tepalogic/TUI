/*
 * ProgressBar.cpp
 *
 *  Created on: Aug 3, 2020
 *      Author: IulianPopa
 */

#include <gui/ProgressBar.h>
#include <gfx/RoundRectangle.h>
#include <gfx/Rectangle.h>

#include <algorithm>

using namespace std;
using namespace tui;


 ProgressBar::ProgressBar(bool vertical, unsigned int h, unsigned int v, unsigned int width, unsigned int height)
 	 : Control(h, v, width, height),
	   m_Vertically(vertical)
{
	 Control::setBorderColor(PBAR_BORDER_COLOR);
	 Control::setBorderRadius(PBAR_BORDER_RADIUS);
	 Control::setBorderSize(PBAR_BORDER_SIZE);
}

ProgressBar& ProgressBar::setCompletedColor(Pixel& color)
{
	m_CompletedColor = color;
	markDirty();

	return *this;
}

ProgressBar& ProgressBar::setRemainColor(Pixel& color)
{
	m_RemainColor = color;
	markDirty();

	return *this;
}

ProgressBar& ProgressBar::setValue(float level)
{
	level = level < 0 ? 0 : ((level > 1.0) ? 1.0f : level);
	const uint16_t temp = level * 10000.0f;
	if (m_Level != temp)
	{
		m_Level = temp;
		markDirty();

		if (m_EventsHandler != nullptr)
			m_EventsHandler->processControlEvent(*this, VALUE_UPDATED);
	}

	return *this;
}

void ProgressBar::__draw(DrawableCanvas& canvas)
{
	RoundRectangle rr(0, 0, canvas.m_Width, canvas.m_Height, m_BorderRadius);
	rr.fill(canvas, Unicolor(m_RemainColor));

	if (m_Vertically)
	{
		const uint16_t fillHeight = canvas.m_Height - canvas.m_Height * value();
		for (int v = canvas.m_Height - 1; fillHeight < v; --v)
			for (int h = 0; h < canvas.m_Width; ++h)
			{
				if (rr.isPositionInActiveRegion(h, v))
					canvas.setPixel(h, v, m_CompletedColor);
			}
	}
	else
	{
		const uint16_t fillWidth = canvas.m_Width * value();
		for (int v = 0; v < canvas.m_Height; ++v)
			for (int h = 0; h < fillWidth; ++h)
			{
				if (rr.isPositionInActiveRegion(h, v))
					canvas.setPixel(h, v, m_CompletedColor);
			}

	}

	rr.drawContour(canvas, Unicolor(m_BorderColor), m_BorderSize);
}
