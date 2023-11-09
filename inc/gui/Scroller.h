/*
 * Scroller.h
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gui/Control.h"
#include "gfx/Fragment.h"

class Scroller: public Control {
public:
	Scroller(unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);
	virtual ~Scroller() = default;

	Control& setWidth(unsigned int width) override;
	Control& setHeight(unsigned int height) override;

	Scroller& setVirtualWidth(unsigned int width);
	unsigned int virtualWidth() const { return m_VirtualWidth; }

	Scroller& setVirtualHeight(unsigned int height);
	unsigned int virtualHeight() const { return m_VirtualHeight; }

	Scroller& setVerticalOffset(int offset);
	int verticalOffset() const {return m_VerticalOffset; }

	Scroller& setHorizontalOffset(int offset);
	int horizontalOffset() const {return m_HorizontalOffset; }

	bool handleUserInteractionEvent(unsigned int event, int h, int v) override;


protected:
	virtual unsigned int scrollAreaWidth() const;
	virtual unsigned int scrollAreaHeight() const;

	bool isScrolling(int h, int v);
	int horizontalOvershootAlignment() const;
	int verticalOvershootAlignment() const;

	int verticalScrollbarLength() const;
	int horizontalScrollbarLength() const;

	int verticalScrollbarOffset() const;
	int horizontalScrollbarOffset() const;

	int16_t m_VerticalOffset = 0;
	int16_t m_HorizontalOffset = 0;

	int16_t m_VirtualWidth;
	int16_t m_VirtualHeight;

	int16_t m_LastHDown = -1;
	int16_t m_LastVDown = -1;

	bool m_UserScrolled = false;
};
