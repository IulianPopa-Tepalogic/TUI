#pragma once

#include <cstdint>

#include "tos.h"

enum PIXEL_COLOR_ENCODINGS {
	UNDEFINED = 0,
	RGB888,
	RGB565,
};

struct Pixel_RGB888 {
	constexpr Pixel_RGB888()
		: blue(0), green(0), red(0)
	{}

	constexpr Pixel_RGB888 (uint8_t r, uint8_t g, uint8_t b)
		: blue(b), green(g), red(r)
	{}

	bool operator==(const Pixel_RGB888& p) const { return (blue == p.blue) & (green == p.green) & (red == p.red); }
	bool operator!=(const Pixel_RGB888& p) const { return !(*this == p); }

	uint8_t blue;
	uint8_t green;
	uint8_t red;

	static constexpr uint8_t alpha = 0xFF;
	static constexpr uint8_t BITS_PER_PIXEL = 24;
};

struct Pixel_RGB565 {
	constexpr Pixel_RGB565()
		: blue(0), green(0), red(0)
	{}

	constexpr Pixel_RGB565 (uint8_t r, uint8_t g, uint8_t b)
		: blue(b), green(g), red(r)
	{}

	bool operator==(const Pixel_RGB565& p) const { return *((uint16_t*)this) == *((uint16_t*)&p); }
	bool operator!=(const Pixel_RGB565& p) const { return !(*this == p); }

	uint16_t blue: 5;
	uint16_t green: 6;
	uint16_t red: 5;

	static constexpr uint8_t alpha = 0xFF;
	static constexpr uint8_t BITS_PER_PIXEL = 16;
};

#define USED_COLOR_ENCODING RGB565

#define __PixelPaste(e) Pixel_ ## e
#define __PixelEval(e) __PixelPaste(e)
#define Pixel __PixelEval(USED_COLOR_ENCODING)

static constexpr uint16_t SCREEN_WIDTH_SIZE = 480;
static constexpr uint16_t SCREEN_HEIGHT_SIZE = 272;
static constexpr bool SCREEN_USE_DOUBLE_BUFFERING = true;

static constexpr PIXEL_COLOR_ENCODINGS HW_COLOR_ENCODE = USED_COLOR_ENCODING;

static constexpr bool USE_SIN_CACHE = false;
static constexpr bool USE_COS_CACHE = false;

typedef char USER_CHAR;

#define HIROS_MUTEX TOS_MUTEX
#define HIROS_MUTEX_LOCK(m) tos_lock_mutex(m)
#define HIROS_MUTEX_TRY_LOCK(m, msTmo) tos_try_lock_mutex(m, tos_ms2ticks(msTmo))
#define HIROS_MUTEX_UNLOCK(m) tos_unlock_mutex(m)


#define HIROS_TICKS() tos_ticks()
#define HIROS_TICKS_2_MS(t) tos_ticks2ms(t)
#define HIROS_MS_2_TICKS(m) tos_ms2ticks(m)

#define HIROS_DIE() tos_critical_fault()

