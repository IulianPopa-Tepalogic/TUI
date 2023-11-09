/*
 * GUIApp.cpp
 *
 *  Created on: Jun 16, 2020
 *      Author: IulianPopa
 */

#include "gui/GUIApp.h"

#include <mutex>

#include "gui/Dialog.h"

#include "gfx/DrawableCanvas.h"


using namespace std;

static bool default_event_filter(unsigned int event, int, int, void*)
{
	return (event == GUIApp::NOOP) ? true : false;
}

GUIApp::GUIApp(DrawableCanvas& canvas, Dialog& homeDialog)
	: m_Canvas(canvas),
	  m_HomeDlg(&homeDialog),
	  m_CurrentDlg(&homeDialog),
	  m_EventFilter(default_event_filter)
{
	m_CurrentDlg->updateCanvas(m_Canvas);
}


void GUIApp::PushDialog(Dialog& dlg, int h, int v, int width, int height)
{
	dlg.updateCanvas(m_Canvas, h, v, width, height);
	dlg.m_Prev = m_CurrentDlg;

	m_CurrentDlg = &dlg;
	UpdateActiveDialog(m_CurrentDlg);
}

void GUIApp::PopDialog()
{
	Dialog *dlg = nullptr;
	if (m_CurrentDlg != nullptr)
	{
		dlg = m_CurrentDlg->m_Prev;
		m_CurrentDlg->m_Prev = nullptr;
	}
	UpdateActiveDialog(dlg);
}

void GUIApp::SwitchToHome()
{
	UpdateActiveDialog(nullptr);
}


bool GUIApp::draw()
{
	lock_guard<HirosMutex> guard(m_DrawLock);

	m_Canvas.startFrameDrawing();

	if (m_CurrentDlg != nullptr)
		m_CurrentDlg->draw(false);

	m_Canvas.endFrameDrawing();

	return true;
}

bool GUIApp::tickAnimations()
{
	lock_guard<HirosMutex> guard(m_DrawLock);
	if (m_CurrentDlg != nullptr)
		return m_CurrentDlg->tickAnimations();

	return false;
}

void GUIApp::handleEvent(unsigned int event, int param, int param2, void* param3)
{
	if (m_EventFilter(event, param, param2, param3))
		return ;

	if ((SCREEN_TOUCH_START <= event) && (event <= SCREEN_TOUCH_END))
	{
		if (m_CurrentDlg != nullptr)
			m_CurrentDlg->dispatchApplicationEvent(event, param, param2);

		return;
	}
}


void GUIApp::UpdateActiveDialog(Dialog* dialog)
{
	m_Canvas.fillColor(APP_BK_COLOR);

	dialog = (dialog != nullptr) ? dialog : m_HomeDlg;
	if (dialog == nullptr)
		return ;

	dialog->m_NeedsRedraw = true;
}
