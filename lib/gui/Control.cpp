/*
 * Control.cpp
 *
 *  Created on: Jun 16, 2020
 *      Author: IulianPopa
 */

#include "gui/Control.h"

#include <memory.h>
#include <math.h>
#include <mutex>

#include "gui/Dialog.h"

#include "gfx/Rectangle.h"
#include "gfx/RoundRectangle.h"


using namespace std;


static bool rectanle_holds_point(unsigned int rhfrom,
								 unsigned int rvfrom,
								 unsigned int rwidth,
								 unsigned int rheight,
								 unsigned int ph,
								 unsigned int pv)
{
	return ((rhfrom <= ph) && (ph <= rhfrom + rwidth))
			&& ((rvfrom <= pv) && (pv <= rvfrom + rheight));
}


Control::Control(unsigned int h, unsigned int v, unsigned int width, unsigned int height)
	: m_HFrom(h),
	  m_VFrom(v),
	  m_Width(width),
	  m_Height(height)
{
}


Control& Control::setHorizontal(unsigned int h)
{
	m_HFrom = h;
	markDirty();

	return *this;
}

Control& Control::setVertical(unsigned int v)
{
	m_VFrom = v;
	markDirty();

	return *this;
}

Control& Control::setWidth(unsigned int width)
{
	m_Width = width;
	markDirty();

	return *this;
}

Control& Control::setHeight(unsigned int height)
{
	m_Height = height;
	markDirty();

	return *this;
}

Control& Control::setBorderSize(unsigned int size)
{
	m_BorderSize = size;
	markDirty();

	return *this;
}
Control& Control::setBorderRadius(unsigned int radius)
{
	m_BorderRadius = radius;
	markDirty();

	return *this;
}

Control& Control::setBorderColor(Pixel color)
{
	m_BorderColor = color;
	markDirty();

	return *this;
}



void Control::markDirty()
{
	if (m_NeedsRedraw | !m_Visible)
		return ;

	m_NeedsRedraw = true;
	notifyTopRedraw();
	notifyBackGroundRedraw();
}


void Control::setVisible(bool visible)
{
	if (visible == m_Visible)
		return ;

	m_Visible = true;
	notifyBackGroundRedraw();
	notifyTopRedraw();

	m_Visible = visible;
	m_NeedsRedraw = m_Visible;

	if (m_EventsHandler != nullptr)
		m_EventsHandler->processControlEvent(*this, VISIBLE_CHANGED);
}


void Control::setActive(bool active)
{
	if (active == m_Active)
		return ;

	m_Active = active;
	if (m_EventsHandler != nullptr)
		m_EventsHandler->processControlEvent(*this, ACTIVE_CHANGED);

	notifyBackGroundRedraw();
	notifyTopRedraw();
}


bool Control::draw(DrawableCanvas& canvas)
{
	if (!m_Visible)
		return false;

	Fragment area(m_HFrom, m_VFrom, m_Width, m_Height, &canvas);
	__draw(area);

	m_NeedsRedraw = false;
	return true;
}

Animation* Control::animateProperty(Animation::PROPERTY property, float toValue, unsigned int lenghtMs, unsigned int pendMs, Animation::LOOP_MODE mode, Animation::TYPE type)
{
	float fromValue;
	if (! getAnimatedProperty(property, &fromValue))
		return nullptr;

	return animateProperty(property, fromValue, toValue, lenghtMs, pendMs, mode, type);
}

Animation* Control::animateProperty(Animation::PROPERTY property, float fromValue, float toValue, unsigned int lenghtMs, unsigned int pendMs, Animation::LOOP_MODE mode, Animation::TYPE type)
{
	lock_guard<HirosMutex> guard(m_Animations.m_Sync);

	auto animation = getRunningAnimation(property);
	if (animation == nullptr)
	{
		animation = AnimationKeeper::AvailableAnimationKeeper().remove();
		if (animation != nullptr)
			m_Animations.add(*animation);
	}

	if (animation == nullptr)
	{
		updateAnimatedProperty(property, toValue);
		return nullptr;
	}

	animation->m_Property = property;
	animation->m_TYPE = type;
	animation->m_LoopingMode = mode;

	animation->prepareAnimation(HIROS_MS_2_TICKS(pendMs), HIROS_MS_2_TICKS(lenghtMs), fromValue, toValue);

	return animation;
}


Animation* Control::animateProperty(Animation::PROPERTY property, Pixel toValue, unsigned int lenghtMs, unsigned int pendMs, Animation::LOOP_MODE mode, Animation::TYPE type)
{
	Pixel fromValue;
	if (! getAnimatedProperty(property, &fromValue))
		return nullptr;

	return animateProperty(property, fromValue, toValue, lenghtMs, pendMs, mode, type);
}

Animation* Control::animateProperty(Animation::PROPERTY property, Pixel fromValue, Pixel toValue, unsigned int lenghtMs, unsigned int pendMs, Animation::LOOP_MODE mode, Animation::TYPE type)
{
	lock_guard<HirosMutex> guard(m_Animations.m_Sync);

	auto animation = getRunningAnimation(property);
	if (animation == nullptr)
	{
		animation = AnimationKeeper::AvailableAnimationKeeper().remove();
		if (animation != nullptr)
			m_Animations.add(*animation);
	}

	if (animation == nullptr)
	{
		updateAnimatedProperty(property, toValue);
		return nullptr;
	}

	animation->m_Property = property;
	animation->m_TYPE = type;
	animation->m_LoopingMode = mode;

	animation->prepareAnimation(HIROS_MS_2_TICKS(pendMs), HIROS_MS_2_TICKS(lenghtMs), fromValue, toValue);

	return animation;
}

