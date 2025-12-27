/*
 * Dialog.h
 *
 *  Created on: Jun 16, 2020
 *      Author: IulianPopa
 */

#ifndef GUI_DIALOG_H_
#define GUI_DIALOG_H_

#include <functional>

#include "tui_mutex.h"

#include "GUIApp.h"
#include "gfx/Fragment.h"

#include "gui_theme_constants.h"
#include "gui/Control.h"

class GUIApp;

class Dialog : public ControlsHandler {
	friend class GUIApp;

	using CONTROL_CALL_BACK = void(Control& control, const CONTROL_EVENT event);

public:
	explicit Dialog(const TUI_CHAR* title);
	virtual ~Dialog() = default;

	void updateCanvas(DrawableCanvas& canvas, unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);

	Dialog& setTitle(const TUI_CHAR* title);
	Dialog& setTitleBackgroundColor(Pixel color);
	Dialog& setTitleBackgroundPattern(const Canvas* canvas);
	Dialog& setBackgroundColor(Pixel color);
	Dialog& setBackgroundPattern(const Canvas* canvas);
	Dialog& setControlEventCallBack(CONTROL_CALL_BACK cb);

	void forceRedraw();
	bool draw(bool force);
	bool tickAnimations();
	void drawUserAreaPart(int h, int v, int width, int height);

	operator const DrawableCanvas&() const { return m_UserArea; }
	void ProcessControlEvent(Control* control, const CONTROL_EVENT event);

	Control* setFocusOnControl(Control* ctrl);
	Control* focusedControl() const { return m_FocusedControl; }

	void processControlEvent(Control& control, const CONTROL_EVENT event) override;
	void promoteControl(Control& ctrl) override;
	void removeControl(Control& ctrl) override;
	void addControl(Control& ctrl) override;

protected:
	bool drawDialogFrame();
	bool dispatchApplicationEvent(unsigned int event, unsigned int arg1, unsigned int arg2);

	DrawableCanvas* m_Canvas = nullptr;
	unsigned int m_HFrom = 0;
	unsigned int m_VFrom = 0;
	unsigned int m_Width = 0;
	unsigned int m_Height = 0;

	Fragment m_UserArea;

	const TUI_CHAR* m_Title = nullptr;
	const Canvas* m_BackgroundPattern = DIALOG_TITLE_BK_PATTERN;
	const Canvas* m_TitleBackgroundPattern = DIALOG_BK_PATTERN;
	uint8_t m_BorderSize = DIALOG_BORDER_SIZE;
	Pixel m_BorderColor = DIALOG_BORDER_COLOR;
	Pixel m_TitleColor = DIALOG_TITLE_BK_COLOR;
	Pixel m_BackgroundColor = DIALOG_BK_COLOR;

	bool m_NeedsRedraw = true;

	GUIApp* m_App = nullptr;
	Dialog* m_Prev = nullptr;

	Control* m_ListHead = nullptr;
	Control* m_ListTail = nullptr;
	Control* m_FocusedControl = nullptr;

	CONTROL_CALL_BACK* mf_ControlEventCallback;
};

#endif /* GUI_DIALOG_H_ */
