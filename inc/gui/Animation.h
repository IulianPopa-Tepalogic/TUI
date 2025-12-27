/*
 * Animation.h
 *
 *  Created on: Aug 12, 2020
 *      Author: IulianPopa
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "tui_hwconfig.h"
#include "tui_mutex.h"


class AnimationKeeper;
class Control;


class Animation {
	friend class AnimationKeeper;

public:
	enum PROPERTY
	{
		NONE,

		HORIZONTAL,
		VERTICAL,
		WIDTH,
		HEIGHT,

		BORDER_SIZE,
		BORDER_RADIUS,
		BORDER_COLOR,

		BKG_COLOR,
		BKG_PATTERN,
		TEXT_SIZE,
		TEXT_COLOR,

		BT_BK_COLOR_PRESSED,
		BT_BKG_PATTERN_PRESSED,
		BT_TEXT_SIZE_PRESSED,
		BT_TEXT_COLOR_PRESSED,

		VALUE,
		COMPLETED_COLOR,
		REMAIN_COLOR,
		INDICATOR_COLOR,

		SCROLLER_VERTICAL_OFFSET,
		SCROLLER_HORIZONTAL_OFFSET,
	};


	enum TYPE
	{
		LINEAR,
		EASY_IN,
		EASY_OUT,
		EASY_IN_OUT
	};

	enum LOOP_MODE {
		ONCE,
		LOOP_START,
		LOOP_INVERSE,
		END
	};

	using ANIMATION_CALLBACK = bool (*)(Animation*, Control* ctrl, void* context);

	Animation();
	~Animation() = default;

	void setAnimationLoopEndCallback(ANIMATION_CALLBACK cb, void* ctxt);

	void prepareAnimation(int pendTicks, int lengthTicks, Pixel fromColor, Pixel toColor);
	void prepareAnimation(int pendTicks, int lengthTicks, float fromValue, float toValue);

	void tick(Control* control);

	PROPERTY m_Property = NONE;
	TYPE m_TYPE = LINEAR;
	LOOP_MODE m_LoopingMode = ONCE;

protected:

	Animation* m_Next = nullptr;
	ANIMATION_CALLBACK m_AnimationCallback = nullptr;
	void* m_AnimationCallbackCtxt = nullptr;

	uint32_t m_StartTick = 0;
	uint32_t m_InterpolationTick = 0;
	uint32_t m_EndTick = 0;

	union {
		float value;
		uint8_t color[sizeof(Pixel)];
	} m_From;

	union {
		float value;
		uint8_t color[sizeof(Pixel)];
	} m_To;
};


class AnimationKeeper {
	friend class Control;

public:
	AnimationKeeper() = default;
	~AnimationKeeper() = default;

protected:
	void add(Animation& animation);
	Animation* remove(Animation* anim = nullptr);
	Animation* next(Animation* anim);

	static AnimationKeeper& AvailableAnimationKeeper();

	Animation* m_ListHead = nullptr;
	HirosMutex m_Sync;
};

#endif /* ANIMATION_H_ */
