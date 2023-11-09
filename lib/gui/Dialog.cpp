/*
 * Dialog.cpp
 *
 *  Created on: Jun 16, 2020
 *      Author: IulianPopa
 */

#include <algorithm>
#include <math.h>

#include "gui/Dialog.h"
#include "gui/Control.h"

#include "gfx/Rectangle.h"


using namespace std;


static void default_event_callback(Control& control, const CONTROL_EVENT event)
{
	//Do nothing!
}

Dialog::Dialog(const USER_CHAR* title)
	: m_Title(title),
	  mf_ControlEventCallback(default_event_callback)
{
}


void Dialog::updateCanvas(DrawableCanvas& canvas, unsigned int h, unsigned int v, unsigned int width, unsigned int height)
{
	m_Canvas = &canvas;

	m_Height = height;
	if (m_Height <= 0)
		m_Height = canvas.m_Height;

	if (m_Height < DIALOG_TITLE_HEIGHT)
		implError();

	m_Width = width;
	if (m_Width <= 0)
		m_Width = canvas.m_Width;

	m_NeedsRedraw = true;
}

Dialog& Dialog::setTitle(const USER_CHAR* title)
{
	m_Title = title;
	m_NeedsRedraw = true;

	return *this;
}


Dialog& Dialog::setTitleBackgroundPattern(const Canvas* canvas)
{
	m_TitleBackgroundPattern = canvas;
	m_NeedsRedraw = true;
	return *this;
}


Dialog& Dialog::setBackgroundColor(Pixel color)
{
	m_BackgroundPattern = nullptr;
	m_BackgroundColor = color;
	m_NeedsRedraw = true;

	return *this;
}


Dialog& Dialog::setBackgroundPattern(const Canvas* canvas)
{
	m_BackgroundPattern = canvas;
	m_NeedsRedraw = true;

	return *this;
}

Dialog& Dialog::setControlEventCallBack(CONTROL_CALL_BACK cb)
{
	mf_ControlEventCallback = (cb == nullptr) ? default_event_callback : cb;

	return *this;
}


void Dialog::addControl(Control& control)
{
	if (m_ListHead == nullptr)
	{
		m_ListHead = m_ListTail = &control;
		control.m_PrevControl = control.m_NextControl = nullptr;
	}
	else
	{
		m_ListTail->m_NextControl = &control;
		control.m_PrevControl = m_ListTail;
		control.m_NextControl = nullptr;
		m_ListTail = &control;
	}

	control.m_EventsHandler = this;

	forceRedraw();
}

void Dialog::removeControl(Control& control)
{
	if (&control == m_ListHead)
	{
		m_ListHead = m_ListHead->m_NextControl;
		if (m_ListHead)
			m_ListHead->m_PrevControl = nullptr;
	}
	else if (&control == m_ListTail)
	{
		m_ListTail = m_ListTail->m_PrevControl;
		if (m_ListTail)
			m_ListTail->m_NextControl = nullptr;
	}
	else
	{
		auto prev = control.m_PrevControl;
		if (prev)
			prev->m_NextControl = control.m_NextControl;

		auto next = control.m_NextControl;
		if (next)
			next->m_PrevControl = prev;
	}

	forceRedraw();
}

void Dialog::promoteControl(Control& control)
{
	removeControl(control);
	return addControl(control);
}


void Dialog::forceRedraw()
{
	m_NeedsRedraw = true;
}

bool Dialog::draw(bool force)
{
	bool result = false;

	if (force | m_NeedsRedraw)
		force = drawDialogFrame();

	else
	{
		for (auto it = m_ListHead; it != nullptr; it = it->m_NextControl)
		{
			if (it->needsRedraw())
			{
				result = true;

				const auto v = it->vertical();
				const auto h = it->horizontal();
				const auto height = it->height();
				const auto width = it->width();

				for (auto vi = v ; vi < v + height; ++vi)
					for (auto hi = h ; hi < h + width; ++hi)
						m_UserArea.setPixel(hi, vi, m_BackgroundColor);
			}
		}

		if (! result)
			return false;

		if (m_BackgroundPattern != nullptr)
			m_UserArea.fillMaskPatern(*m_BackgroundPattern, m_BackgroundColor);
	}

	for (auto it = m_ListHead; it != nullptr; it = it->m_NextControl)
	{
		if (force | it->needsRedraw())
		{
			result = true;
			it->draw(m_UserArea);
		}
	}

	return result | force;
}

bool Dialog::tickAnimations()
{
	bool result = false;
	for (auto it = m_ListHead; it != nullptr; it = it->m_NextControl)
		result |= it->tickAnimatedProperties();

	return result;
}


