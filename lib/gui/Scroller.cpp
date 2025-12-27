/*
 * Scroller.cpp
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */


#include "gui/Scroller.h"

#include <algorithm>
#include <cmath>

#include "gui/GUIApp.h"

using namespace std;
using namespace tui;

Scroller::Scroller(unsigned int h, unsigned int v, unsigned int width, unsigned int height)
	: Control(h, v, width, height),
	  m_VirtualWidth(m_Width),
	  m_VirtualHeight(m_Height)
{
}

Control& Scroller::setWidth(unsigned int width)
{
	Control::setWidth(width);
	m_VirtualWidth = max<unsigned int>(m_VirtualHeight, width);
	m_VirtualWidth = max<unsigned int>(m_VirtualWidth, scrollAreaWidth());

	return *this;
}

Control& Scroller::setHeight(unsigned int height)
{
	Control::setHeight(height);
	m_VirtualHeight = max<unsigned int>(m_VirtualHeight, height);
	m_VirtualHeight = max<unsigned int>(m_VirtualHeight, scrollAreaHeight());

	return *this;
}


Scroller& Scroller::setVirtualWidth(unsigned int width)
{
	const auto newValue = max<int>(scrollAreaWidth(), width);
	if (newValue != m_VirtualWidth)
	{
		m_VirtualWidth = newValue;
		setHorizontalOffset(horizontalOvershootAlignment());
		markDirty();
	}

	return *this;
}

Scroller& Scroller::setVirtualHeight(unsigned int height)
{
	const auto newValue = max<int>(scrollAreaHeight(), height);
	if (newValue != m_VirtualHeight)
	{
		m_VirtualHeight = newValue;
		setVerticalOffset(verticalOvershootAlignment());
		markDirty();
	}

	return *this;
}



Scroller& Scroller::setVerticalOffset(int offset)
{
	if (m_VirtualHeight <= scrollAreaHeight())
		return *this;

	offset = max<int>(-scrollAreaHeight(), offset);
	offset = min<int>(offset, m_VirtualHeight);

	if (offset != m_VerticalOffset)
	{
		m_VerticalOffset = offset;
		markDirty();
	}

	return *this;
}

Scroller& Scroller::setHorizontalOffset(int offset)
{
	if (m_VirtualWidth <= scrollAreaWidth())
		return *this;

	offset = max<int>(-scrollAreaWidth(), offset);
	offset = min<int>(offset, m_VirtualWidth);

	if (offset != m_HorizontalOffset)
	{
		m_HorizontalOffset = offset;
		markDirty();
	}

	return *this;
}

bool Scroller::handleUserInteractionEvent(unsigned int event, int h, int v)
{
	const auto prevResult = Control::handleUserInteractionEvent(event, h, v);

	switch (event)
	{
	case GUIApp::TOUCH_CONTACT:
	case GUIApp::TOUCH_DOWN:
		if (m_LastHDown < 0)
		{
			m_LastHDown = h;
			m_LastVDown = v;
		}

		if (isScrolling(h, v))
		{
			setVerticalOffset(verticalOffset() - (v - m_LastVDown));
			setHorizontalOffset(horizontalOffset() - (h - m_LastHDown));

			m_LastHDown = h;
			m_LastVDown = v;
			m_UserScrolled = true;
		}
		break;

	case GUIApp::LOST_FOCUS:
	case GUIApp::TOUCH_UP:
		m_LastVDown = m_LastHDown = -1;
		m_UserScrolled = false;

		setHorizontalOffset(horizontalOvershootAlignment());
		setVerticalOffset(verticalOvershootAlignment());

		break;

	default:
		return prevResult;
	}

	return true;
}

int Scroller::scrollAreaWidth() const
{
	return 0;
}

int Scroller::scrollAreaHeight() const
{
	return 0;
}

bool Scroller::isScrolling(int h, int v)
{
	return ((abs(h - m_LastHDown) > 5) || (abs(v - m_LastVDown) > 5));
}

int Scroller::horizontalOvershootAlignment() const
{
	if (m_HorizontalOffset < 0)
		return 0;

	if (m_VirtualWidth - m_HorizontalOffset < (int)scrollAreaWidth())
		return max<int>(m_VirtualWidth - scrollAreaWidth(), 0);

	return m_HorizontalOffset;
}

int Scroller::verticalOvershootAlignment() const
{
	if (m_VerticalOffset < 0)
		return 0;

	if (m_VirtualHeight - m_VerticalOffset < (int)scrollAreaHeight())
		return max<int>(m_VirtualHeight - scrollAreaHeight(), 0);

	return m_VerticalOffset;
}

int Scroller::horizontalScrollbarLength() const
{
	float sizeFact = virtualWidth();
	sizeFact /= scrollAreaWidth();

	return ceil(scrollAreaWidth() / sizeFact);
}


int Scroller::verticalScrollbarLength() const
{
	float sizeFact = virtualHeight();
	sizeFact /= scrollAreaHeight();

	return ceil(scrollAreaHeight() / sizeFact);
}


int Scroller::horizontalScrollbarOffset() const
{
	const int width = virtualWidth();

	const auto horizontalOffset = min<int>(max<int>(m_HorizontalOffset, -width), width);
	const float raport = (float)horizontalOffset / width;

	return raport * scrollAreaWidth();
}

int Scroller::verticalScrollbarOffset() const
{
	const int height = virtualHeight();

	const auto verticalOffset = min<int>(max<int>(m_VerticalOffset, -height), height);
	const float raport = (float)verticalOffset / height;

	return raport * scrollAreaHeight();
}


