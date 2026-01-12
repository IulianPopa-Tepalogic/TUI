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

static inline uint8_t fpart(float x) { return (x - floorf(x)) * 255; }
static inline uint8_t rfpart(float x) { return 255 - fpart(x); }

#include <cmath>
#include <algorithm>

static inline bool cliptest(float p, float q, float& u1, float& u2)
{
    if (p == 0.0f)
    	return q >= 0.0f;

    float r = q / p;
    if (p < 0.0f) {
    	if (r > u2)
    		return false;
    	if (r > u1) u1 = r;
    }
    else {
    	if (r < u1)
    		return false;
    	if (r < u2) u2 = r;
    }
    return true;
}

bool clip_line(int* const hfrom, int* const vfrom, int* const hto, int* const vto)
{
    int x0 = *hfrom, y0 = *vfrom, x1 = *hto, y1 = *vto;

    const int xmin = 0;
    const int ymin = 0;
    const int xmax = SCREEN_WIDTH_SIZE  - 1;
    const int ymax = SCREEN_HEIGHT_SIZE - 1;

    float u1 = 0.0f, u2 = 1.0f;
    float dx = x1 - x0;
    float dy = y1 - y0;


    if (!cliptest(-dx, x0 - xmin, u1, u2)) return false;
    if (!cliptest( dx, xmax - x0, u1, u2)) return false;
    if (!cliptest(-dy, y0 - ymin, u1, u2)) return false;
    if (!cliptest( dy, ymax - y0, u1, u2)) return false;

    if (u2 < 1.0f) {
        x1 = roundf(x0 + u2 * dx);
        y1 = roundf(y0 + u2 * dy);
    }
    if (u1 > 0.0f) {
        x0 = roundf(x0 + u1 * dx);
        y0 = roundf(y0 + u1 * dy);
    }

    *hfrom = x0; *vfrom = y0;
    *hto   = x1; *vto   = y1;

    return true;
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const Pixel& color) const
{
	if (! clip_line(&hfrom, &vfrom, &hto, &vto)) return;

    const bool steep = std::abs(vto - vfrom) > std::abs(hto - hfrom);
    if (steep) { std::swap(hfrom, vfrom); std::swap(hto, vto); }

    if (hfrom > hto) { std::swap(hfrom, hto); std::swap(vfrom, vto); }

    const float dx = float(hto - hfrom);
    const float dy = float(vto - vfrom);
    const float gradient = dy / dx;

    float y = float(vfrom);
    for (int x = hfrom; x <= hto; ++x)
    {
        const int iy = std::min((int)y, (steep ? SCREEN_WIDTH_SIZE : SCREEN_HEIGHT_SIZE) - 2);

        const uint8_t frac = fpart(y);
        const uint8_t rfpart_y = rfpart(y);

        if (!steep)
        {
            setPixel(x, iy,     color, rfpart_y);
            setPixel(x, iy + 1, color, frac);
        }
        else
        {
            setPixel(iy,     x, color, rfpart_y);
            setPixel(iy + 1, x, color, frac);
        }

        y += gradient;
    }
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& pixel) const
{
	if (! clip_line(&hfrom, &vfrom, &hto, &vto)) return;

    const bool steep = std::abs(vto - vfrom) > std::abs(hto - hfrom);
    if (steep) { std::swap(hfrom, vfrom); std::swap(hto, vto); }

    if (hfrom > hto) { std::swap(hfrom, hto); std::swap(vfrom, vto); }

    const float dx = float(hto - hfrom);
    const float dy = float(vto - vfrom);
    const float gradient = dy / dx;

    float y = float(vfrom);
    for (int x = hfrom; x <= hto; ++x)
    {
        const int iy = std::min((int)y, (steep ? SCREEN_WIDTH_SIZE : SCREEN_HEIGHT_SIZE) - 2);

        const uint8_t frac = fpart(y);
        const uint8_t rfpart_y = rfpart(y);

        if (!steep)
        {
            setPixel(x, iy,     pixel, rfpart_y);
            setPixel(x, iy + 1, pixel, frac);
        }
        else
        {
            setPixel(iy,     x, pixel, rfpart_y);
            setPixel(iy + 1, x, pixel, frac);
        }

        y += gradient;
    }
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const
{
    if (dashFill == 0) return;

    if (! clip_line(&hfrom, &vfrom, &hto, &vto)) return;

    const bool steep = std::abs(vto - vfrom) > std::abs(hto - hfrom);
    if (steep) { std::swap(hfrom, vfrom); std::swap(hto, vto); }

    if (hfrom > hto) { std::swap(hfrom, hto); std::swap(vfrom, vto); }

    const float dx = float(hto - hfrom);
    const float dy = float(vto - vfrom);
    const float gradient = dy / dx;

    const int pattern = dashFill + dashSkip;
    float y = float(vfrom);
    for (int x = hfrom; x <= hto; ++x)
    {
        if (dashSkip > 0)
        {
            int t = x - hfrom;
            if ((t % pattern) >= dashFill)
            {
                y += gradient;
                continue;
            }
        }

        const int iy = std::min((int)y, (steep ? SCREEN_WIDTH_SIZE : SCREEN_HEIGHT_SIZE) - 2);
        const uint8_t frac = fpart(y);
        const uint8_t rfpart_y = rfpart(y);

        if (!steep)
        {
            setPixel(x, iy,     color, rfpart_y);
            setPixel(x, iy + 1, color, frac);
        }
        else
        {
            setPixel(iy,     x, color, rfpart_y);
            setPixel(iy + 1, x, color, frac);
        }

        y += gradient;
    }
}


void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
    if (dashFill == 0) return;

    if (! clip_line(&hfrom, &vfrom, &hto, &vto)) return;

    const bool steep = std::abs(vto - vfrom) > std::abs(hto - hfrom);
    if (steep) { std::swap(hfrom, vfrom); std::swap(hto, vto); }

    if (hfrom > hto) { std::swap(hfrom, hto); std::swap(vfrom, vto); }

    const float dx = float(hto - hfrom);
    const float dy = float(vto - vfrom);
    const float gradient = dy / dx;

    const int pattern = dashFill + dashSkip;
    float y = float(vfrom);
    for (int x = hfrom; x <= hto; ++x)
    {
        if (dashSkip > 0)
        {
            int t = x - hfrom;
            if ((t % pattern) >= dashFill)
            {
                y += gradient;
                continue;
            }
        }

        const int iy = std::min((int)y, (steep ? SCREEN_WIDTH_SIZE : SCREEN_HEIGHT_SIZE) - 2);
        const uint8_t frac = fpart(y);
        const uint8_t rfpart_y = rfpart(y);

        if (!steep)
        {
            setPixel(x, iy,     pixel, rfpart_y);
            setPixel(x, iy + 1, pixel, frac);
        }
        else
        {
            setPixel(iy,     x, pixel, rfpart_y);
            setPixel(iy + 1, x, pixel, frac);
        }

        y += gradient;
    }
}

