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


bool tui_gui_initialize()
{
	MathCalcs::InitCaches();

	return true;
}

void tui_update_screen()
{
	__tui_commit_shadow_buffer();
}
