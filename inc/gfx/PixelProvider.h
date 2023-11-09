/*
 * PixelProvider.h
 *
 *  Created on: Sep 4, 2020
 *      Author: IulianPopa
 */

#ifndef PIXELPROVIDER_H_
#define PIXELPROVIDER_H_


#include <algorithm>

#include "hiros_gui_base.h"
#include "ColorGradient.h"

class Shape;
class Canvas;

class PixelProvider
{
public:
	using CANVAS_PIXEL_PROVIDER = Pixel (*)(int h, int v, const Shape& shape);

	PixelProvider()
		: m_Shape(nullptr)
	{}

	explicit PixelProvider(const Shape& shape)
		: m_Shape(&shape)
	{}
	virtual ~PixelProvider() = default;

	virtual Pixel get(int h, int v) const = 0;

protected:
	const Shape* m_Shape;
};


class ColorPixelProvider : public PixelProvider
{
public:
	ColorPixelProvider(const uint8_t red, const uint8_t green, const uint8_t blue)
		: ColorPixelProvider(Pixel(red, green, blue))
	{}

	ColorPixelProvider(const Pixel& color)
		: m_Color(color)
	{}

	Pixel get(int h, int v) const override { return m_Color; }

protected:
	const Pixel m_Color;
};


class CustomPixelProvider : public PixelProvider
{
public:
	CustomPixelProvider(CANVAS_PIXEL_PROVIDER provider, const Shape& shape)
		: PixelProvider(shape),
		  m_Provider(provider)
	{}

	Pixel get(int h, int v) const override { return m_Provider(h, v, *m_Shape); }

protected:
	 CANVAS_PIXEL_PROVIDER m_Provider;
};


class VericalGradientPixelProvider : public PixelProvider
{
public:
	VericalGradientPixelProvider(const Pixel& from, const Pixel& to, const Shape& shape)
		: VericalGradientPixelProvider(ColorGradient(from, to), shape)
	{}

	VericalGradientPixelProvider(const ColorGradient& gradient, const Shape& shape)
		: PixelProvider(shape),
		  m_Gradient(gradient)
	{}

	Pixel get(int h, int v) const;

	ColorGradient m_Gradient;
	int16_t m_StartOffset = 0;
	uint16_t m_Width = 0;
};


class HorizontalGradientPixelProvider : public PixelProvider
{
public:
	HorizontalGradientPixelProvider(const Pixel& from, const Pixel& to, const Shape& shape)
		: HorizontalGradientPixelProvider(ColorGradient(from, to), shape)
	{}

	HorizontalGradientPixelProvider(const ColorGradient& gradient, const Shape& shape)
		: PixelProvider(shape),
		  m_Gradient(gradient)
	{}

	Pixel get(int h, int v) const;

	ColorGradient m_Gradient;
	int16_t m_StartOffset = 0;
	uint16_t m_Width = 0;
};


class RadiantGradientProvider : public PixelProvider
{
public:
	RadiantGradientProvider(const Pixel& from, const Pixel& to, const Shape& shape)
		: RadiantGradientProvider(ColorGradient(from, to), shape)
	{}

	RadiantGradientProvider(const ColorGradient& gradient, const Shape& shape)
		: PixelProvider(shape),
		  m_Gradient(gradient)
	{}

	Pixel get(int h, int v) const;

	const ColorGradient m_Gradient;

	uint16_t m_RadiusOffset = 0;
};

class PatternPixelProvider : public PixelProvider
{
public:
	PatternPixelProvider(const Canvas& pattern, bool repeatHorizontally, bool repeatVertically, const Shape& shape)
	: PixelProvider(shape),
	  m_Pattern(pattern),
	  m_RepeatedHorizontally(repeatHorizontally),
	  m_RepeatedVertically(repeatVertically),
	  m_Stretch(false)
	{};

	PatternPixelProvider(const Canvas& pattern, bool stretch, const Shape& shape)
	: PixelProvider(shape),
	  m_Pattern(pattern),
	  m_RepeatedHorizontally(false),
	  m_RepeatedVertically(false),
	  m_Stretch(stretch)
	{};


	Pixel get(int h, int v) const;

	const Canvas& m_Pattern;
	const bool m_RepeatedHorizontally;
	const bool m_RepeatedVertically;
	const bool m_Stretch;
};


#endif /* PIXELPROVIDER_H_ */
