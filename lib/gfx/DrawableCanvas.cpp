/*
 * DrawableCanvas.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: IulianPopa
 */

#include "gfx/DrawableCanvas.h"

#include <algorithm>
#include <math.h>

#include "../MathCalcs.h"

using namespace std;

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& pixel) const
{
	if (hfrom == hto)
	{
		const auto len = vto - vfrom;
		drawVLine(hfrom, vfrom, len, 1, pixel);
		return ;
	}
	else if (vfrom == vto)
	{
		const auto len = hto - hfrom;
		drawHLine(hfrom, vfrom, len, 1, pixel);
		return ;
	}

	const float hsize = fabs((float)(hfrom - hto));
	const float vsize = fabs((float)(vfrom - vto));
	if (vsize < hsize)
	{
		if (vto < vfrom)
		{
			swap(vto, vfrom);
			swap(hto, hfrom);
		}

		const int direction = hfrom < hto ? 1 : -1;
		const float segment = hsize / vsize;
		const float step = (float)(vto - vfrom)/(hto - hfrom);

		for (int h = hfrom, hcount = 0; h != hto; h += direction, ++hcount)
		{
			int v = vfrom + (hcount / segment);
			const float linev = (h - hfrom) * step + vfrom;

			float err = linev - v;
			if (err < .0f)
			{
				err = -err;
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h, v - 1, pixel, err);
			}
			else
			{
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h, v + 1, pixel, err);
			}
		}
	}
	else
	{
		if (hto < hfrom)
		{
			swap(vto, vfrom);
			swap(hto, hfrom);
		}

		const int direction = vfrom < vto ? 1 : -1;
		const float segment = vsize / hsize;
		const float step = (float)(hto - hfrom)/(vto - vfrom);

		for (int v = vfrom, vcount = 0; v != vto; v += direction, ++vcount)
		{
			int h = hfrom + (vcount / segment);
			const float lineh = (v - vfrom) * step + hfrom;

			float err = lineh - h;
			if (err < .0f)
			{
				err = -err;
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h - 1, v, pixel, err);
			}
			else
			{
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h + 1, v, pixel, err);
			}
		}
	}
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if (dashFill == 0)
		return ;

	if (hfrom == hto)
	{
		const auto len = vto - vfrom;
		drawVLine(hfrom, vfrom, len, 1, pixel, dashFill, dashSkip);
		return ;
	}
	else if (vfrom == vto)
	{
		drawHLine(hfrom, vfrom, hto - hfrom, 1, pixel, dashFill, dashSkip);
		return ;
	}

	const float hsize = fabs((float)(hfrom - hto));
	const float vsize = fabs((float)(vfrom - vto));
	if (vsize < hsize)
	{
		if (vto < vfrom)
		{
			swap(vto, vfrom);
			swap(hto, hfrom);
		}

		const int direction = hfrom < hto ? 1 : -1;
		const float segment = hsize / vsize;
		const float step = (float)(vto - vfrom)/(hto - hfrom);

		auto fill = dashFill;
		for (int h = hfrom, hcount = 0; (direction > 0 ? h < hto : hto < h); h += direction, ++hcount)
		{
			if (dashSkip > 0)
			{
				if (fill == 0)
				{
					hcount += (dashSkip - 1);
					h += direction * (dashSkip - 1);
					fill = dashFill;
					continue;
				}
				else
					fill--;
			}

			int v = vfrom + (hcount / segment);
			const float linev = (h - hfrom) * step + vfrom;

			float err = linev - v;
			if (err < .0f)
			{
				err = -err;
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h, v - 1, pixel, err);
			}
			else
			{
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h, v + 1, pixel, err);
			}
		}
	}
	else
	{
		if (hto < hfrom)
		{
			swap(vto, vfrom);
			swap(hto, hfrom);
		}

		const int direction = vfrom < vto ? 1 : -1;
		const float segment = vsize / hsize;
		const float step = (float)(hto - hfrom)/(vto - vfrom);

		auto fill = dashFill;
		for (int v = vfrom, vcount = 0; (direction > 0 ? v < vto : vto < v); v += direction, ++vcount)
		{
			if (dashSkip > 0)
			{
				if (fill == 0)
				{
					vcount += (dashSkip - 1);
					v += direction * (dashSkip - 1);
					fill = dashFill;
					continue;
				}
				else
					fill--;
			}

			int h = hfrom + (vcount / segment);
			const float lineh = (v - vfrom) * step + hfrom;

			float err = lineh - h;
			if (err < .0f)
			{
				err = -err;
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h - 1, v, pixel, err);
			}
			else
			{
				setPixel(h, v, pixel, 1.0f - err);
				setPixel(h + 1, v, pixel, err);
			}
		}
	}
}

