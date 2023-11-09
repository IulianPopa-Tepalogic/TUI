/*
 * ControllerItemDrawer.h
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gfx/Fragment.h"

class ItemsIllustrator {
public:
	enum Role {
		REGULAR_ITEM,
		SELECTED_ITEM,
		HEADER_ITEM,
		CHOOSED_ITEM,
	};

	ItemsIllustrator() = default;
	virtual ~ItemsIllustrator() = default;

	virtual unsigned int getRecommendedWidth(unsigned int itemIdx, Role role, const void* item);
	virtual unsigned int getRecommendedHeight(unsigned int itemIdx, Role role, const void* item);

	virtual void draw(DrawableCanvas& area, unsigned int itemIdx, Role role, const void* item) = 0;
};


class TextItemsIllustrator : public ItemsIllustrator {
public:
	enum HorizontalAlignment {
		LEFT,
		HCENTER,
		RIGHT
	};

	enum VerticalAlignment {
		TOP,
		VCENTER,
		BOTTOM
	};

	TextItemsIllustrator() = default;
	~TextItemsIllustrator() = default;

	unsigned int getRecommendedWidth(unsigned int itemIdx, Role role, const void* item) override;
	unsigned int getRecommendedHeight(unsigned int itemIdx, Role role, const void* item) override;

	void draw(DrawableCanvas& area, unsigned int itemIdx, Role role, const void* item) override;

	HorizontalAlignment m_AlignHorizontally = LEFT;
	VerticalAlignment m_AlignVertically = VCENTER;
};

