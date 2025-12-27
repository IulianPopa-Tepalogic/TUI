/*
 * List.cpp
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */

#include "gui/List.h"

#include <algorithm>

#include "gui_theme_constants.h"
#include "gui/GUIApp.h"
#include "gfx/RoundRectangle.h"

using namespace std;
using namespace tui;

List::List(bool vertical, unsigned int h, unsigned int v, unsigned int width, unsigned int height, unsigned int spacing)
	: Scroller (h, v, width, height),
	  m_Spacing(spacing),
	  m_Vertical(vertical)
{
}


Control& List::setBorderSize(unsigned int size)
{
	Control::setBorderSize(size);

	//Force a refresh of these when drawing
	m_VirtualWidth = 0;
	m_VirtualHeight = 0;

	return *this;
}

List& List::attachModel(ItemsModel& model)
{
	m_Model = &model;
	markDirty();

	return *this;
}

List& List::attachIllustrator(ItemsIllustrator& illustrator)
{
	m_Illustrator = &illustrator;
	markDirty();

	return *this;
}

bool List::selectItem(int index)
{
	if ((m_Model == nullptr) || (m_Model->count() <= index))
		return false;

	if (m_SelectedItem != index)
	{
		m_SelectedItem = index;
		markDirty();
	}
	return true;
}

bool List::setHeader(const void* entry)
{
	m_Header = entry;
	markDirty();

	return true;
}

bool List::handleUserInteractionEvent(unsigned int event, int h, int v)
{
	if ((event == GUIApp::TOUCH_UP)
		&& ! m_UserScrolled)
	{
		selectItem(retrieveSelectedItem(h - horizontal(), v - vertical()));
		if (m_EventsHandler != nullptr)
			m_EventsHandler->processControlEvent(*this, SELECTION_UPDATED);
	}

	return Scroller::handleUserInteractionEvent(event, h, v);
}

int List::scrollAreaWidth() const
{
	return m_ItemsArea.m_Width;
}


int List::scrollAreaHeight() const
{
	return m_ItemsArea.m_Height;
}


void List::__draw(DrawableCanvas& canvas)
{
	if ((m_Illustrator == nullptr) || (m_Model == nullptr))
		return ;

	const int itemsCount = m_Model->count();

	canvas.fillColor(LIST_BACKGROUND_COLOR);

	const auto headerHeight = m_Illustrator->getRecommendedHeight(0, ItemsIllustrator::HEADER_ITEM, m_Header);

	m_ItemsArea.UpdateCanvas(canvas);
	m_ItemsArea.UpdateArea(m_BorderSize, m_BorderSize, canvas.m_Width - m_BorderSize, headerHeight);

	m_Illustrator->draw(m_ItemsArea, 0, ItemsIllustrator::HEADER_ITEM, m_Header);

	m_ItemsArea.UpdateArea(m_BorderSize, m_BorderSize + headerHeight, canvas.m_Width - 2 * m_BorderSize, canvas.m_Height - (2 * m_BorderSize + headerHeight));

	refreshVirtualDimensions();

	if (m_Vertical)
	{
		for (int itemIdx = 0, v = 0; itemIdx < itemsCount; ++itemIdx)
		{
			auto item = m_Model->itemAt(itemIdx);
			const auto role = (itemIdx == m_SelectedItem) ? ItemsIllustrator::SELECTED_ITEM : ItemsIllustrator::REGULAR_ITEM;
			const int itemHeight = m_Illustrator->getRecommendedHeight(itemIdx, role, item);

			if (v + itemHeight < m_VerticalOffset)
				v += itemHeight + m_Spacing;

			else if (v <= (m_VerticalOffset + m_ItemsArea.m_Height))
			{
				Fragment itemArea(-m_HorizontalOffset, v - m_VerticalOffset, scrollAreaWidth(), itemHeight, &m_ItemsArea);
				m_Illustrator->draw(itemArea, itemIdx, role, item);
				v += itemHeight + m_Spacing;
			}
			else
				break;
		}
	}
	else
	{
		for (int itemIdx = 0, h = 0; itemIdx < itemsCount; ++itemIdx)
		{
			auto item = m_Model->itemAt(itemIdx);
			const auto role = (itemIdx == m_SelectedItem) ? ItemsIllustrator::SELECTED_ITEM : ItemsIllustrator::REGULAR_ITEM;
			const int itemWidth = m_Illustrator->getRecommendedWidth(itemIdx, role, item);

			if (h + itemWidth < m_HorizontalOffset)
				h += itemWidth + m_Spacing;

			else if (h <= (m_HorizontalOffset + m_ItemsArea.m_Width))
			{
				Fragment itemArea(h - m_HorizontalOffset,  -m_VerticalOffset, itemWidth, scrollAreaHeight(), &m_ItemsArea);
				m_Illustrator->draw(itemArea, itemIdx, role, item);
				h += itemWidth + m_Spacing;
			}
			else
				break;
		}
	}

	if (scrollAreaHeight() < virtualHeight())
	{
		const auto barLength = verticalScrollbarLength();
		const auto barPos = verticalScrollbarOffset();

		RoundRectangle rr (scrollAreaWidth() - (SCROLLER_SCROLLBAR_WIDTH + SCROLLER_SCROLLBAR_OFFSET),
						   barPos,
						   SCROLLER_SCROLLBAR_WIDTH,
						   barLength,
						   SCROLLER_SCROLLBAR_RADIUS);
		rr.fill(m_ItemsArea, Unicolor(SCROLLER_SCROLLBAR_COLOR));
	}

	if (scrollAreaWidth() < virtualWidth())
	{
		const auto barLength = horizontalScrollbarLength();
		const auto barPos = horizontalScrollbarOffset();

		RoundRectangle rr (barPos,
						   scrollAreaHeight() - (SCROLLER_SCROLLBAR_WIDTH + SCROLLER_SCROLLBAR_OFFSET),
						   barLength,
						   SCROLLER_SCROLLBAR_WIDTH,
						   SCROLLER_SCROLLBAR_RADIUS);
		rr.fill(m_ItemsArea, Unicolor(SCROLLER_SCROLLBAR_COLOR));
	}


	if (m_BorderSize > 0)
	{
		RoundRectangle rr(0, 0, canvas.m_Width, canvas.m_Height, m_BorderRadius);
		rr.drawContour(canvas, Unicolor(m_BorderColor), m_BorderSize);
	}
}


