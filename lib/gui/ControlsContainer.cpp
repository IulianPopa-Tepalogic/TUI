/*
 * ControlsContainer.cpp
 *
 *  Created on: Nov 5, 2020
 *      Author: IulianPopa
 */

#include <gui/ControlsContainers.h>

using namespace std;
using namespace tui;

void ControlsContainerModel::draw(DrawableCanvas& canvas, int width, int height)
{
	if ((width != m_CacheCanvasHeight) || (height != m_CacheCanvasWidth))
		cacheCanvasSize(width, height);

	m_CacheCanvasHeight = height;
	m_CacheCanvasWidth = width;

	int i = controlsCount();
	while (--i >= 0)
	{
		if (! drawControl(canvas,i))
			break;
	}
}

void ControlsContainerModel::notifyModelUpdate()
{
	m_CacheCanvasHeight = m_CacheCanvasWidth = -1;

	if (m_AssociatedContainer != nullptr)
		m_AssociatedContainer->modelUpdated();
}


ControlsContainer::ControlsContainer(ControlsContainerModel* model, unsigned int h, unsigned int v, unsigned int width, unsigned int height)
	: Control(h, v, width, height)
{
	setModel(model);
}



void ControlsContainer::processControlEvent(Control& control, const CONTROL_EVENT event)
{
	TUI_DIE();
}


void ControlsContainer::promoteControl(Control& control)
{
	TUI_DIE();
}

void ControlsContainer::removeControl(Control& control)
{
	TUI_DIE();
}

void ControlsContainer::addControl(Control& control)
{
	TUI_DIE();
}


void ControlsContainer::setModel(ControlsContainerModel* model)
{
	if (m_ControlModel != nullptr)
		m_ControlModel->m_AssociatedContainer = nullptr;

	if (model->m_AssociatedContainer != nullptr)
		TUI_DIE();

	model->m_AssociatedContainer = this;
	modelUpdated();
}





void ControlsContainer::modelUpdated()
{
	markDirty();
}