void DrawableCanvas::drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const PixelColorProvider& pixel) const
{
	if (length < 0)
	{
		length = -length;
		clockWiseDeg += 180;
	}

	clockWiseDeg = MathCalcs::normalizeDeg(clockWiseDeg);

	const auto hto = hfrom + MathCalcs::cos(clockWiseDeg) * length;
	const auto vto = vfrom + MathCalcs::sin(clockWiseDeg) * length;

	drawLine(hfrom, vfrom, hto, vto, pixel);
}

void DrawableCanvas::drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if (length < 0)
	{
		length = -length;
		clockWiseDeg += 180;
	}

	clockWiseDeg = MathCalcs::normalizeDeg(clockWiseDeg);

	const auto hto = hfrom + MathCalcs::cos(clockWiseDeg) * length;
	const auto vto = vfrom + MathCalcs::sin(clockWiseDeg) * length;

	drawLine (hfrom, vfrom, hto, vto, pixel, dashFill, dashSkip);
}


void DrawableCanvas::drawHLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel) const
{
	if (length < 0)
	{
		hfrom += length;
		length = -length;
	}

	if (width < 0)
	{
		vfrom += width;
		width = -width;
	}

	if (hfrom < 0)
	{
		const auto off = -hfrom;
		length -= off;
		hfrom = 0;
	}

	if (vfrom < 0)
	{
		const int off = -vfrom;
		vfrom = 0;

		width -= off;
	}

	length = ((hfrom + length) <= m_Width) ? length : (m_Width - hfrom);
	width = ((vfrom + width) <= m_Height) ? width : (m_Height - vfrom);

	if ((length <= 0) || (width <= 0))
		return;

	for (int lineIdx = 0; lineIdx < width; lineIdx++)
		for (int i = 0;  i < length; ++i)
			setPixel(hfrom + i, vfrom + lineIdx, pixel.get(hfrom + i, vfrom + lineIdx));
}

void DrawableCanvas::drawHLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if (dashFill == 0)
		return ;

	else if (dashSkip == 0)
	{
		drawHLine(hfrom, vfrom, length, width, pixel);
		return ;
	}

	if (length < 0)
	{
		hfrom += length;
		length = -length;
	}

	if (width < 0)
	{
		vfrom += width;
		width = -width;
	}

	if (hfrom < 0)
	{
		const auto off = -hfrom;
		length -= off;
		hfrom = 0;
	}

	if (vfrom < 0)
	{
		const int off = -vfrom;
		vfrom = 0;

		width -= off;
	}

	length = ((hfrom + length) <= m_Width) ? length : (m_Width - hfrom);
	width = ((vfrom + width) <= m_Height) ? width : (m_Height - vfrom);

	auto fill = dashFill;
	for (int lineIdx = 0; lineIdx < width; lineIdx++)
		for (int i = 0;  i < length; ++i)
		{
			if (fill == 0)
			{
				i += dashSkip - 1;
				fill = dashFill;
				continue;
			}
			else
				fill--;

			setPixel(hfrom + i, vfrom + lineIdx, pixel.get(hfrom + i, vfrom + lineIdx));
		}
}

void DrawableCanvas::drawVLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel) const
{
	if (width < 0)
	{
		hfrom += width;
		width = -width;
	}

	if (length < 0)
	{
		vfrom += length;
		length = -length;
	}

	if (vfrom < 0)
	{
		const int off = -vfrom;
		vfrom = 0;
		length -= off;
	}

	if (hfrom < 0)
	{
		const int off = -hfrom;
		hfrom = 0;
		width -= off;
	}

	length = ((vfrom + length) <= m_Height) ? length : (m_Height - vfrom);
	width = ((hfrom + width) <= m_Width) ? width : (m_Width - hfrom);

	if ((length <= 0) || (width <= 0))
		return;

	for (int lineIdx = 0; lineIdx < width; ++lineIdx)
		for (int i = 0;  i < length; ++i)
			setPixel(hfrom + lineIdx, vfrom + i, pixel.get(hfrom + lineIdx, vfrom + i));
}

