/*
 * ItemsModel.cpp
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */

#include "gui/ItemsModel.h"


bool ItemsModel::updateItemAt(unsigned int, void* )
{
	return false;
}

bool ItemsModel::insertItemBefore(unsigned int, void* )
{
	return false;
}


bool ItemsModel::appendItem(void*)
{
	return false;
}

bool ItemsModel::removeItem(unsigned int)
{
	return false;
}


StringsModel::StringsModel(ENTRY* entries, size_t entriesCapacity, size_t entriesUsed)
	: m_Entries(entries),
	  m_EntriesUsed(entriesUsed),
	  m_EntriesCapacity(entriesCapacity)
{
	if (entriesCapacity < entriesUsed)
		HIROS_DIE();
}

int StringsModel::count()
{
	return m_EntriesUsed;
}

const void* StringsModel::itemAt(unsigned int itemIdx)
{
	return (itemIdx < m_EntriesUsed) ? m_Entries[itemIdx] : nullptr;
}

bool StringsModel::updateItemAt(unsigned int itemIdx, void* item)
{
	if (m_EntriesUsed <= itemIdx)
		return false;

	m_Entries[itemIdx] = reinterpret_cast<USER_CHAR*>(item);
	return true;

}

bool StringsModel::insertItemBefore(unsigned int itemIdx, void* item)
{
	if (m_EntriesCapacity <= m_EntriesUsed)
		return false;

	if (m_EntriesUsed <= itemIdx)
		return false;

	for (unsigned int iterator = m_EntriesUsed++; itemIdx < iterator; --iterator)
		m_Entries[iterator] = m_Entries[iterator - 1];

	m_Entries[itemIdx] = reinterpret_cast<USER_CHAR*>(item);
	return true;
}

 bool StringsModel::appendItem(void* item)
 {
	if (m_EntriesCapacity <= m_EntriesUsed)
		return false;

	m_Entries[m_EntriesUsed++] = reinterpret_cast<USER_CHAR*>(item);
	return true;
 }

 bool StringsModel::removeItem(unsigned int itemIdx)
 {
	if (m_EntriesUsed == 0)
		return false;

	for (unsigned int iterator = itemIdx; iterator < m_EntriesUsed; ++iterator)
		m_Entries[iterator] = m_Entries[iterator + 1];

	m_EntriesUsed--;
	return true;
 }
