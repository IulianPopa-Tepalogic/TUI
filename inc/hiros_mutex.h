/*
 * hiros_mutex.c
 *
 *  Created on: Aug 7, 2020
 *      Author: IulianPopa
 */

#ifndef HIROS_MUTEX_H_
#define HIROS_MUTEX_H_

#include "hiros_hwconfig.h"

class HirosMutex {

public:
	void lock() { HIROS_MUTEX_LOCK(&m_Mutex); }
	void unlock() { HIROS_MUTEX_UNLOCK(&m_Mutex); }

protected:
	HIROS_MUTEX m_Mutex;
};


#endif /* HIROS_MUTEX_C_ */