void DrawableCanvas::drawVLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if (dashFill == 0)
		return ;

	else if (dashSkip == 0)
	{
		drawVLine(hfrom, vfrom, length, width, pixel);
		return ;
	}

	if (width < 0)
	{
		hfrom -= width;
		width = -width;
	}

	if (length < 0)
	{
		vfrom -= length;
		length = -length;
	}

	if (vfrom < 0)
	{
		const int off = -vfrom;
		vfrom = 0;
		length -= off;
	}

	if (hfrom < 0)
	{
		const int off = -hfrom;
		hfrom = 0;
		width -= off;
	}

	length = ((vfrom + length) <= m_Height) ? length : (m_Height - hfrom);
	width = ((hfrom + width) <= m_Width) ? width : (m_Width - hfrom);

	if ((length <= 0) || (width <= 0))
		return;

	auto fill = dashFill;
	for (int lineIdx = 0; lineIdx < width; ++lineIdx)
	{
		for (int i = 0;  i < length; ++i)
		{
			if (fill == 0)
			{
				i += dashSkip - 1;
				fill = dashFill;
				continue;
			}
			else
				fill--;

			setPixel(hfrom + lineIdx, vfrom + i, pixel.get(hfrom + lineIdx, vfrom + i));
		}
	}
}


static bool check_point_first_cadrant(float startAngle, float endAngle, float tanStart, float tanEnd, float dh, float dv)
{
	if ((startAngle > 90.0f) || (dh < .0f) || (dv < .0f))
		return false;

	if ((dh != .0f) && (tanStart <= (dv / dh)))
		return (endAngle >= 90.0f) || ((dh != .0f) && ((dv / dh) <= tanEnd));

	return (dh == .0f) && (endAngle >= 90.0f);
}

static bool check_point_second_cadrant(float startAngle, float endAngle, float tanStart, float tanEnd, float dh, float dv)
{
	if ((startAngle > 180.0f) || (endAngle <= 90.0f) || (dh > .0f) || (dv < .0f))
		return false;

	if ((startAngle <= 90.0f) || ((dh != .0f) && (tanStart <= (dv / dh))))
		return (endAngle >= 180.0f) || ((dh != .0f) && ((dv / dh) <= tanEnd));

	return false;
}

static bool check_point_third_cadrant(float startAngle, float endAngle, float tanStart, float tanEnd, float dh, float dv)
{
	if ((startAngle > 270.0f) || (endAngle <= 180.0f) || (dh > .0f) || (dv > .0f))
		return false;

	if ((startAngle <= 180.0f) || ((dh != .0f) && (tanStart <= (dv / dh))))
		return (endAngle >= 270.0f) || ((dh != .0f) && ((dv / dh) <= tanEnd));

	return false;
}

static bool check_point_fourth_cadrant(float startAngle, float endAngle, float tanStart, float tanEnd, float dh, float dv)
{
	if ((endAngle <= 270.0f) || (dh < .0f) || (dv > .0f))
		return false;

	if ((startAngle <= 270.0f) || ((dh != .0f) && (tanStart <= (dv / dh))))
		return (dv / dh) <= tanEnd;

	return (dh == .0f) && (endAngle > 270.0f) && (startAngle <= 270.0f);
}