void DrawableCanvas::drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const Pixel& color) const
{
	if (length < 0)
	{
		length = -length;
		clockWiseDeg += 180;
	}

	clockWiseDeg = MathCalcs::normalizeDeg(clockWiseDeg);

	const auto hto = hfrom + MathCalcs::cos(clockWiseDeg) * length;
	const auto vto = vfrom + MathCalcs::sin(clockWiseDeg) * length;

	drawLine(hfrom, vfrom, hto, vto, color);
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


void DrawableCanvas::drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const
{
	if (length < 0)
	{
		length = -length;
		clockWiseDeg += 180;
	}

	clockWiseDeg = MathCalcs::normalizeDeg(clockWiseDeg);

	const auto hto = hfrom + MathCalcs::cos(clockWiseDeg) * length;
	const auto vto = vfrom + MathCalcs::sin(clockWiseDeg) * length;

	drawLine (hfrom, vfrom, hto, vto, color, dashFill, dashSkip);
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


void DrawableCanvas::drawHLine (int hfrom, int vfrom, int length, int width, const Pixel& color) const
{
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

	int lineIdx = width;
	while (--lineIdx >= 0)
	{
		int i = length;
		while (--i >= 0)
			setPixel(hfrom + i, vfrom + lineIdx, color);
	}
}


void DrawableCanvas::drawHLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel) const
{
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

	int lineIdx = width;
	while (--lineIdx >= 0)
	{
		int i = length;
		while (--i >= 0)
			setPixel(hfrom + i, vfrom + lineIdx, pixel);
	}
}


void DrawableCanvas::drawHLine (int hfrom, int vfrom, int length, int width, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const
{
	if ((dashFill <= 0) || (dashSkip < 0))
		return ;

	else if (dashSkip == 0)
	{
		drawHLine(hfrom, vfrom, length, width, color);
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

	if (length <= 0 || width <= 0)
		return;

	for (int lineIdx = 0; lineIdx < width; lineIdx++)
	{
		auto fill = dashFill;
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

			setPixel(hfrom + i, vfrom + lineIdx, color);
		}
	}
}


void DrawableCanvas::drawHLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if ((dashFill <= 0) || (dashSkip < 0))
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

	if (length <= 0 || width <= 0)
		return;

	for (int lineIdx = 0; lineIdx < width; lineIdx++)
	{
		auto fill = dashFill;
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

			setPixel(hfrom + i, vfrom + lineIdx, pixel);
		}
	}
}


