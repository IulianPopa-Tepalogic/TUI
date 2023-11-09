/*
 * Keyboard.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: IulianPopa
 */

#include "gui/Keyboard.h"


void NumericKeyboardModel::showDecimalPoint(bool show)
{
	if (m_ShowDecimalPoint == show)
		return ;

	m_ShowDecimalPoint = show;
	notifyModelUpdate();
}


void NumericKeyboardModel::showNegate(bool show)
{
	if (m_ShowNegatePoint == show)
		return ;

	notifyModelUpdate();
}

int NumericKeyboardModel::controlsCount(void)
{
	int result = 10 + 1;

	if (showDecimalPoint())
		++result;

	if(showNegate())
		++result;

	return result;
}

Control* NumericKeyboardModel::getControl(unsigned int index)
{
	if (index < 10)
		return &m_Digits[index];

	if (index == 10)
		return &m_Delete;

	if (index == 11)
		return showNegate() ? &m_Negate : &m_DecimalPoint;

	else if (index == 12)
		return &m_DecimalPoint;

	return nullptr;
}

bool NumericKeyboardModel::drawControl(DrawableCanvas& canvas, int control)
{
	return false;
}

void NumericKeyboardModel::verticalSpacing(unsigned int spacing)
{
	if (m_VerticalSpacing != spacing)
	{
		m_VerticalSpacing = spacing;
		notifyModelUpdate();
	}
}

void NumericKeyboardModel::horizontalSpacing(unsigned int spacing)
{
	if (m_HorizontalSpacing != spacing)
	{
		m_HorizontalSpacing = spacing;
		notifyModelUpdate();
	}
}

void NumericKeyboardModel::cacheCanvasSize(int width, int height)
{
	const int rowsCount = 4 + (showDecimalPoint() && showNegate() ? 1 : 0);
	const int colsCount = 3;

	int btnWidth = (width - (colsCount - 1) * m_VerticalSpacing) / colsCount;
	int btnHeight = (height - (rowsCount - 1) * m_HorizontalSpacing) / rowsCount;

	m_Digits[7].setHorizontal(0).setVertical(0).setWidth(btnWidth).setHeight(btnHeight);
	m_Digits[8].setHorizontal(btnWidth + m_HorizontalSpacing).setVertical(0).setWidth(btnWidth).setHeight(btnHeight);
	m_Digits[9].setHorizontal(width - btnWidth).setVertical(0).setWidth(btnWidth).setHeight(btnHeight);

	m_Digits[4].setHorizontal(0).setVertical(btnHeight + m_VerticalSpacing).setWidth(btnWidth).setHeight(btnHeight);
	m_Digits[5].setHorizontal(btnWidth + m_HorizontalSpacing).setVertical(btnHeight + m_VerticalSpacing).setWidth(btnWidth).setHeight(btnHeight);
	m_Digits[6].setHorizontal(width - btnWidth).setVertical(btnHeight + m_VerticalSpacing).setWidth(btnWidth).setHeight(btnHeight);

	m_Digits[1].setHorizontal(0).setVertical(2 * (btnHeight + m_VerticalSpacing)).setWidth(btnWidth).setHeight(btnHeight);
	m_Digits[2].setHorizontal(btnWidth + m_HorizontalSpacing).setVertical(2 * (btnHeight + m_VerticalSpacing)).setWidth(btnWidth).setHeight(btnHeight);
	m_Digits[3].setHorizontal(width - btnWidth).setVertical(2 * (btnHeight + m_VerticalSpacing)).setWidth(btnWidth).setHeight(btnHeight);

	if (m_ShowDecimalPoint && m_ShowNegatePoint)
	{
		m_Negate.setHorizontal(0).setVertical(3 * (btnHeight + m_VerticalSpacing)).setWidth(btnWidth).setHeight(btnHeight);
		m_Digits[0].setHorizontal(btnWidth + m_HorizontalSpacing).setVertical(3 * (btnHeight + m_VerticalSpacing)).setWidth(btnWidth).setHeight(btnHeight);
		m_DecimalPoint.setHorizontal(width - btnWidth).setVertical(3 * (btnHeight + m_VerticalSpacing)).setWidth(btnWidth).setHeight(btnHeight);

		m_Delete.setHorizontal(0).setVertical(height - btnHeight).setWidth(width).setHeight(height);
	}
	else if (! (m_ShowDecimalPoint || m_ShowNegatePoint))
	{
		m_Digits[0].setHorizontal(0).setVertical(height - btnHeight).setWidth(2 * btnWidth + m_HorizontalSpacing).setHeight(btnHeight);
		m_Digits[0].setHorizontal(width - btnWidth).setVertical(height - btnHeight).setWidth(btnWidth).setHeight(btnHeight);
	}
	else
	{
		Button* btnDisplayed = m_ShowNegatePoint ? &m_Negate : &m_Delete;
		btnDisplayed->setHorizontal(0).setVertical(height - btnHeight).setWidth(btnWidth).setHeight(btnHeight);

		m_Digits[0].setHorizontal(btnWidth + m_HorizontalSpacing).setVertical(height - btnHeight).setWidth(btnWidth).setHeight(btnHeight);

		btnDisplayed = m_ShowDecimalPoint ? &m_DecimalPoint : &m_Delete;
		btnDisplayed->setHorizontal(width - btnWidth).setVertical(height - btnHeight).setWidth(btnWidth).setHeight(btnHeight);
	}
}


Keyboard::Keyboard(unsigned int h, unsigned int v, unsigned int width, unsigned int height)
	: ControlsContainer(nullptr, h, v, width, height)
{
}


void Keyboard::__draw(DrawableCanvas& canvas)
{
	if (m_ControlModel == nullptr)
		return ;

	m_ControlModel->draw(canvas, width(), height());
}