void DrawableCanvas::drawSemiCircle(int hcenter, int vcenter, int radius, int width, const PixelColorProvider& pixel, float startAngle, float endAngle) const
{
	if (width < 0)
	{
		radius += -width;
		width = -width;
	}

	if ((radius < 1) || (width == 0))
		return ;

	while (360.f < startAngle)
		startAngle -= 360.0f;

	while (startAngle < .0f)
		startAngle += 360.0f;

	while (endAngle < startAngle)
		endAngle += 360.0f;

	const float tanStartAngle = tan(MathCalcs::deg2rad(startAngle));
	const float tanEndAngle = tan(MathCalcs::deg2rad(endAngle));

	int first_ring_radius = radius;
	int second_ring_radius = max<int>(radius - width + 1, 0);

	int lastFirstV = first_ring_radius, lastSecondV = second_ring_radius;
	for (int it_h = 0; it_h <= first_ring_radius; ++it_h)
	{
		const int firstV = sqrt((float)(first_ring_radius * first_ring_radius - it_h * it_h));
		const int secondV = (it_h < second_ring_radius) ? sqrt((float)(second_ring_radius * second_ring_radius - it_h * it_h)) : 0;

		for (int it_v = lastFirstV + 1; it_v > firstV; --it_v)
		{
			const float cradius = sqrt((float)(it_h*it_h + it_v*it_v));
			float err = fabs(cradius - first_ring_radius);
			if (1.f < err)
				continue;

			err = 1.f - err;

			if (check_point_first_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, it_h, it_v))
				setPixel(hcenter + it_h, vcenter + it_v, pixel, err);

			if (check_point_fourth_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, it_h, -it_v))
				setPixel(hcenter + it_h, vcenter - it_v, pixel, err);

			if (check_point_second_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, -it_h, it_v))
				setPixel(hcenter - it_h, vcenter + it_v, pixel, err);

			if (check_point_third_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, -it_h, -it_v))
				setPixel(hcenter - it_h, vcenter - it_v, pixel, err);
		}

		for (int idv = firstV; (idv > secondV) || (idv == 0); --idv)
		{
			if (check_point_first_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, it_h, idv))
				setPixel(hcenter + it_h, vcenter + idv, pixel);

			if (check_point_fourth_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, it_h, -idv))
				setPixel(hcenter + it_h, vcenter - idv, pixel);

			if (check_point_second_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, -it_h, idv))
				setPixel(hcenter - it_h, vcenter + idv, pixel);

			if (check_point_third_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, -it_h, -idv))
				setPixel(hcenter - it_h, vcenter - idv, pixel);
		}


		int lastH = it_h - 1;
		if (lastH >= 0)
		{
			for (int idv = lastSecondV + 1; idv >= secondV; --idv)
			{
				const float cradius = sqrt((float)(lastH*lastH + idv*idv));
				float err = fabs(cradius - second_ring_radius);
				if (1.f < err)
					continue;

				err = 1.f - err;
				if (check_point_first_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, lastH, idv))
					setPixel(hcenter + lastH, vcenter + idv, pixel, err);

				if (check_point_fourth_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, lastH, -idv))
					setPixel(hcenter + lastH, vcenter - idv, pixel, err);

				if (check_point_second_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, -lastH, +idv))
					setPixel(hcenter - lastH, vcenter + idv, pixel, err);

				if (check_point_third_cadrant(startAngle, endAngle, tanStartAngle, tanEndAngle, -lastH, -idv))
					setPixel(hcenter - lastH, vcenter - idv, pixel, err);
			}
		}

		lastFirstV = firstV;
		lastSecondV = secondV;
	}
}

void DrawableCanvas::drawSemiCircle(int hcenter, int vcenter, int radius, int width, const PixelColorProvider& pixel, float startAngle, float endAngle, uint8_t dashFill, uint8_t dashSkip) const
{
	if (dashFill == 0)
		return ;
	else if (dashSkip == 0)
	{
		drawSemiCircle(hcenter, vcenter, radius, width, pixel, startAngle, endAngle);
		return ;
	}

	if (width < 0)
	{
		radius += -width;
		width = -width;
	}

	if ((radius < 1) || (width == 0))
		return ;

	if (endAngle < startAngle)
		endAngle += 360.0f;

	if (endAngle < startAngle)
		swap(endAngle, startAngle);

	auto fill = dashFill;
	const float step = 1.0f;
	for (float angle = startAngle; angle <= endAngle; angle += step)
	{
		if (fill == 0)
		{
			angle += (dashSkip - 1) * step;
			fill = dashFill;
			continue ;
		}
		else
			fill--;

		const float angleCos = MathCalcs::cos(angle);
		const float angleSin = MathCalcs::sin(angle);
		const float last = max<int>(radius - width, 0);

		for (float r = radius; r > last; r -= 1.0f)
		{
			const float h = hcenter + angleCos * r;
			const float v = vcenter + angleSin * r;

			const int hrnd = round(h);
			const int vrnd = round(v);

			setPixel(hrnd, vrnd, pixel, 1.0f - fabs(h - hrnd + v - vrnd) / 2.0f);
		}
	}
}

