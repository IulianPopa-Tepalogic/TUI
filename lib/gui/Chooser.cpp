/*
 * Chooser.cpp
 *
 *  Created on: Dec 9, 2020
 *      Author: IulianPopa
 */

#include <gui/Chooser.h>

#include <algorithm>

#include "gui/GUIApp.h"
#include "gfx/RoundRectangle.h"


using namespace std;

Chooser::Chooser(unsigned int h, unsigned int v, unsigned int width, unsigned int height)
	: List(true, h, v, width, height)
{
	m_ExpandedHorizontal = horizontal();
	m_ExpandedVertical = vertical();
	m_ExpandedHeight = this->height();
	m_ExpandedWidth = this->width();

}

bool Chooser::handleUserInteractionEvent(unsigned int event, int h, int v)
{
	if (m_Expanded)
	{
		if ((event == GUIApp::TOUCH_UP)
			&& ! m_UserScrolled)
		{
			selectItem(retrieveSelectedItem(h - horizontal(), v - vertical()));
			if (m_EventsHandler != nullptr)
				m_EventsHandler->processControlEvent(*this, SELECTION_UPDATED);

			showExpanded(false);
			return true;
		}
		else if (event == GUIApp::LOST_FOCUS)
		{
			showExpanded(false);
			return true;

		}

		return Scroller::handleUserInteractionEvent(event, h, v);
	}

	auto result = Control::handleUserInteractionEvent(event, h, v);
	if ((event == GUIApp::TOUCH_DOWN)
		|| (event == GUIApp::TOUCH_CONTACT))
	{
		showExpanded(true);
		result = true;
	}

	return result;
}

Chooser& Chooser::setExpandedHorizontal(uint16_t h)
{
	if (m_Expanded)
		setHorizontal(h);
	else
		m_ExpandedHorizontal = h;

	markDirty();
	return *this;
}

Chooser& Chooser::setExpandedVertical(uint16_t v)
{
	if (m_Expanded)
		setVertical(v);
	else
		m_ExpandedVertical = v;

	markDirty();
	return *this;
}


Chooser& Chooser::setExpandedHeight(uint16_t height)
{
	if (m_Expanded)
		setHeight(height);

	else
		m_ExpandedHeight = height;

	markDirty();
	return *this;
}

Chooser& Chooser::setExpandedWidth(uint16_t width)
{
	if (m_Expanded)
		setWidth(width);

	else
		m_ExpandedWidth = width;

	markDirty();
	return *this;
}


void Chooser::__draw(DrawableCanvas& canvas)
{
	if (m_Expanded)
	{
		List::__draw(canvas);
		return ;
	}

	const void* item = ((m_SelectedItem < 0) || (m_Model == nullptr)) ? nullptr : m_Model->itemAt(m_SelectedItem);
	m_Illustrator->draw(canvas, m_SelectedItem, ItemsIllustrator::CHOOSED_ITEM, item);

	if (m_BorderSize > 0)
	{
		RoundRectangle rr(0, 0, canvas.m_Width, canvas.m_Height, m_BorderRadius);
		rr.drawContour(canvas, Unicolor(m_BorderColor), m_BorderSize);
	}
}

void Chooser::showExpanded(bool expanded)
{
	if (m_Expanded == expanded)
		return ;

	m_Expanded = expanded;

	uint16_t h = horizontal();
	uint16_t v = vertical();
	uint16_t ht = height();
	uint16_t wd = width();

	swap(h, m_ExpandedHorizontal);
	swap(v, m_ExpandedVertical);
	swap(ht, m_ExpandedHeight);
	swap(wd, m_ExpandedWidth);

	setHorizontal(h);
	setVertical(v);
	setHeight(ht);
	setWidth(wd);

	if (m_EventsHandler != nullptr)
		m_EventsHandler->promoteControl(*this);

	markDirty();
}

