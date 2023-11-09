/*
 * Font.h
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#pragma once

#include <stdint.h>

#include "hiros_hwconfig.h"

struct Font {
public:
	struct CharacterDescriptor {
		const uint8_t width; 			//in pixels
		const uint8_t height;			//in pixels
		const uint16_t bitmapOffset;	//bitmap offset
	};

	struct Character {
		const uint8_t width;	//in pixels
		const uint8_t height;	//in pixels
		const uint8_t* const data;
	};

	const uint32_t baseCharacter;
	const uint16_t charactersCount;
	const uint8_t spaceWidth;
	const CharacterDescriptor* const descriptors;
	const uint8_t *const charactersBitmaps;

	Character getCharacter(unsigned int unicode) const;
	unsigned int getStringWidth(const USER_CHAR* text) const;
	unsigned int getStringMaxHeight(const USER_CHAR* text) const;
};
