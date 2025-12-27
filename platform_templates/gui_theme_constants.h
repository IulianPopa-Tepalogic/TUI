/*
 * gui_theme_constants.h
 *
 *  Created on: Jun 17, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "tui_gui_base.h"

class Font;

extern Font mss8pts;

static constexpr Pixel APP_BK_COLOR = Pixel(0, 0, 0);

static constexpr unsigned int DIALOG_BORDER_SIZE = 3;
static constexpr Pixel DIALOG_BORDER_COLOR = Pixel(255, 0, 0);
static constexpr Pixel DIALOG_TITLE_BK_COLOR = Pixel(255, 255, 0);
static constexpr Pixel DIALOG_TITLE_COLOR = Pixel(0, 0, 0);
static constexpr Pixel DIALOG_BK_COLOR = Pixel(25, 25, 25);
static constexpr Canvas* const DIALOG_TITLE_BK_PATTERN = nullptr;
static constexpr Canvas* const DIALOG_BK_PATTERN = nullptr;
static constexpr Font& DIALOG_TITLE_FONT = mss8pts;
static constexpr unsigned int DIALOG_TITLE_HEIGHT = 20;
static constexpr float DIALOG_FONT_SCALE = 1.3f;


/* LABEL */
static constexpr Font& LABEL_TEXT_FONT = mss8pts;
static constexpr float LABEL_TEXT_SIZE = 1.21f;
static constexpr Pixel LABEL_TEXT_COLOR = Pixel(0, 0, 0);

/* Button */
static constexpr Font& BUTTON_TEXT_FONT = mss8pts;
static constexpr float BUTTON_TEXT_SIZE = 1.0f;
static constexpr Pixel BUTTON_TEXT_COLOR = Pixel(0, 0, 0);
static constexpr float BUTTON_PRESSED_TEXT_SIZE = 1.0f;
static constexpr Pixel BUTTON_PRESSED_TEXT_COLOR = Pixel(255, 255, 255);
static constexpr Pixel BUTTON_BK_COLOR = Pixel(255, 255, 255);
static constexpr Pixel BUTTON_PRESSED_BK_COLOR = Pixel(0, 0, 0);
static constexpr Canvas* const BUTTON_PRESSED_BK_PATTERN = nullptr;
static constexpr Canvas* const BUTTON_BK_PATTERN = nullptr;

/* Slider */
static constexpr Pixel SLIDER_INDICATOR_COLOR = Pixel(255, 0, 0);
static constexpr uint8_t SLIDER_INDICATOR_RECT_THICK = 7;
static constexpr uint8_t SLIDER_INDICATOR_RECT_RADIUS = 3;
static constexpr Pixel SLIDER_LEVEL_COMPLETED_COLOR = Pixel(255, 0, 0);
static constexpr Pixel SLIDER_LEVEL_REMAIN_COLOR = Pixel(0, 0, 0);
static constexpr uint8_t SLIDER_LEVEL_RECT_THICK = 7;
static constexpr uint8_t SLIDER_LEVEL_RECT_RADIUS = 3;

/* Progress Bar */
static constexpr uint8_t PBAR_BORDER_SIZE = 1;
static constexpr uint8_t PBAR_BORDER_RADIUS = 10;
static constexpr Pixel PBAR_BORDER_COLOR = Pixel(255, 255, 255);
static constexpr Pixel PBAR_COMPLETED_COLOR = Pixel(255, 0, 0);
static constexpr Pixel PBAR_REMAIN_COLOR = Pixel(0, 0, 0);


/* Scroller */
static constexpr uint8_t SCROLLER_SCROLLBAR_WIDTH = 4;
static constexpr uint8_t SCROLLER_SCROLLBAR_OFFSET = 2;
static constexpr uint8_t SCROLLER_SCROLLBAR_RADIUS = 2;
static constexpr Pixel SCROLLER_SCROLLBAR_COLOR = Pixel(255, 0, 255);

/* List */
static constexpr Font& LIST_HEADER_ITEM_TEXT_FONT = mss8pts;
static constexpr Font& LIST_ITEM_TEXT_FONT = mss8pts;
static constexpr Font& LIST_SELECTED_ITEM_TEXT_FONT = mss8pts;
static constexpr float LIST_HEADER_ITEM_TEXT_SIZE = 1.4f;
static constexpr float LIST_ITEM_TEXT_SIZE = 1.0f;
static constexpr float LIST_SELECTED_ITEM_TEXT_SIZE = 1.1f;

static constexpr Pixel LIST_HEADER_ITEM_COLOR = Pixel(255, 255, 255);
static constexpr Pixel LIST_HEADER_ITEM_BK_COLOR = Pixel(255, 0, 0);
static constexpr Pixel LIST_ITEM_COLOR = Pixel(0, 0, 0);
static constexpr Pixel LIST_ITEM_BK_COLOR = Pixel(255, 255, 255);
static constexpr Pixel LIST_SELECTED_ITEM_COLOR = Pixel(0, 255, 0);
static constexpr Pixel LIST_SELECTED_ITEM_BK_COLOR = Pixel(0, 0, 0);
static constexpr Pixel LIST_BACKGROUND_COLOR = Pixel(0, 0, 255);

/* Chooser */
static constexpr Font& CHOOSER_ITEM_TEXT_FONT = mss8pts;
static constexpr float CHOOSER_ITEM_TEXT_SIZE = 1.0f;
static constexpr Pixel CHOOSER_ITEM_BK_COLOR = Pixel(255, 255, 255);
static constexpr Pixel CHOOSER_ITEM_COLOR = Pixel(0, 0, 0);