void DrawableCanvas::drawVLine (int hfrom, int vfrom, int length, int width, const Pixel& color) const
{
    if (length < 0)
    {
    	vfrom += length;
    	length = -length;
    }

    if (width < 0)
    {
    	hfrom += width;
    	width  = -width;
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

	int lineIdx = width;
	while (--lineIdx >= 0) {
		int i = length;
		while (--i >= 0)
			setPixel(hfrom + lineIdx, vfrom + i, color);
	}
}


void DrawableCanvas::drawVLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel) const
{
    if (length < 0)
    {
    	vfrom += length;
    	length = -length;
    }

    if (width < 0)
    {
    	hfrom += width;
    	width  = -width;
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

	int lineIdx = width;
	while (--lineIdx >= 0) {
		int i = length;
		while (--i >= 0)
			setPixel(hfrom + lineIdx, vfrom + i, pixel);
	}
}


void DrawableCanvas::drawVLine (int hfrom, int vfrom, int length, int width, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const
{
	if ((dashFill <= 0) || (dashSkip < 0))
		return ;

    if (length < 0)
    {
    	vfrom += length;
    	length = -length;
    }

    if (width < 0)
    {
    	hfrom += width;
    	width  = -width;
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
	{
		auto fill = dashFill;
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

			setPixel(hfrom + lineIdx, vfrom + i, color);
		}
	}
}


void DrawableCanvas::drawVLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
	if ((dashFill <= 0) || (dashSkip < 0))
		return ;

    if (length < 0)
    {
    	vfrom += length;
    	length = -length;
    }

    if (width < 0)
    {
    	hfrom += width;
    	width  = -width;
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
	{
		auto fill = dashFill;
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

			setPixel(hfrom + lineIdx, vfrom + i, pixel);
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


void DrawableCanvas::drawCircle(int hcenter, int vcenter, int radius, const Pixel& color) const
{
	if (radius < 0)
		return;
	else if (radius == 0)
	{
		if ((0 <= hcenter) && (hcenter < SCREEN_WIDTH_SIZE )
			&& (0 <= vcenter) && (vcenter < SCREEN_HEIGHT_SIZE))
		{
			setPixel(hcenter, vcenter, color);
		}
		return;
	}

	const uint_t uperbound = radius * (radius + 1);
	const uint_t lowerbound = radius * (radius - 1);
	const uint_t sqradius = radius * radius;
	const uint_t inv2radius = 1.f / (2 * radius);

	int hItFrom = hcenter - radius;
	if (hItFrom < 0) { hItFrom = 0; if (abs(hItFrom - hcenter) >= radius) return;}
	int hItTo = hcenter + radius;
	if (hItTo >= SCREEN_WIDTH_SIZE) {  hItTo = SCREEN_WIDTH_SIZE - 1; if (abs(hItTo - hcenter) >= radius) return; }

	int vItFrom = vcenter - radius;
	if (vItFrom < 0) { vItFrom = 0; if (abs(vItFrom - vcenter) >= radius) return; }

	int vItTo = vcenter + radius;
	if (vItTo >= SCREEN_HEIGHT_SIZE) { vItTo = SCREEN_HEIGHT_SIZE - 1; if (abs(vItTo - vcenter) >= radius) return; }

	for (int h = hItFrom; h <= hItTo; ++h)
	{
		for (int v = vItFrom; v <= vItTo; ++v)
		{
 			const uint_t sqlen = (h - hcenter) * (h - hcenter) + (v - vcenter) * (v - vcenter);
			if ((sqlen <= lowerbound) || (sqlen >= uperbound))
				continue;

			if (sqradius < sqlen)
			{
				const float extra = 1.0f - (sqlen - sqradius) * inv2radius ;
				setPixel(h, v, color, (uint8_t)(extra * 255));
			}
			else
			{
				const float extra = 1.0f + (sqlen - sqradius) * inv2radius;
				setPixel(h, v, color, (uint8_t)(extra * 255));
			}
		}
	}
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

