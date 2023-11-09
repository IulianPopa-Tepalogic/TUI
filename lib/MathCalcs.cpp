/*
 * MathCalcs.cpp
 *
 *  Created on: May 20, 2020
 *      Author: IulianPopa
 */

#include "MathCalcs.h"

#include <math.h>
#include <stdint.h>


float MathCalcs::sinCache[3600];
float MathCalcs::cosCache[3600];

float MathCalcs::asinRad(float x)
{
	return std::asin(x);
}

float MathCalcs::atanRad(float x)
{
	return std::atan(x);
}

void MathCalcs::InitCaches()
{
	for (int idx = 0; idx < 3600; ++idx)
	{
		sinCache[idx] = std::sin(deg2rad(idx / 10.0f));
		cosCache[idx] = std::cos(deg2rad(idx / 10.0f));
	}
}
