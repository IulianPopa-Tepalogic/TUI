/*
 * List.h
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gui/Scroller.h"
#include "gui/ItemsModel.h"
#include "gui/ItemsIllustrator.h"

class List: public Scroller {
public:
	List(bool vertical = true, unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0, unsigned int spacing = 0);
	virtual ~List() = default;

	Control& setBorderSize(unsigned int size) override;

	List& attachModel(ItemsModel& model);
	List& attachIllustrator(ItemsIllustrator& illustrator);

	int selectedItem() const { return m_SelectedItem; }
	bool selectItem(int index);

	bool setHeader(const void* entry);

	bool handleUserInteractionEvent(unsigned int event, int h, int v) override;

protected:
	unsigned int scrollAreaWidth() const override;
	unsigned int scrollAreaHeight() const override;

	void __draw(DrawableCanvas& canvas) override;
	void refreshVirtualDimensions();
	int retrieveSelectedItem(int h, int v) const;

	ItemsModel* m_Model = nullptr;
	ItemsIllustrator* m_Illustrator = nullptr;
	const void *m_Header = nullptr;

	Fragment m_ItemsArea;

	int16_t m_SelectedItem = -1;
	const uint8_t m_Spacing;
	const bool m_Vertical;
};

