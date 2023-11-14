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

class PixelColorProvider
{
public:
	using CANVAS_PIXEL_PROVIDER = Pixel (*)(int h, int v, const Shape& shape);

	PixelColorProvider()
		: m_Shape(nullptr)
	{}

	explicit PixelColorProvider(const Shape& shape)
		: m_Shape(&shape)
	{}
	virtual ~PixelColorProvider() = default;

	virtual Pixel get(int h, int v) const = 0;

protected:
	const Shape* const m_Shape;
};


class Unicolor : public PixelColorProvider
{
public:
	Unicolor(const uint8_t red, const uint8_t green, const uint8_t blue)
		: Unicolor(Pixel(red, green, blue))
	{}

	Unicolor(const Pixel& color)
		: m_Color(color)
	{}

	Pixel get(int h, int v) const override { return m_Color; }

protected:
	const Pixel m_Color;
};


class CustomPixelProvider : public PixelColorProvider
{
public:
	CustomPixelProvider(CANVAS_PIXEL_PROVIDER provider, const Shape& shape)
		: PixelColorProvider(shape),
		  m_Provider(provider)
	{}

	Pixel get(int h, int v) const override { return m_Provider(h, v, *m_Shape); }

protected:
	 CANVAS_PIXEL_PROVIDER m_Provider;
};


class VericalGradient : public PixelColorProvider
{
public:
	VericalGradient(const Pixel& from, const Pixel& to, const Shape& shape)
		: VericalGradient(ColorGradient(from, to), shape)
	{}

	VericalGradient(const ColorGradient& gradient, const Shape& shape)
		: PixelColorProvider(shape),
		  m_Gradient(gradient)
	{}

	Pixel get(int h, int v) const;

	ColorGradient m_Gradient;
	int16_t m_StartOffset = 0;
	uint16_t m_Width = 0;
};


class HorizontalGradient : public PixelColorProvider
{
public:
	HorizontalGradient(const Pixel& from, const Pixel& to, const Shape& shape)
		: HorizontalGradient(ColorGradient(from, to), shape)
	{}

	HorizontalGradient(const ColorGradient& gradient, const Shape& shape)
		: PixelColorProvider(shape)
		, m_Gradient(gradient)
	{}

	Pixel get(int h, int v) const;

	ColorGradient m_Gradient;
	int16_t m_StartOffset = 0;
	uint16_t m_Width = 0;
};


class RadiantGradient : public PixelColorProvider
{
public:
	RadiantGradient(const Pixel& from, const Pixel& to, const Shape& shape)
		: RadiantGradient(ColorGradient(from, to), shape)
	{}

	RadiantGradient(const ColorGradient& gradient, const Shape& shape)
		: PixelColorProvider(shape),
		  m_Gradient(gradient)
	{}

	Pixel get(int h, int v) const;

	const ColorGradient m_Gradient;

	uint16_t m_RadiusOffset = 0;
};

class CanvasBackground : public PixelColorProvider
{
public:
	CanvasBackground(const Canvas& pattern, bool repeatHorizontally, bool repeatVertically, const Shape& shape)
	: PixelColorProvider(shape),
	  m_Pattern(pattern),
	  m_RepeatedHorizontally(repeatHorizontally),
	  m_RepeatedVertically(repeatVertically),
	  m_Stretch(false)
	{};

	CanvasBackground(const Canvas& pattern, bool stretch, const Shape& shape)
	: PixelColorProvider(shape),
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