void DrawableCanvas::copyHorizontal(int hfrom, int vfrom, unsigned int width, int hto, int vto) const
{
	width = ((hfrom + width) <= m_Width) ? width : (m_Width - hfrom);
	width = ((hto + width) <= m_Width) ? width : (m_Width - hto);

	for (unsigned int i = 0; i < width; ++i)
		setPixel(hto + i, vto, getPixel(hfrom + i, vfrom));
}

void DrawableCanvas::copyVertical(int hfrom, int vfrom, unsigned int height, int hto, int vto) const
{
	height = ((vfrom + height) <= m_Height) ? height : (m_Height - vfrom);
	height = ((vto + height) <= m_Height) ? height : (m_Height - vto);

	for (unsigned int i = 0; i < height; ++i)
		setPixel(hto, vto + i, getPixel(hfrom, vfrom + i));
}

void DrawableCanvas::drawCanvas(const Canvas& canvas, int hto, int vto, int width, int height, Pixel skipColor, bool useSkipColor) const
{
	if (width <= 0)
		width = canvas.m_Width;

	if (height <= 0)
		height = canvas.m_Height;

	width = min(width, m_Width - hto);
	height = min(height, m_Height - vto);

	for (int v = 0; v < height; ++v)
		for (int h = 0; h < width; ++h)
		{
			const Pixel p = canvas.getPixel(h, v);

			if (!useSkipColor || (p != skipColor))
				setPixel(hto + h, vto + v, p);
		}
}

void DrawableCanvas::drawMaskCanvas(const Canvas& canvas, const Pixel maskColor, int hto, int vto, int width, int height) const
{
	if (width <= 0)
		width = canvas.m_Width;

	if (height <= 0)
		height = canvas.m_Height;

	width = min(width, m_Width - hto);
	height = min(height, m_Height - vto);

	for (int v = 0; v < height; ++v)
		for (int h = 0; h < width; ++h)
		{
			const Pixel p = getPixel(hto + h, vto + v);
			if (p != maskColor)
				continue;
			setPixel(hto + h, vto + v, canvas.getPixel(h, v));
		}
}

void DrawableCanvas::fillColor(const Pixel& color) const
{
	for (unsigned int v = 0; v < m_Height; v++)
		for (unsigned int h = 0; h < m_Width; h++)
			setPixel(h, v, color);
}

void DrawableCanvas::fillPatern(const Canvas& canvas, bool repeatHorizontally, bool repeatVertically, Pixel skipColor, bool useSkipColor) const
{
	if (repeatHorizontally & repeatVertically)
	{
		for (int v = 0; v < m_Height; ++v)
			for (int h = 0; h < m_Width; ++h)
			{
				const Pixel p = canvas.getPixel(h % canvas.m_Width, v % canvas.m_Height);
				if (!useSkipColor || (p != skipColor))
					setPixel(h, v, p);
			}
	}
	else if (repeatHorizontally)
	{
		const auto height = min(m_Height, canvas.m_Height);
		for (int v = 0; v < height; ++v)
			for (int h = 0; h < m_Width; ++h)
			{
				const Pixel p =  canvas.getPixel(h % canvas.m_Width, v);
				if (!useSkipColor || (p != skipColor))
					setPixel(h, v, p);
			}
	}
	else
	{
		const auto width = min(m_Width, canvas.m_Width);
		for (int v = 0; v < m_Height; ++v)
			for (int h = 0; h < width; ++h)
			{
				const Pixel p =  canvas.getPixel(h, v % canvas.m_Width);
				if (!useSkipColor || (p != skipColor))
					setPixel(h, v, p);
			}
	}
}

