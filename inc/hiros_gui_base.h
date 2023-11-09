/*
 * hiros.h
 *
 *  Created on: May 15, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "hiros_hwconfig.h"

bool hiros_qui_initialize();


void updateScreen();
void* getWorkingBuffer();
/*
 *  Platform dependent function!
 * This needs to be implemented according to the used platform.
 * Should not be called directly by users.
 */

void implSignalStartFrameDrawing();
void implSignalEndFrameDrawing();
void implSignalFrameConsumed();

void* implGetDrawMainBuffer();
void* implGetDrawWorkingBuffer();
void implCommitScreen(void* screen);


void implWarning(const char* msg = nullptr);
void implError(const char* msg = nullptr);
