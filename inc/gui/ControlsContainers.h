/*
 * ViewScroller.h
 *
 *  Created on: Nov 5, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gui/Control.h"

class ControlsContainer;

class ControlsContainerModel {
	friend class ControlsContainer;
public:
	ControlsContainerModel() = default;
	virtual ~ControlsContainerModel() = default;

	virtual int controlsCount() = 0;
	virtual Control* getControl(unsigned int index) = 0;

	virtual void draw(DrawableCanvas& canvas, int width, int height);

protected:
	virtual bool drawControl(DrawableCanvas& canvas, int control) = 0;
	virtual void cacheCanvasSize(int width, int height) = 0;

	void notifyModelUpdate();

	ControlsContainer* m_AssociatedContainer = nullptr;

	int16_t m_CacheCanvasWidth = -1;
	int16_t m_CacheCanvasHeight = -1;
};

class ControlsContainer : public Control, public ControlsHandler {
	friend class ControlsContainerModel;
public:
	ControlsContainer(ControlsContainerModel* model = nullptr, unsigned int h = 0, unsigned int v = 0, unsigned int width = 0, unsigned int height = 0);
	virtual ~ControlsContainer() override = default;

	bool setOffsets(int offfsetW, int offsetH);

	void processControlEvent(Control& control, const CONTROL_EVENT event) override;
	void promoteControl(Control& ctrl) override;
	void removeControl(Control& ctrl) override;
	void addControl(Control& ctrl) override;

	void setModel(ControlsContainerModel* model);

protected:
	ControlsContainerModel* m_ControlModel;
	void modelUpdated();

private:
	ControlsContainerModel* m_Next;
};