Animation* Control::getRunningAnimation(Animation::PROPERTY property)
{
	Animation* anim = nullptr;

	lock_guard<HirosMutex> guard(m_Animations.m_Sync);

	while ((anim = m_Animations.next(anim)) != nullptr)
	{
		if (anim->m_Property == property)
			return anim;
	}

	return nullptr;
}

void Control::cancelAnimation(Animation::PROPERTY property)
{
	lock_guard<HirosMutex> guard(m_Animations.m_Sync);

	auto animation = getRunningAnimation(property);
	if (animation == nullptr)
		return ;

	m_Animations.remove(animation);
	AnimationKeeper::AvailableAnimationKeeper().add(*animation);
}


void Control::cancelAllAnimations()
{
	lock_guard<HirosMutex> guard(m_Animations.m_Sync);

	Animation* animation = nullptr;
	while ((animation = m_Animations.remove()) != nullptr)
		AnimationKeeper::AvailableAnimationKeeper().add(*animation);
}



bool Control::getAnimatedProperty(Animation::PROPERTY property, float* outValue)
{
	switch (property)
	{
	case Animation::PROPERTY::WIDTH:
		*outValue = m_Width;
		return true;

	case Animation::PROPERTY::HEIGHT:
		*outValue = m_Height;
		return true;

	case Animation::PROPERTY::HORIZONTAL:
		*outValue = m_HFrom;
		return true;

	case Animation::PROPERTY::VERTICAL:
		*outValue = m_VFrom;
		return true;

	case Animation::PROPERTY::BORDER_SIZE:
		*outValue = m_BorderSize;
		return true;

	case Animation::PROPERTY::BORDER_RADIUS:
		*outValue = m_BorderRadius;
		return true;

	default:
		return false;
	}

	return true;
}

bool Control::getAnimatedProperty(Animation::PROPERTY property, Pixel* outColor)
{
	if (property != Animation::PROPERTY::BORDER_COLOR)
		return false;

	*outColor = m_BorderColor;
	return true;
}

bool Control::updateAnimatedProperty(Animation::PROPERTY property, float value)
{
	switch (property)
	{
	case Animation::PROPERTY::WIDTH:
		setWidth(round(value));
		break;

	case Animation::PROPERTY::HEIGHT:
		setHeight(round(value));
		break;

	case Animation::PROPERTY::HORIZONTAL:
		setHorizontal(round(value));
		break;

	case Animation::PROPERTY::VERTICAL:
		setVertical(round(value));
		break;

	case Animation::PROPERTY::BORDER_SIZE:
		setBorderSize(round(value));
		break;

	case Animation::PROPERTY::BORDER_RADIUS:
		setBorderRadius(round(value));
		break;

	default:
		return false;
	}

	return true;
}

bool Control::updateAnimatedProperty(Animation::PROPERTY property, Pixel color)
{
	if (property != Animation::PROPERTY::BORDER_COLOR)
		return false;

	setBorderColor(color);
	return true;
}


bool Control::tickAnimatedProperties()
{
	bool result = false;

	Animation *anim = nullptr;
	while ((anim = m_Animations.next(anim)) != nullptr)
	{
		anim->tick(this);
		if (anim->m_LoopingMode == Animation::LOOP_MODE::END)
		{
			cancelAnimation(anim->m_Property);
			anim = nullptr; //start over
		}
		result |= true;
	}

	return result;
}

bool Control::overlaps(const Control& c) const
{
	if (! (m_Visible & c.m_Visible))
		return false;

	return rectanle_holds_point(horizontal(), vertical(), width(), height(), c.horizontal(), c.vertical())
			|| rectanle_holds_point(horizontal(), vertical(), width(), height(), c.horizontal() + c.width(), c.vertical())
			|| rectanle_holds_point(horizontal(), vertical(), width(), height(), c.horizontal() + c.width(), c.vertical() + c.height())
			|| rectanle_holds_point(horizontal(), vertical(), width(), height(), c.horizontal(), c.vertical() + c.height())
			|| rectanle_holds_point(c.horizontal(), c.vertical(), c.width(), c.height(), horizontal(), vertical());
}


bool Control::handleUserInteractionEvent(unsigned int event, int h, int v)
{
	switch (event)
	{
	case GUIApp::GAIN_FOCUS:
		if (m_EventsHandler != nullptr)
			m_EventsHandler->processControlEvent(*this, GAIN_FOCUS);
		break;

	case GUIApp::LOST_FOCUS:
		if (m_EventsHandler != nullptr)
			m_EventsHandler->processControlEvent(*this, LOST_FOCUS);
		break;

	default:
		return false;
	}

	return true;
}


bool Control::isTouchInActiveArea(int h, int v) const
{
	RoundRectangle rr(horizontal(), vertical(), width(), height(), m_BorderRadius);
	return rr.isPositionInActiveRegion(h, v);
}


void Control::notifyBackGroundRedraw()
{
	Control* iterator = m_PrevControl;
	while (iterator != nullptr)
	{
		if (overlaps(*iterator))
			iterator->markDirty();

		iterator = iterator->m_PrevControl;
	}
}

void Control::notifyTopRedraw()
{
	Control* iterator = m_NextControl;
	while (iterator != nullptr)
	{
		if (overlaps(*iterator))
			iterator->markDirty();

		iterator = iterator->m_NextControl;
	}
}
