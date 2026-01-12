#pragma once

#include <cstdint>

namespace tui {

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

static constexpr uint16_t SCREEN_WIDTH_SIZE  = 480;
static constexpr uint16_t SCREEN_HEIGHT_SIZE = 272;
static constexpr bool 	  SCREEN_USE_DOUBLE_BUFFERING = true;
static constexpr uint8_t  AA_SAMPLING = 4;

static constexpr bool 	  USE_SIN_CACHE = false;
static constexpr bool 	  USE_COS_CACHE = false;

}

#define USED_COLOR_ENCODING RGB565

#define __PixelPaste(e) Pixel_ ## e
#define __PixelEval(e) __PixelPaste(e)
#define Pixel __PixelEval(USED_COLOR_ENCODING)

static constexpr tui::PIXEL_COLOR_ENCODINGS tui::HW_COLOR_ENCODE = USED_COLOR_ENCODING;

typedef char TUI_CHAR;

#define TUI_MUTEX mutex
#define TUI_MUTEX_LOCK(m) lock(m)
#define TUI_MUTEX_TRY_LOCK(m, msTmo) try_lock(m, tos_ms2ticks(msTmo))
#define TUI_MUTEX_UNLOCK(m) unlock(m)

#define TUI_TICKS() ticks()
#define TUI_TICKS_2_MS(t) ticks2ms(t)
#define TUI_MS_2_TICKS(m) ms2ticks(m)

#define TUI_DIE(x) __tui_error(x)
