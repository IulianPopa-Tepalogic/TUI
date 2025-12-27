/*
 * TUI_screen.cpp
 *
 *  Created on: May 15, 2020
 *      Author: IulianPopa
 */

#include <algorithm>
#include <memory.h>
#include "TUI_gui_base.h"
#include "MathCalcs.h"

using namespace std;

static uint8_t* sgMainBuffer;
static uint8_t* sgWorkBuffer;

bool TUI_qui_initialize()
{
	MathCalcs::InitCaches();

	sgMainBuffer = static_cast<uint8_t*>(implGetDrawMainBuffer());
	sgWorkBuffer = static_cast<uint8_t*>(implGetDrawWorkingBuffer());

	return (sgMainBuffer != nullptr)
			& (sgWorkBuffer != nullptr);
}

void updateScreen()
{
	memcpy(sgMainBuffer, sgWorkBuffer, SCREEN_HEIGHT_SIZE * SCREEN_WIDTH_SIZE * (Pixel::BITS_PER_PIXEL / 8));
	implCommitScreen(sgMainBuffer);
}

void* getWorkingBuffer()
{
	return sgWorkBuffer;
}


