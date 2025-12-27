/*
 * ControllerItemDrawer.cpp
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */

#include <gui/ItemsIllustrator.h>

#include "gfx/Rectangle.h"
#include "gui_theme_constants.h"

using namespace std;
using namespace tui;

unsigned int ItemsIllustrator::getRecommendedWidth(unsigned int itemIdx, Role role, const void* item)
{
	return 0;
}

unsigned int ItemsIllustrator::getRecommendedHeight(unsigned int itemIdx, Role role, const void* item)
{
	return 0;
}

static float getUsedFontSize(ItemsIllustrator::Role role)
{
	return (role == ItemsIllustrator::REGULAR_ITEM)
		   ? LIST_ITEM_TEXT_SIZE
		   : (role == ItemsIllustrator::SELECTED_ITEM)
			 ? LIST_SELECTED_ITEM_TEXT_SIZE
			 : (role == ItemsIllustrator::CHOOSED_ITEM) ? CHOOSER_ITEM_TEXT_SIZE : LIST_HEADER_ITEM_TEXT_SIZE;
}

static Font& getUsedFont(ItemsIllustrator::Role role)
{
	return (role == ItemsIllustrator::REGULAR_ITEM)
		   ? LIST_ITEM_TEXT_FONT
		   : (role == ItemsIllustrator::SELECTED_ITEM)
			 ? LIST_SELECTED_ITEM_TEXT_FONT
			 : (role == ItemsIllustrator::CHOOSED_ITEM) ? CHOOSER_ITEM_TEXT_FONT : LIST_HEADER_ITEM_TEXT_FONT;
}


unsigned int TextItemsIllustrator::getRecommendedWidth(unsigned int itemIdx, Role role, const void* item)
{
	if (item == nullptr)
		return 0;

	auto text = reinterpret_cast<const TUI_CHAR*>(item);
	return getUsedFont(role).getStringWidth(text) * getUsedFontSize(role);
}

unsigned int TextItemsIllustrator::getRecommendedHeight(unsigned int itemIdx, Role role, const void* item)
{
	return item == nullptr ? 0 : (getUsedFont(role).getStringMaxHeight("I") * getUsedFontSize(role));
}

void TextItemsIllustrator::draw(DrawableCanvas& area, unsigned int itemIdx, Role role, const void* item)
{
	const Pixel bkColor = (role == REGULAR_ITEM)
						  ? LIST_ITEM_BK_COLOR
						  : (role == SELECTED_ITEM)
							? LIST_SELECTED_ITEM_BK_COLOR
							: (role == CHOOSED_ITEM) ? CHOOSER_ITEM_BK_COLOR : LIST_HEADER_ITEM_BK_COLOR;

	Rectangle rect(0, 0, area.m_Width, area.m_Height);
	rect.fill(area, Unicolor(bkColor));

	if (item == nullptr)
		return ;

	auto text = reinterpret_cast<const TUI_CHAR*>(item);

	const auto fontSize = getUsedFontSize(role);
	const auto& font = getUsedFont(role);

	int htext = 0;
	if (m_AlignHorizontally == RIGHT)
		htext = area.m_Width - font.getStringWidth(text) * fontSize;
	else if (m_AlignHorizontally == HCENTER)
		htext = (area.m_Width - font.getStringWidth(text) * fontSize) / 2;

	int vtext = 0;
	if (m_AlignVertically == BOTTOM)
		vtext = area.m_Height - font.getStringMaxHeight(text) * fontSize;
	else if (m_AlignVertically == VCENTER)
		vtext = (area.m_Height - font.getStringMaxHeight(text) * fontSize) / 2;

	const Pixel color = (role == REGULAR_ITEM)
						 ? LIST_ITEM_COLOR
						 : (role == SELECTED_ITEM)
						   ? LIST_SELECTED_ITEM_COLOR
						   : (role == CHOOSED_ITEM) ? CHOOSER_ITEM_COLOR : LIST_HEADER_ITEM_COLOR;
	area.writeText(text, font, Unicolor(color), htext, vtext, fontSize, false);
}