bool Dialog::drawDialogFrame()
{
	if (m_Canvas == nullptr)
		implError();

	m_Canvas->fillColor(Pixel(0, 0, 0));
	if (DIALOG_BORDER_SIZE >= 0)
	{
		m_Canvas->drawHLine(0, 0, m_Canvas->m_Width, DIALOG_BORDER_SIZE, ColorPixelProvider(DIALOG_BORDER_COLOR));
		m_Canvas->drawHLine(0, m_Canvas->m_Height - DIALOG_BORDER_SIZE, m_Canvas->m_Width, DIALOG_BORDER_SIZE, ColorPixelProvider(DIALOG_BORDER_COLOR));

		m_Canvas->drawVLine(0, 0, m_Canvas->m_Height, DIALOG_BORDER_SIZE, ColorPixelProvider(DIALOG_BORDER_COLOR));
		m_Canvas->drawVLine(m_Canvas->m_Width - DIALOG_BORDER_SIZE, 0, m_Canvas->m_Height, DIALOG_BORDER_SIZE, ColorPixelProvider(DIALOG_BORDER_COLOR));
	}

	if (m_Title != nullptr)
	{
		Fragment titleBar(DIALOG_BORDER_SIZE, DIALOG_BORDER_SIZE, m_Canvas->m_Width - 2 * DIALOG_BORDER_SIZE, DIALOG_TITLE_HEIGHT, m_Canvas);
		if (m_TitleBackgroundPattern != nullptr)
			titleBar.fillPatern(*m_TitleBackgroundPattern);
		else
			titleBar.fillColor(DIALOG_TITLE_BK_COLOR);

		const auto& font =  DIALOG_TITLE_FONT;

		const auto title_width = min<unsigned int>(ceil(font.getStringWidth(m_Title) * DIALOG_FONT_SCALE), titleBar.m_Width);
		const auto title_height = min<unsigned int>(titleBar.m_Height, ceil(font.getStringMaxHeight(m_Title) * DIALOG_FONT_SCALE));

		titleBar.writeText(m_Title, font, ColorPixelProvider(DIALOG_TITLE_COLOR), (titleBar.m_Width - title_width) / 2, (titleBar.m_Height - title_height) / 2, DIALOG_FONT_SCALE);


		m_UserArea.UpdateCanvas(*m_Canvas);
		m_UserArea.UpdateArea(DIALOG_BORDER_SIZE,
							  DIALOG_BORDER_SIZE + DIALOG_TITLE_HEIGHT,
							  m_Width - 2 * DIALOG_BORDER_SIZE,
							  m_Height - (DIALOG_TITLE_HEIGHT + 2 * DIALOG_BORDER_SIZE));

	}
	else
	{
		m_UserArea.UpdateCanvas(*m_Canvas);
		m_UserArea.UpdateArea(DIALOG_BORDER_SIZE, DIALOG_BORDER_SIZE, m_Width - 2 * DIALOG_BORDER_SIZE, m_Height - 2 * DIALOG_BORDER_SIZE);
	}

	if (m_BackgroundPattern != nullptr)
		m_UserArea.fillPatern(*m_BackgroundPattern);
	else
		m_UserArea.fillColor(m_BackgroundColor);

	m_NeedsRedraw = false;

	return true;
}

void Dialog::drawUserAreaPart(int h, int v, int width, int height)
{
	m_UserArea.startFrameDrawing();

	for (int vi = v ; vi < v + height; ++vi)
		for (int hi = h ; hi < h + width; ++hi)
			m_UserArea.setPixel(hi, vi, m_BackgroundColor);

	if (m_BackgroundPattern != nullptr)
		m_UserArea.fillMaskPatern(*m_BackgroundPattern, m_BackgroundColor);

	m_UserArea.endFrameDrawing();
}

bool Dialog::dispatchApplicationEvent(unsigned int event, unsigned int h, unsigned int v)
{
	bool result = false;
	auto it = m_ListTail;

	//TODO: Currently it assumes only touch events are used.
	if ((GUIApp::SCREEN_TOUCH_START <= event) && (event < GUIApp::SCREEN_TOUCH_END))
	{
		if ((m_UserArea.horizontal() <= h) && ( h < m_UserArea.horizontal() + m_UserArea.m_Width)
			&& (m_UserArea.vertical() <= v) && ( v < m_UserArea.vertical() + m_UserArea.m_Height))
		{
			h -= m_UserArea.horizontal();
			v -= m_UserArea.vertical();

			while (it != nullptr)
			{
				result = it->isTouchInActiveArea(h, v)
						&& it->handleUserInteractionEvent(event, h, v);
				if (result)
					break;

				it = it->m_PrevControl;
			}

			setFocusOnControl(it);
		}
	}

	return result;
}

void Dialog::processControlEvent(Control& control, CONTROL_EVENT event)
{
	mf_ControlEventCallback(control, event);
}

Control* Dialog::setFocusOnControl(Control* ctrl)
{
	if (m_FocusedControl == ctrl)
		return m_FocusedControl;

	swap(m_FocusedControl, ctrl);

	if (ctrl != nullptr)
		ctrl->handleUserInteractionEvent(GUIApp::LOST_FOCUS, 0, 0);

	if (m_FocusedControl != nullptr)
		m_FocusedControl->handleUserInteractionEvent(GUIApp::GAIN_FOCUS, 0, 0);

	return ctrl;
}



