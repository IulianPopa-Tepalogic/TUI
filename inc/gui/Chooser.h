/*
 * Chooser.h
 *
 *  Created on: Dec 9, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gui/List.h"

class Chooser : public List {
public:
	Chooser(unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);
	virtual ~Chooser() = default;

	Chooser& setExpandedHorizontal(uint16_t h);
	Chooser& setExpandedVertical(uint16_t v);
	Chooser& setExpandedHeight(uint16_t height);
	Chooser& setExpandedWidth(uint16_t width);

	bool handleUserInteractionEvent(unsigned int event, int h, int v) override;

protected:
	void __draw(DrawableCanvas& canvas) override;
	void showExpanded(bool expanded);

	uint16_t m_ExpandedHorizontal = 0;
	uint16_t m_ExpandedVertical = 0;
	uint16_t m_ExpandedHeight = 0;
	uint16_t m_ExpandedWidth = 0;
	bool m_Expanded = false;
};