void List::refreshVirtualDimensions()
{
	uint16_t width = 0, height = 0;

	int itemsCount = (m_Model == nullptr ? 0 : m_Model->count());
	if (m_Vertical)
	{
		for (int itemIdx = 0; itemIdx < itemsCount; ++itemIdx)
		{
			const auto role = (itemIdx == m_SelectedItem) ? ItemsIllustrator::SELECTED_ITEM : ItemsIllustrator::REGULAR_ITEM;
			height += (m_Illustrator != nullptr)
					    ? m_Illustrator->getRecommendedHeight(itemIdx, role,  m_Model->itemAt(itemIdx))
					    : 0;
			height += m_Spacing;
		}
	}
	else
	{
		for (int itemIdx = 0; itemIdx < itemsCount; ++itemIdx)
		{
			const auto role = (itemIdx == m_SelectedItem) ? ItemsIllustrator::SELECTED_ITEM : ItemsIllustrator::REGULAR_ITEM;
			width += (m_Illustrator != nullptr)
					   ? m_Illustrator->getRecommendedWidth(itemIdx, role, m_Model->itemAt(itemIdx))
					   : 0;
			width += m_Spacing;
		}
	}

	setVirtualWidth(width);
	setVirtualHeight(height);
}

int List::retrieveSelectedItem(int h, int v) const
{
	if (m_Model == nullptr || m_Illustrator == nullptr)
		return -1;

	const int itemsCount = m_Model->count();
	if (m_Vertical)
	{
		const int realV = m_VerticalOffset + v - m_ItemsArea.vertical();
		int iteratedHeight = 0;
		for (int sel = 0; sel < itemsCount; ++sel)
		{
			const auto role = (sel == m_SelectedItem) ? ItemsIllustrator::SELECTED_ITEM : ItemsIllustrator::REGULAR_ITEM;
			const int itemHeight = m_Illustrator->getRecommendedHeight(sel, role, m_Model->itemAt(sel));
			if ((iteratedHeight <= realV) && (realV <= iteratedHeight + itemHeight))
				return sel;

			iteratedHeight += itemHeight + m_Spacing;
		}
	}
	else
	{
		const int realH = m_HorizontalOffset + h - m_ItemsArea.horizontal();
		int iteratedWidth = 0;
		for (int sel = 0; sel < itemsCount; ++sel)
		{
			const auto role = (sel == m_SelectedItem) ? ItemsIllustrator::SELECTED_ITEM : ItemsIllustrator::REGULAR_ITEM;
			const int itemWidth = m_Illustrator->getRecommendedWidth(sel, role, m_Model->itemAt(sel));

			if ((iteratedWidth <= realH) && (realH <= iteratedWidth + itemWidth))
				return sel;

			iteratedWidth += itemWidth + m_Spacing;
		}
	}

	return -1;
}
