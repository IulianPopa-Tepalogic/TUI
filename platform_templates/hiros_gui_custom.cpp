/*
 * TUI_gui_custom.cpp
 *
 *  Created on: May 16, 2020
 *      Author: IulianPopa
 */

#include <TUI_gui_base.h>

#include "bsp/bsp.h"
#include "tos.h"


void* __tui_get_draw_buffer()
{
	return (uint8_t*)0xC0000000;
}

void* __tui_get_shadow_buffer()
{
	uint8_t* result = static_cast<uint8_t*>(__tui_get_draw_buffer());
	if (SCREEN_USE_DOUBLE_BUFFERING)
		result += SCREEN_HEIGHT_SIZE * SCREEN_WIDTH_SIZE * BITS_PER_PIXEL / 8;

	return result;
}

void __tui_commit_shadow_buffer(void* const screen)
{
	const auto addr = reinterpret_cast<uint32_t>(screen);
	LTDC_L1CFBAR = addr;
	LTDC_SRCR = 2;
}


void __tui_error(const char*)
{
	tos_critical_fault();
}