void DrawableCanvas::fillMaskPatern(const Canvas& canvas, const Pixel maskColor, bool repeatHorizontally, bool repeatVertically) const
{
	if (repeatHorizontally & repeatVertically)
	{
		for (int v = 0; v < m_Height; ++v)
			for (int h = 0; h < m_Width; ++h)
			{
				const Pixel p = getPixel(h, v);
				if (p != maskColor)
					continue;

				setPixel(h, v, canvas.getPixel(h % canvas.m_Width, v % canvas.m_Height));
			}
	}
	else if (repeatHorizontally)
	{
		const auto height = min(m_Height, canvas.m_Height);
		for (int v = 0; v < height; ++v)
			for (int h = 0; h < m_Width; ++h)
			{
				const Pixel p = getPixel(h, v);
				if (p != maskColor)
					continue;

				setPixel(h % canvas.m_Width, v, canvas.getPixel(h % canvas.m_Width, v % canvas.m_Height));
			}
	}
	else
	{
		const auto width = min(m_Width, canvas.m_Width);
		for (int v = 0; v < m_Height; ++v)
			for (int h = 0; h < width; ++h)
			{
				const Pixel p = getPixel(h, v);
				if (p != maskColor)
					continue;

				setPixel(h, v % canvas.m_Height, canvas.getPixel(h % canvas.m_Width, v % canvas.m_Height));
			}
	}
}

void DrawableCanvas::stretchCanvas(const Canvas& canvas, int hto, int vto, int width, int height) const
{
	if (width <= 0)
		width = m_Width;

	if (height <= 0)
		height = m_Height;

	if ((m_Height == canvas.m_Height) && (m_Width == canvas.m_Height))
	{
		drawCanvas(canvas);
		return ;
	}

	const float horFact = (float)width / (float)canvas.m_Width;
	const float verFact = (float)height / (float)canvas.m_Height;

	for (int v = 0; v < height; ++v)
	{
		const float vfact = (float)v / verFact;
		const int vfactFloor = vfact;
		for (int h = 0; h < width; ++h)
		{
			const float hfact = (float)h / horFact;
			const int hfactFloor = hfact;

			const Pixel ph1 = combinePixels(canvas.getPixel(hfactFloor, vfactFloor), canvas.getPixel(hfactFloor + 1, vfactFloor), hfact - hfactFloor);
			const Pixel ph2 = combinePixels(canvas.getPixel(hfactFloor, vfactFloor+1), canvas.getPixel(hfactFloor + 1, vfactFloor+1), hfact - hfactFloor);
			setPixel(hto + h, vto + v, combinePixels(ph1, ph2, vfact - vfactFloor));
		}
	}
}

void DrawableCanvas::writeText(const TUI_CHAR* string, const Font& font, const PixelColorProvider& color, int hto, int vto, float multiply, bool vertically) const
{
	for (const TUI_CHAR *ch = string; *ch != 0; ++ch)
	{
		const Character chCanvas(font.getCharacter((unsigned int)*ch));
		drawLetter(chCanvas, color, hto, vto, multiply);
		if (vertically)
			vto += ceil(chCanvas.m_Height * multiply) + 1;
		else
			hto += ceil(chCanvas.m_Width * multiply) + 1;
	}
}


static constexpr Pixel combineFontPixel(const Pixel p1, const Pixel p2, const float frac)
{
	const uint8_t blue = (1.0f -  frac) * p1.blue + frac * p2.blue;
	return Pixel(0, 0, blue);
}



void DrawableCanvas::drawLetter(const Character& c, const PixelColorProvider& color, int hto, int vto, const float multiply) const
{
	const int lastv = c.m_Height * multiply;
	const int lasth = c.m_Width * multiply;

	for (int v = 0; v < lastv; ++v)
	{
		const float vfact = v / multiply;
		const int vfactFloor = vfact;

		for (int h = 0; h < lasth; ++h)
		{
			const float hfact = h / multiply;
			const int hfactFloor = hfact;

			const Pixel ph1 = combineFontPixel(c.getPixel(hfactFloor, vfactFloor), c.getPixel(hfactFloor + 1, vfactFloor), hfact - hfactFloor);
			const Pixel ph2 = combineFontPixel(c.getPixel(hfactFloor, vfactFloor+1), c.getPixel(hfactFloor + 1, vfactFloor+1), hfact - hfactFloor);
			const Pixel mask = combineFontPixel(ph1, ph2, vfact - vfactFloor);

			const float factor = (float)mask.blue / Character::BLACK_PIXEL.blue;
			setPixel(h + hto, v + vto, maskPixel(color.get(h, v), getPixel(h + hto, v + vto), factor));
		}
	}
}

