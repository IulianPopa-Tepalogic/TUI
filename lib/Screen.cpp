#include "Screen.h"

#include <stdio.h>

#include "tui_gui_base.h"
#include "tui_resources/resources.h"
#include "gfx/PixelColorProvider.h"

using namespace std;


Screen::Screen()
	: MemoryImage(SCREEN_WIDTH_SIZE, SCREEN_HEIGHT_SIZE, nullptr)
{
	if (! sm_GUIInited)
	{
		tui_gui_initialize();
		sm_GUIInited = true;
	}

	TUI_MUTEX_INIT(&m_ScreenMutex);
}

void Screen::startFrameDrawing()
{
	if (! TUI_MUTEX_TRY_LOCK(&m_ScreenMutex, 5000)) TUI_DIE();
	m_FrameInterval = TUI_TICKS();
	m_Buffer = reinterpret_cast<Pixel*>(__tui_get_shadow_buffer());
}


void Screen::endFrameDrawing(bool flush)
{
	const uint32_t cticks = TUI_TICKS();
	m_FrameInterval = cticks - m_FrameInterval;
	TUI_MUTEX_UNLOCK (&m_ScreenMutex);

	if (flush)
		flushFrame();
}

void Screen::flushFrame()
{
	char str[24];

	if (! TUI_MUTEX_TRY_LOCK(&m_ScreenMutex, 5000)) TUI_DIE();

	if (m_showFps)
	{
		unsigned int count = TUI_TICKS_2_MS(m_FrameInterval);
		m_FrameInterval = 0;

		int fps = (count == 0) ? 1000 : (1000 / count);
		sprintf(str, "%u fps", fps);

		drawHLine(400, 18, 50, 17, Unicolor(0, 0, 0));
		writeText(str, mss8pts, Unicolor(0xFF, 0xFF, 0xFF), 405, 22);
	}

	tui_update_screen();

	TUI_MUTEX_UNLOCK(&m_ScreenMutex);
}

bool Screen::sm_GUIInited = false;
