/*
 * Font.cpp
 *
 *  Created on: Jun 13, 2020
 *      Author: IulianPopa
 */

#include "gfx/Font.h"

#include <algorithm>

static const uint8_t spaceBitmap[16] = {0, };
static const uint8_t noCharacterBitmap[16] = {
		0b0, 0b0, 0b0, 0b11111111, 0b11000011, 0b10100101, 0b10011001, 0b10010001, 0b10011001, 0b10100101, 0b11000011, 0b11111111,
};
static const Font::Character noCharacter = { 8 /*width */, 2 /* height */, noCharacterBitmap };


using namespace std;


Font::Character Font::getCharacter(unsigned int unicode) const
{
	if (unicode == ' ')
		return {spaceWidth, 1, spaceBitmap};

	if (unicode < baseCharacter)
		return noCharacter;

	unicode -= baseCharacter;
	if (unicode >= charactersCount)
		return noCharacter;

	return {descriptors[unicode].width, descriptors[unicode].height, charactersBitmaps + descriptors[unicode].bitmapOffset};
}


unsigned int Font::getStringWidth(const USER_CHAR* text) const
{
	unsigned int result = 0;
	unsigned int charsCount = 0;
	while (*text != 0)
	{
		USER_CHAR ch = *text++;
		if (ch == ' ')
		{
			result += spaceWidth;
			continue;
		}
		else if (ch < baseCharacter)
			continue;

		ch -= baseCharacter;
		if (ch >= charactersCount)
			continue;

		++charsCount;
		result += descriptors[(unsigned int)ch].width;
	}

	return result + ((charsCount > 0) ? charsCount - 1 : 0);
}

unsigned int Font::getStringMaxHeight(const USER_CHAR* text) const
{
	unsigned int result = 0;
	while (*text != 0)
	{
		USER_CHAR ch = *text++;
		if ((ch == ' ')
			|| (ch < baseCharacter))
		{
			continue;
		}

		ch -= baseCharacter;
		if (ch >= charactersCount)
			continue;

		result = max<unsigned int>(result, descriptors[(unsigned int)ch].height);
	}

	return result;
}
