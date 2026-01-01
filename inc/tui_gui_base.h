/*
 * hiros.h
 *
 *  Created on: May 15, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "tui_hwconfig.h"

bool tui_qui_initialize();
void tui_update_screen();

/*
 *  Platform dependent function!
 * This needs to be implemented according to the used platform.
 * Should not be called directly by users.
 */

void* __tui_get_draw_buffer();
void* __tui_get_shadow_buffer();
void  __tui_commit_shadow_buffer();


void __tui_warning(const char* msg = nullptr);
void __tui_error(const char* msg = nullptr);
