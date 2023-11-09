/*
 * hiros_gui_custom.cpp
 *
 *  Created on: May 16, 2020
 *      Author: IulianPopa
 */

#include <hiros_gui_base.h>

#include "bsp/bsp.h"
#include "tos.h"


void* implGetDrawMainBuffer()
{
	return (uint8_t*)0xC0000000;
}

void* implGetDrawWorkingBuffer()
{
	uint8_t* result = static_cast<uint8_t*>(implGetDrawMainBuffer());
	if (SCREEN_USE_DOUBLE_BUFFERING)
		result += SCREEN_HEIGHT_SIZE * SCREEN_WIDTH_SIZE * BITS_PER_PIXEL / 8;

	return result;
}

void implCommitScreen(void* const screen)
{
	const auto addr = reinterpret_cast<uint32_t>(screen);
	LTDC_L1CFBAR = addr;
	LTDC_SRCR = 2;
}


void implError(const char*)
{
	tos_critical_fault();
}
