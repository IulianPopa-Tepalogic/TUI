/*
 * Animation.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: IulianPopa
 */


#include <algorithm>
#include <math.h>
#include <memory.h>
#include <mutex>

#include "gui/Animation.h"
#include "gui/Control.h"

using namespace std;

static bool _default_callback(Animation*, Control*, void* )
{
	return true;
}


Animation::Animation()
	: m_AnimationCallback(_default_callback)
{
}

 void Animation::setAnimationLoopEndCallback(ANIMATION_CALLBACK cb, void* ctxt)
{
	m_AnimationCallback = (cb != nullptr) ? cb : _default_callback;
	m_AnimationCallbackCtxt = ctxt;
}

void Animation::prepareAnimation(int pendTicks, int lengthTicks, Pixel fromColor, Pixel toColor)
{
	const auto tick = HIROS_TICKS();

	m_StartTick = m_InterpolationTick = tick + pendTicks;
	m_EndTick = tick + pendTicks + (lengthTicks == 0 ? 1 : lengthTicks);

	memcpy(m_From.color, &fromColor, sizeof (m_From.color));
	memcpy(m_To.color, &toColor, sizeof (m_To.color));
}

void Animation::prepareAnimation(int pendTicks, int lengthTicks, float fromValue, float toValue)
{
	const auto tick = HIROS_TICKS();

	m_StartTick = m_InterpolationTick = tick + pendTicks;
	m_EndTick = tick + pendTicks + (lengthTicks == 0 ? 1 : lengthTicks);

	m_From.value = fromValue;
	m_To.value = toValue;
}

void Animation::tick(Control* control)
{
	const uint32_t ticks = HIROS_TICKS();
	float raport = min(max<float>(ticks - m_StartTick, .0f) / (m_EndTick - m_StartTick), 1.0f);

	switch (m_TYPE)
	{
	case LINEAR:
		break;
	case EASY_IN:
		raport = raport * raport;
		break;

	case EASY_OUT:
		raport = 1.0f - raport;
		raport = raport * raport;
		raport = 1.0f - raport;
		break;

	case EASY_IN_OUT:
		const float raport5 = raport * raport * raport * raport *raport;
		raport = (1.0f - raport);
		raport = raport * raport * raport * raport *raport;
		raport = (1.0f - raport + raport5);
		raport *= 0.5f;
		break;
	}

	switch (m_Property)
	{
	case BORDER_COLOR:
	case BKG_COLOR:
	case TEXT_COLOR:
	case BT_BK_COLOR_PRESSED:
	case BT_TEXT_COLOR_PRESSED:
	case COMPLETED_COLOR:
	case REMAIN_COLOR:
	case INDICATOR_COLOR:
	{
		const uint8_t blue = (1.0f - raport) * ((Pixel*)m_From.color)->blue + raport * ((Pixel*)m_To.color)->blue;
		const uint8_t green = (1.0f - raport) * ((Pixel*)m_From.color)->green + raport * ((Pixel*)m_To.color)->green;
		const uint8_t red = (1.0f - raport) * ((Pixel*)m_From.color)->red + raport * ((Pixel*)m_To.color)->red;

		control->updateAnimatedProperty(m_Property, Pixel(red, green, blue));
	}
		break;

	default:
		if (m_From.value <= m_To.value)
			control->updateAnimatedProperty(m_Property, m_From.value + fabs(m_To.value - m_From.value) * raport);
		else
			control->updateAnimatedProperty(m_Property, m_From.value - fabs(m_From.value - m_To.value) * raport);
	}

	if (ticks < m_EndTick)
		return ;

	if (!m_AnimationCallback(this, control, m_AnimationCallbackCtxt))
		return ;

	if (m_LoopingMode == LOOP_MODE::LOOP_START)
	{
		const auto lenght = m_EndTick - m_StartTick ;
		m_StartTick = ticks;
		m_EndTick = ticks + lenght;
	}
	else if (m_LoopingMode == LOOP_MODE::LOOP_INVERSE)
	{
		const auto lenght = m_EndTick - m_StartTick ;
		m_StartTick = ticks;
		m_EndTick = ticks + lenght;

		switch (m_Property)
		{
		case BORDER_COLOR:
		case BKG_COLOR:
		case TEXT_COLOR:
		case BT_BK_COLOR_PRESSED:
		case BT_TEXT_COLOR_PRESSED:
		case COMPLETED_COLOR:
		case REMAIN_COLOR:
		case INDICATOR_COLOR:
			swap (m_From.color, m_To.color);
			break;

		default:
			swap (m_From.value, m_To.value);
		};
	}
	else if (m_LoopingMode == LOOP_MODE::ONCE)
		m_LoopingMode = LOOP_MODE::END;
}


static Animation availableAnimations[10];
static AnimationKeeper __availableAnimations;
static bool availableInited = false;

AnimationKeeper& AnimationKeeper::AvailableAnimationKeeper()
{
	if (!availableInited)
	{
		availableInited = true;
		for (size_t i = 0; i < (sizeof availableAnimations / sizeof availableAnimations[0]); ++i)
			__availableAnimations.add(availableAnimations[i]);
	}

	return __availableAnimations;
}

void AnimationKeeper::add(Animation& animation)
{
	lock_guard<HirosMutex> guard(m_Sync);

	animation.m_Next = m_ListHead;
	m_ListHead = &animation;
}

Animation* AnimationKeeper::remove(Animation* animation)
{
	lock_guard<HirosMutex> guard(m_Sync);

	if ((animation == nullptr) || (animation == m_ListHead))
	{
		Animation* result = m_ListHead;
		if (result == nullptr)
			return nullptr;

		m_ListHead = result->m_Next;
		result->m_Next = nullptr;

		return result;
	}

	Animation* it = m_ListHead;
	Animation* prev = nullptr;
	while ((it != nullptr) && (it != animation))
	{
		prev = it;
		it = it->m_Next;
	}

	if (it != animation)
		return it;

	prev->m_Next = animation->m_Next;
	animation->m_Next = nullptr;
	animation->setAnimationLoopEndCallback(nullptr, nullptr);

	return animation;
}

Animation* AnimationKeeper::next(Animation* animation)
{
	lock_guard<HirosMutex> guard(m_Sync);
	return animation == nullptr ? m_ListHead : animation->m_Next;
}
