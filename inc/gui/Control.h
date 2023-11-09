/*
 * Control.h
 *
 *  Created on: Jun 16, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gfx/Fragment.h"
#include "gui/Animation.h"

enum CONTROL_EVENT
{
	UKNOWN = 0,

	GAIN_FOCUS,
	LOST_FOCUS,
	VISIBLE_CHANGED,
	ACTIVE_CHANGED,
	PRESSED,
	RELEASED,
	TOGGLED_CHANGED,
	VALUE_UPDATED,
	TEXT_CHANGED,
	SELECTION_UPDATED
};


class ControlsHandler {
public:
	ControlsHandler() = default;
	virtual ~ControlsHandler() = default;

	virtual void processControlEvent(Control& control, const CONTROL_EVENT event) = 0;
	virtual void promoteControl(Control& ctrl) = 0;
	virtual void removeControl(Control& ctrl) = 0;
	virtual void addControl(Control& ctrl) = 0;
};

class Control {
	friend class Dialog;
	friend class ControlsContainer;

public:

	Control(unsigned int h, unsigned int v, unsigned int width, unsigned int height);
	virtual ~Control()  = default;

	unsigned int horizontal() const { return m_HFrom; }
	unsigned int vertical() const { return m_VFrom; }
	unsigned int width() const { return m_Width; }
	unsigned int height() const { return m_Height; }

	virtual Control& setHorizontal(unsigned int h);
	virtual Control& setVertical(unsigned int v);
	virtual Control& setWidth(unsigned int width);
	virtual Control& setHeight(unsigned int height);

	virtual Control& setBorderSize(unsigned int size);
	virtual Control& setBorderRadius(unsigned int radius);
	virtual Control& setBorderColor(Pixel color);

	bool needsRedraw() const { return m_NeedsRedraw; }
	void markDirty();
	void setVisible(bool visible);
	void setActive(bool active);

	bool draw(DrawableCanvas& canvas);

	Animation* animateProperty(Animation::PROPERTY property, float toValue, unsigned int lenghtMs, unsigned int pendMs = 0, Animation::LOOP_MODE mode = Animation::ONCE, Animation::TYPE type = Animation::LINEAR);
	Animation* animateProperty(Animation::PROPERTY property, float fromValue, float toValue, unsigned int lenghtMs, unsigned int pendMs = 0, Animation::LOOP_MODE mode = Animation::ONCE, Animation::TYPE type = Animation::LINEAR);
	Animation* animateProperty(Animation::PROPERTY property, Pixel toColor, unsigned int lenghtMs, unsigned int pendMs = 0, Animation::LOOP_MODE mode = Animation::ONCE, Animation::TYPE type = Animation::LINEAR);
	Animation* animateProperty(Animation::PROPERTY property, Pixel fromColor, Pixel toColor, unsigned int lenghtMs, unsigned int pendMs = 0, Animation::LOOP_MODE mode = Animation::ONCE, Animation::TYPE type = Animation::LINEAR);
	Animation* getRunningAnimation(Animation::PROPERTY property);
	void cancelAnimation(Animation::PROPERTY property);
	void cancelAllAnimations();

	virtual bool getAnimatedProperty(Animation::PROPERTY property, float* outValue);
	virtual bool getAnimatedProperty(Animation::PROPERTY property, Pixel* outColor);
	virtual bool updateAnimatedProperty(Animation::PROPERTY property, float value);
	virtual bool updateAnimatedProperty(Animation::PROPERTY property, Pixel color);
	bool tickAnimatedProperties();

	bool overlaps(const Control& constrol) const;

	virtual bool handleUserInteractionEvent(unsigned int event, int arg1, int arg2);
	virtual bool isTouchInActiveArea(int h, int v) const;

protected:
	virtual void __draw(DrawableCanvas& canvas) = 0;

	Pixel m_BorderColor = Pixel (0, 0, 0);

	uint16_t m_HFrom;
	uint16_t m_VFrom;
	uint16_t m_Width;
	uint16_t m_Height;

	uint8_t m_BorderSize = 0;
	uint8_t m_BorderRadius = 0;

	bool m_NeedsRedraw = true;
	bool m_Visible = true;
	bool m_Active = true;

	ControlsHandler* m_EventsHandler = nullptr;

	AnimationKeeper m_Animations;

private:
	void notifyBackGroundRedraw();
	void notifyTopRedraw();

	Control* m_NextControl = nullptr;
	Control* m_PrevControl = nullptr;
};
