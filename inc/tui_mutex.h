/*
 * tui_mutex.c
 *
 *  Created on: Aug 7, 2020
 *      Author: IulianPopa
 */

#ifndef TUI_MUTEX_H_
#define TUI_MUTEX_H_

#include "tui_hwconfig.h"

class HirosMutex {

public:
	void lock() { TUI_MUTEX_LOCK(&m_Mutex); }
	void unlock() { TUI_MUTEX_UNLOCK(&m_Mutex); }

protected:
	TUI_MUTEX m_Mutex;
};


#endif
