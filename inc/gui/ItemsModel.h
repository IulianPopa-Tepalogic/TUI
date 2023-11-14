/*
 * ItemsModel.h
 *
 *  Created on: Nov 14, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "hiros_hwconfig.h"

class ItemsModel {
public:
	ItemsModel() = default;
	virtual ~ItemsModel() = default;

	virtual int count() = 0;
	virtual const void* itemAt(unsigned int itemIdx) = 0;
	virtual bool updateItemAt(unsigned int itemIdx, void* item);
	virtual bool insertItemBefore(unsigned int itemIdx, void* item);
	virtual bool appendItem(void* item);
	virtual bool removeItem(unsigned int itemIdx);
};


class StringsModel : public ItemsModel {
public:
	using ENTRY = USER_CHAR*;

	StringsModel(ENTRY* entries, size_t entriesCapacity, size_t entriesUsed = 0);
	virtual ~StringsModel() = default;

	int count() override;
	const void* itemAt(unsigned int itemIdx) override;
	bool updateItemAt(unsigned int itemIdx, void* item) override;
	bool insertItemBefore(unsigned int itemIdx, void* item) override;
	bool appendItem(void* item) override;
	bool removeItem(unsigned int itemIdx) override;

protected:
	ENTRY* const m_Entries;
	uint16_t m_EntriesUsed;
	const uint16_t m_EntriesCapacity;
};
