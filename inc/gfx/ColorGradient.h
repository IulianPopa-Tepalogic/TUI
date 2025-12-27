/*
 * ColorGradient.h
 *
 *  Created on: May 22, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "tui_gui_base.h"

#include <vector>
#include <algorithm>

class ColorGradient {
public:
	constexpr ColorGradient()
	{}

	constexpr ColorGradient(Pixel from, Pixel to)
		: m_From(from),
		  m_To(to)
	{}

	~ColorGradient() = default;

	Pixel Interpolate(int offset, int wide) const;

	Pixel m_From;
	Pixel m_To;
};
