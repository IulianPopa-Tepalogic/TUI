/*
 * GUIApp.h
 *
 *  Created on: Jun 16, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "tui_mutex.h"

class Dialog;
class DrawableCanvas;

class GUIApp {
public:
	enum Event {
		NOOP = 0,

		GAIN_FOCUS,
		LOST_FOCUS,

		SCREEN_TOUCH_START,
		TOUCH_UP = SCREEN_TOUCH_START,
		TOUCH_DOWN,
		TOUCH_CONTACT,
		TOUCH_MOVE_UP,
		TOUCH_MOVE_DOWN,
		TOUCH_MOVE_LEFT,
		TOUCH_MOVE_RIGHT,
		TOUCH_ZOOM_IN,
		TOUCH_ZOOM_OUT,

		SCREEN_TOUCH_END,

		USER_DEFINED_START = 0x1000,
		USER_1,
		USER_2
	};

	using USER_EVENT_FILTER = bool (*)(unsigned int event, int param, int param2, void* param3);

	explicit GUIApp(DrawableCanvas& canvas, Dialog& homeDialog);
	~GUIApp()  = default;

	void SetHomeDialog(Dialog& dlg) { m_HomeDlg  = &dlg; }

	void PushDialog(Dialog& dlg, int h = 0, int v = 0, int width = 0, int height = 0);
	void PopDialog();

	void SwitchToHome();

	bool draw();
	bool tickAnimations();

	void handleEvent(unsigned int event, int param = 0, int param2 = 0, void* param3 = nullptr);
	void installUserEventFilter(USER_EVENT_FILTER filter) { m_EventFilter = filter; };

protected:
	void UpdateActiveDialog(Dialog* const dialog);

	DrawableCanvas& m_Canvas;
	Dialog* m_HomeDlg;
	Dialog* m_CurrentDlg;
	USER_EVENT_FILTER m_EventFilter;

	HirosMutex m_DrawLock;
};

