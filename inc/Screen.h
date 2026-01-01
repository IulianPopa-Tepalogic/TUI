#pragma once

#include <gfx/MemoryImage.h>


class Screen : public MemoryImage
{
public:
	explicit Screen();
	virtual ~Screen() = default;

	void startFrameDrawing();
	void endFrameDrawing(bool flush);
	void flushFrame();

protected:
	TUI_MUTEX m_ScreenMutex;
	uint32_t  m_FrameInterval = 0;

	static bool sm_GUIInited;

public:
	bool m_showFps = false;
};
