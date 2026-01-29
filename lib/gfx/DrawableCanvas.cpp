/*
 * DrawableCanvas.cpp
 *
 *  Created on: Jun 17, 2020
 *      Author: IulianPopa
 */

#include <gfx/Canvas.h>
#include "gfx/DrawableCanvas.h"

#include <algorithm>
#include <math.h>

#include "../MathCalcs.h"

using namespace std;

static inline uint8_t fpart(float x) { return (x - floorf(x)) * 255; }
static inline uint8_t rfpart(float x) { return 255 - fpart(x); }

#include <cmath>
#include <algorithm>

static inline int outcode(float x, float y, float xmin, float ymin, float xmax, float ymax)
{
    const int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;
    int c = 0;

    if (x < xmin) c |= LEFT;
    else if (x > xmax) c |= RIGHT;

    if (y < ymin) c |= BOTTOM;
    else if (y > ymax) c |= TOP;

    return c;
}

bool clip_line(int* const hfrom, int* const vfrom,
               int* const hto,   int* const vto,
               const int maxWidth, const int maxHeight)
{
    // Keep your invariant
    if (*hfrom > *hto) { std::swap(*hfrom, *hto); std::swap(*vfrom, *vto); }

    const float xmin = 0.0f;
    const float ymin = 0.0f;
    const float xmax = float(maxWidth  - 1);
    const float ymax = float(maxHeight - 1);

    float x0 = float(*hfrom), y0 = float(*vfrom);
    float x1 = float(*hto),   y1 = float(*vto);

    int c0 = outcode(x0, y0, xmin, ymin, xmax, ymax);
    int c1 = outcode(x1, y1, xmin, ymin, xmax, ymax);

    for (;;)
    {
        if ((c0 | c1) == 0) {
            // both inside
            break;
        }
        if (c0 & c1) {
            // both outside on same side -> no intersection
            return false;
        }

        // Choose an outside endpoint
        int co = c0 ? c0 : c1;
        float x, y;

        const float dx = x1 - x0;
        const float dy = y1 - y0;

        // Intersect with the boundary indicated by co
        if (co & 8) { // TOP: y = ymax
            if (dy == 0.0f) return false;
            x = x0 + dx * (ymax - y0) / dy;
            y = ymax;
        } else if (co & 4) { // BOTTOM: y = ymin
            if (dy == 0.0f) return false;
            x = x0 + dx * (ymin - y0) / dy;
            y = ymin;
        } else if (co & 2) { // RIGHT: x = xmax
            if (dx == 0.0f) return false;
            y = y0 + dy * (xmax - x0) / dx;
            x = xmax;
        } else { // LEFT: x = xmin
            if (dx == 0.0f) return false;
            y = y0 + dy * (xmin - x0) / dx;
            x = xmin;
        }

        if (!std::isfinite(x) || !std::isfinite(y)) return false;

        // Replace the endpoint we clipped
        if (co == c0) {
            x0 = x; y0 = y;
            c0 = outcode(x0, y0, xmin, ymin, xmax, ymax);
        } else {
            x1 = x; y1 = y;
            c1 = outcode(x1, y1, xmin, ymin, xmax, ymax);
        }
    }

    // Convert back to int (your code truncates; rounding also works if you prefer)
    *hfrom = (int)x0; *vfrom = (int)y0;
    *hto   = (int)x1; *vto   = (int)y1;

    // Restore invariant
    if (*hfrom > *hto) { std::swap(*hfrom, *hto); std::swap(*vfrom, *vto); }

    // final safety
    return (*hfrom >= 0 && *hfrom < maxWidth  &&
            *hto   >= 0 && *hto   < maxWidth  &&
            *vfrom >= 0 && *vfrom < maxHeight &&
            *vto   >= 0 && *vto   < maxHeight);
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const Pixel& color) const
{
	if (clip_line(&hfrom, &vfrom, &hto, &vto, m_Width, m_Height) == false) return;

	if (hfrom == hto)
	{
		drawVLine(hfrom, vfrom, vto - vfrom, 1, color);
		return;
	}
	else if (vfrom == vto)
	{
		drawHLine(hfrom, vfrom, hto - hfrom, 1, color);
		return;
	}

	const float m = ((float)(vfrom - vto)) / (hfrom - hto);
	const float b =  vfrom - m * hfrom;
	const float aafactor = .5f;

	if (m > 0)
	{
		int h = hfrom, v = vfrom;
		while ((h <= hto) && (v <= vto) && (v >= vfrom))
		{
			const float hn = (v + 1 - b) / m;
			int deltaH = (int)(hn - h);
			if (deltaH + h >= hto) deltaH = hto - h;

			if (deltaH == 0)
			{
				const float vn = m * (h + 1) + b;
				int deltaV = (int)(vn - v);
				if (deltaV + v >= vto) deltaV = vto - v;
				if (deltaV <= 0) {
					setPixel(h, v, color);
					break;
				}

				int bound = v + deltaV / 2;
				const float d = 1.f / (bound - v + 1);

				for (int iv = v; iv <= bound; ++iv)
					setPixel(h, iv, color);

				h += 1;
				if (h <= hto) {
					float lum = .0f, step = d * aafactor;
					for (int iv = v; iv <= bound; ++iv, lum += step)
						setPixel(h, iv, color, lum);
				}

				v += deltaV;

				if (h <= hto) {
					for (int iv = bound + 1; iv < v; ++iv)
						setPixel(h, iv, color);
				}

				if (hfrom < h) {
					const int ht = h - 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int iv = bound; iv < v; ++iv, lum -= step)
						setPixel(ht, iv, color, lum);
				}
			}
			else
			{
				int bound = h + deltaH / 2;
				const float d = 1.f / (bound - h + 1);

				for (int ih = h; ih <= bound; ++ih)
					setPixel(ih, v, color);

				v += 1;
				if (v <= vto) {
					float lum = .0f, step = d * aafactor;
					for (int ih = h; ih <= bound; ++ih, lum += step) {
						setPixel(ih, v, color, lum);
					}
				}

				h += deltaH;
				if (v <= vto) {
					for (int ih = bound + 1; ih < h ; ++ih)
						setPixel(ih, v, color);
				}

				if (vfrom < v) {
					const int vt = v - 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int ih = bound; ih < h; ++ih, lum -= step)
						setPixel(ih, vt, color, lum);
				}

			}
		}
	}
	else
	{
		int h = hfrom, v = vfrom;
		while ((h <= hto) && (v >= vto) && (v <= vfrom))
		{
			const float hn = (v - 1 - b) / m;
			int deltaH = (int)(hn - h);
			if (deltaH + h >= hto) deltaH = hto - h;

			if (deltaH == 0)
			{
				const float vn = m * (h + 1) + b;
				int deltaV = (int)(v - vn);
				if (v - deltaV <= vto) deltaV = v - vto;
				if (deltaV <= 0) {
					setPixel(h, v, color);
					break;
				}

				int bound = v - deltaV / 2;
				const float d = 1.f / (v - bound + 1);

				for (int iv = v; iv >= bound; --iv)
					setPixel(h, iv, color);

				h += 1;
				if (h <= hto) {
					float lum = .0f, step = d * aafactor;
					for (int iv = v; iv >= bound; --iv, lum += step) {
						setPixel(h, iv, color, lum);
					}
				}

				v -= deltaV;

				for (int iv = bound - 1; iv > v; --iv)
					setPixel(h, iv, color);

				if (hfrom < h) {
					const int ht = h - 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int iv = bound; iv > v; --iv, lum -= step) {
						setPixel(ht, iv, color, lum);
					}
				}
			}
			else
			{
				int bound = h + deltaH / 2;
				const float d = 1.f / (bound - h + 1);

				for (int ih = h; ih <= bound; ++ih)
					setPixel(ih, v, color);

				if (v < vfrom) {
					const int vt = v + 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int ih = h; ih <= bound; ++ih, lum -= step) {
						setPixel(ih, vt, color, lum);
					}
				}

				h += deltaH;
				for (int ih = bound + 1; ih < h; ++ih)
					setPixel(ih, v, color);

				if (vto < v) {
					const int vt = v - 1;
					float lum = .0f, step = d * aafactor;
					for (int ih = bound; ih < h; ++ih, lum += step) {
						setPixel(ih, vt, color, lum);
					}
				}
				v -= 1;
			}
		}
	}
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& pixel) const
{
	if (clip_line(&hfrom, &vfrom, &hto, &vto, m_Width, m_Height) == false) return;

	if (hfrom == hto)
	{
		drawVLine(hfrom, vfrom, vto - vfrom, 1, pixel);
		return;
	}
	else if (vfrom == vto)
	{
		drawHLine(hfrom, vfrom, hto - hfrom, 1, pixel);
		return;
	}

	const float m = ((float)(vfrom - vto)) / (hfrom - hto);
	const float b =  vfrom - m * hfrom;
	const float aafactor = .5f;

	if (m > 0)
	{
		int h = hfrom, v = vfrom;
		while ((h <= hto) && (v <= vto) && (v >= vfrom))
		{
			const float hn = (v + 1 - b) / m;
			int deltaH = (int)(hn - h);
			if (deltaH + h >= hto) deltaH = hto - h;

			if (deltaH == 0)
			{
				const float vn = m * (h + 1) + b;
				int deltaV = (int)(vn - v);
				if (deltaV + v >= vto) deltaV = vto - v;
				if (deltaV <= 0) {
					setPixel(h, v, pixel);
					break;
				}

				int bound = v + deltaV / 2;
				const float d = 1.f / (bound - v + 1);

				for (int iv = v; iv <= bound; ++iv)
					setPixel(h, iv, pixel);

				h += 1;
				if (h <= hto) {
					float lum = .0f, step = d * aafactor;
					for (int iv = v; iv <= bound; ++iv, lum += step)
						setPixel(h, iv, pixel, lum);
				}

				v += deltaV;

				if (h <= hto) {
					for (int iv = bound + 1; iv < v; ++iv)
						setPixel(h, iv, pixel);
				}

				if (hfrom < h) {
					const int ht = h - 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int iv = bound; iv < v; ++iv, lum -= step)
						setPixel(ht, iv, pixel, lum);
				}
			}
			else
			{
				int bound = h + deltaH / 2;
				const float d = 1.f / (bound - h + 1);

				for (int ih = h; ih <= bound; ++ih)
					setPixel(ih, v, pixel);

				v += 1;
				if (v <= vto) {
					float lum = .0f, step = d * aafactor;
					for (int ih = h; ih <= bound; ++ih, lum += step) {
						setPixel(ih, v, pixel, lum);
					}
				}

				h += deltaH;
				if (v <= vto) {
					for (int ih = bound + 1; ih < h ; ++ih)
						setPixel(ih, v, pixel);
				}

				if (vfrom < v) {
					const int vt = v - 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int ih = bound; ih < h; ++ih, lum -= step)
						setPixel(ih, vt, pixel, lum);
				}

			}
		}
	}
	else
	{
		int h = hfrom, v = vfrom;
		while ((h <= hto) && (v >= vto) && (v <= vfrom))
		{
			const float hn = (v - 1 - b) / m;
			int deltaH = (int)(hn - h);
			if (deltaH + h >= hto) deltaH = hto - h;

			if (deltaH == 0)
			{
				const float vn = m * (h + 1) + b;
				int deltaV = (int)(v - vn);
				if (v - deltaV <= vto) deltaV = v - vto;
				if (deltaV <= 0) {
					setPixel(h, v, pixel);
					break;
				}

				int bound = v - deltaV / 2;
				const float d = 1.f / (v - bound + 1);

				for (int iv = v; iv >= bound; --iv)
					setPixel(h, iv, pixel);

				h += 1;
				if (h <= hto) {
					float lum = .0f, step = d * aafactor;
					for (int iv = v; iv >= bound; --iv, lum += step) {
						setPixel(h, iv, pixel, lum);
					}
				}

				v -= deltaV;

				for (int iv = bound - 1; iv > v; --iv)
					setPixel(h, iv, pixel);

				if (hfrom < h) {
					const int ht = h - 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int iv = bound; iv > v; --iv, lum -= step) {
						setPixel(ht, iv, pixel, lum);
					}
				}
			}
			else
			{
				int bound = h + deltaH / 2;
				const float d = 1.f / (bound - h + 1);

				for (int ih = h; ih <= bound; ++ih)
					setPixel(ih, v, pixel);

				if (v < vfrom) {
					const int vt = v + 1;
					float lum = 1.0f * aafactor, step = d * aafactor;
					for (int ih = h; ih <= bound; ++ih, lum -= step) {
						setPixel(ih, vt, pixel, lum);
					}
				}

				h += deltaH;
				for (int ih = bound + 1; ih < h; ++ih)
					setPixel(ih, v, pixel);

				if (vto < v) {
					const int vt = v - 1;
					float lum = .0f, step = d * aafactor;
					for (int ih = bound; ih < h; ++ih, lum += step) {
						setPixel(ih, vt, pixel, lum);
					}
				}
				v -= 1;
			}
		}
	}
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const
{
    if (dashFill == 0) return;

    if (dashSkip == 0) {
        drawLine(hfrom, vfrom, hto, vto, color);
        return;
    }

    if (!clip_line(&hfrom, &vfrom, &hto, &vto, m_Width, m_Height)) return;

    if (hfrom == hto) { drawVLine(hfrom, vfrom, vto - vfrom, 1, color); return; }
    if (vfrom == vto) { drawHLine(hfrom, vfrom, hto - hfrom, 1, color); return; }

    const float m = ((float)(vfrom - vto)) / (hfrom - hto);
    const float b =  vfrom - m * hfrom;
    const float aafactor = .5f;

    const int period = int(dashFill) + int(dashSkip);
    const int hstart = hfrom;

    auto dashOn = [&](int h) -> bool {
        int ph = (h - hstart) % period;
        if (ph < 0) ph += period;
        return ph < int(dashFill);
    };

    auto setDashed = [&](int h, int v) {
        if (dashOn(h)) setPixel(h, v, color);
    };

    auto setDashedAA = [&](int h, int v, float lum) {
        if (dashOn(h)) setPixel(h, v, color, lum);
    };

    if (m > 0)
    {
        int h = hfrom, v = vfrom;
        while ((h <= hto) && (v <= vto) && (v >= vfrom))
        {
            const float hn = (v + 1 - b) / m;
            int deltaH = (int)(hn - h);
            if (deltaH + h >= hto) deltaH = hto - h;

            if (deltaH == 0)
            {
                const float vn = m * (h + 1) + b;
                int deltaV = (int)(vn - v);
                if (deltaV + v >= vto) deltaV = vto - v;

                if (deltaV <= 0) {
                    setDashed(h, v);
                    break;
                }

                int bound = v + deltaV / 2;
                const float d = 1.f / (bound - v + 1);

                for (int iv = v; iv <= bound; ++iv)
                    setDashed(h, iv);

                h += 1;
                if (h <= hto) {
                    float lum = .0f, step = d * aafactor;
                    for (int iv = v; iv <= bound; ++iv, lum += step)
                        setDashedAA(h, iv, lum);
                }

                v += deltaV;

                if (h <= hto) {
                    for (int iv = bound + 1; iv < v; ++iv)
                        setDashed(h, iv);
                }

                if (hfrom < h) {
                    const int ht = h - 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int iv = bound; iv < v; ++iv, lum -= step)
                        setDashedAA(ht, iv, lum);
                }
            }
            else
            {
                int bound = h + deltaH / 2;
                const float d = 1.f / (bound - h + 1);

                for (int ih = h; ih <= bound; ++ih)
                    setDashed(ih, v);

                v += 1;
                if (v <= vto) {
                    float lum = .0f, step = d * aafactor;
                    for (int ih = h; ih <= bound; ++ih, lum += step)
                        setDashedAA(ih, v, lum);
                }

                h += deltaH;
                if (v <= vto) {
                    for (int ih = bound + 1; ih < h ; ++ih)
                        setDashed(ih, v);
                }

                if (vfrom < v) {
                    const int vt = v - 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int ih = bound; ih < h; ++ih, lum -= step)
                        setDashedAA(ih, vt, lum);
                }
            }
        }
    }
    else
    {
        int h = hfrom, v = vfrom;
        while ((h <= hto) && (v >= vto) && (v <= vfrom))
        {
            const float hn = (v - 1 - b) / m;
            int deltaH = (int)(hn - h);
            if (deltaH + h >= hto) deltaH = hto - h;

            if (deltaH == 0)
            {
                const float vn = m * (h + 1) + b;
                int deltaV = (int)(v - vn);

                // Clamp to vto (not 0)
                if (v - deltaV <= vto) deltaV = v - vto;

                if (deltaV <= 0) {
                    setDashed(h, v);
                    break;
                }

                int bound = v - deltaV / 2;
                const float d = 1.f / (v - bound + 1);

                for (int iv = v; iv >= bound; --iv)
                    setDashed(h, iv);

                h += 1;
                if (h <= hto) {
                    float lum = .0f, step = d * aafactor;
                    for (int iv = v; iv >= bound; --iv, lum += step)
                        setDashedAA(h, iv, lum);
                }

                v -= deltaV;

                for (int iv = bound - 1; iv > v; --iv)
                    setDashed(h, iv);

                if (hfrom < h) {
                    const int ht = h - 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int iv = bound; iv > v; --iv, lum -= step)
                        setDashedAA(ht, iv, lum);
                }
            }
            else
            {
                int bound = h + deltaH / 2;
                const float d = 1.f / (bound - h + 1);

                for (int ih = h; ih <= bound; ++ih)
                    setDashed(ih, v);

                if (v < vfrom) {
                    const int vt = v + 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int ih = h; ih <= bound; ++ih, lum -= step)
                        setDashedAA(ih, vt, lum);
                }

                h += deltaH;
                for (int ih = bound + 1; ih < h; ++ih)
                    setDashed(ih, v);

                if (vto < v) {
                    const int vt = v - 1;
                    float lum = .0f, step = d * aafactor;
                    for (int ih = bound; ih < h; ++ih, lum += step)
                        setDashedAA(ih, vt, lum);
                }
                v -= 1;
            }
        }
    }
}

void DrawableCanvas::drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& pixel, uint8_t dashFill, uint8_t dashSkip) const
{
    if (dashFill == 0) return;

    if (dashSkip == 0) {
        drawLine(hfrom, vfrom, hto, vto, pixel);
        return;
    }

    if (!clip_line(&hfrom, &vfrom, &hto, &vto, m_Width, m_Height)) return;

    if (hfrom == hto) { drawVLine(hfrom, vfrom, vto - vfrom, 1, pixel); return; }
    if (vfrom == vto) { drawHLine(hfrom, vfrom, hto - hfrom, 1, pixel); return; }

    const float m = ((float)(vfrom - vto)) / (hfrom - hto);
    const float b =  vfrom - m * hfrom;
    const float aafactor = .5f;

    const int period = int(dashFill) + int(dashSkip);
    const int hstart = hfrom;

    auto dashOn = [&](int h) -> bool {
        int ph = (h - hstart) % period;
        if (ph < 0) ph += period;
        return ph < int(dashFill);
    };

    auto setDashed = [&](int h, int v) {
        if (dashOn(h)) setPixel(h, v, pixel);
    };

    auto setDashedAA = [&](int h, int v, float lum) {
        if (dashOn(h)) setPixel(h, v, pixel, lum);
    };

    if (m > 0)
    {
        int h = hfrom, v = vfrom;
        while ((h <= hto) && (v <= vto) && (v >= vfrom))
        {
            const float hn = (v + 1 - b) / m;
            int deltaH = (int)(hn - h);
            if (deltaH + h >= hto) deltaH = hto - h;

            if (deltaH == 0)
            {
                const float vn = m * (h + 1) + b;
                int deltaV = (int)(vn - v);
                if (deltaV + v >= vto) deltaV = vto - v;

                if (deltaV <= 0) {
                    setDashed(h, v);
                    break;
                }

                int bound = v + deltaV / 2;
                const float d = 1.f / (bound - v + 1);

                for (int iv = v; iv <= bound; ++iv)
                    setDashed(h, iv);

                h += 1;
                if (h <= hto) {
                    float lum = .0f, step = d * aafactor;
                    for (int iv = v; iv <= bound; ++iv, lum += step)
                        setDashedAA(h, iv, lum);
                }

                v += deltaV;

                if (h <= hto) {
                    for (int iv = bound + 1; iv < v; ++iv)
                        setDashed(h, iv);
                }

                if (hfrom < h) {
                    const int ht = h - 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int iv = bound; iv < v; ++iv, lum -= step)
                        setDashedAA(ht, iv, lum);
                }
            }
            else
            {
                int bound = h + deltaH / 2;
                const float d = 1.f / (bound - h + 1);

                for (int ih = h; ih <= bound; ++ih)
                    setDashed(ih, v);

                v += 1;
                if (v <= vto) {
                    float lum = .0f, step = d * aafactor;
                    for (int ih = h; ih <= bound; ++ih, lum += step)
                        setDashedAA(ih, v, lum);
                }

                h += deltaH;
                if (v <= vto) {
                    for (int ih = bound + 1; ih < h ; ++ih)
                        setDashed(ih, v);
                }

                if (vfrom < v) {
                    const int vt = v - 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int ih = bound; ih < h; ++ih, lum -= step)
                        setDashedAA(ih, vt, lum);
                }
            }
        }
    }
    else
    {
        int h = hfrom, v = vfrom;
        while ((h <= hto) && (v >= vto) && (v <= vfrom))
        {
            const float hn = (v - 1 - b) / m;
            int deltaH = (int)(hn - h);
            if (deltaH + h >= hto) deltaH = hto - h;

            if (deltaH == 0)
            {
                const float vn = m * (h + 1) + b;
                int deltaV = (int)(v - vn);

                // IMPORTANT: clamp to vto (not 0)
                if (v - deltaV <= vto) deltaV = v - vto;

                if (deltaV <= 0) {
                    setDashed(h, v);
                    break;
                }

                int bound = v - deltaV / 2;
                const float d = 1.f / (v - bound + 1);

                for (int iv = v; iv >= bound; --iv)
                    setDashed(h, iv);

                h += 1;
                if (h <= hto) {
                    float lum = .0f, step = d * aafactor;
                    for (int iv = v; iv >= bound; --iv, lum += step)
                        setDashedAA(h, iv, lum);
                }

                v -= deltaV;

                for (int iv = bound - 1; iv > v; --iv)
                    setDashed(h, iv);

                if (hfrom < h) {
                    const int ht = h - 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int iv = bound; iv > v; --iv, lum -= step)
                        setDashedAA(ht, iv, lum);
                }
            }
            else
            {
                int bound = h + deltaH / 2;
                const float d = 1.f / (bound - h + 1);

                for (int ih = h; ih <= bound; ++ih)
                    setDashed(ih, v);

                if (v < vfrom) {
                    const int vt = v + 1;
                    float lum = 1.0f * aafactor, step = d * aafactor;
                    for (int ih = h; ih <= bound; ++ih, lum -= step)
                        setDashedAA(ih, vt, lum);
                }

                h += deltaH;
                for (int ih = bound + 1; ih < h; ++ih)
                    setDashed(ih, v);

                if (vto < v) {
                    const int vt = v - 1;
                    float lum = .0f, step = d * aafactor;
                    for (int ih = bound; ih < h; ++ih, lum += step)
                        setDashedAA(ih, vt, lum);
                }
                v -= 1;
            }
        }
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


void DrawableCanvas::drawCircle(int hcenter, int vcenter, int radius, uint8_t width, const Pixel& color) const
{
	if (radius < 0)
		return;

	else if (radius == 0)
	{
		if ((0 <= hcenter) && (hcenter < m_Width )
			&& (0 <= vcenter) && (vcenter < m_Height))
		{
			setPixel(hcenter, vcenter, color);
		}
		return;
	}

	if (width > radius) width = radius;

	const uint_t radius_l = (radius - width + 1);
	const uint_t uperbound = (radius + 1) * (radius + 1);
	const uint_t lowerbound = (radius_l - 1)* (radius_l - 1);
	const uint_t sqradius = radius * radius;
	const uint_t sqradius_l = radius_l * radius_l;
	const float inv2radius = 1.f / (2 * radius); //an extra for 2 is the scaling factor
	const float inv2radius_l = 1.f / (2 * radius_l); //an extra for 2 is the scaling factor

	int hItFrom = hcenter - radius;
	if (hItFrom < 0) { hItFrom = 0; if (abs(hItFrom - hcenter) >= radius) return;}
	int hItTo = hcenter + radius;
	if (hItTo >= m_Width) {  hItTo = m_Width - 1; if (abs(hItTo - hcenter) >= radius) return; }

	int vItFrom = vcenter - radius;
	if (vItFrom < 0) { vItFrom = 0; if (abs(vItFrom - vcenter) >= radius) return; }

	int vItTo = vcenter + radius;
	if (vItTo >= m_Height) { vItTo = m_Height - 1; if (abs(vItTo - vcenter) >= radius) return; }

	for (int h = hItFrom; h <= hItTo; ++h)
	{
		for (int v = vItFrom; v <= vItTo; ++v)
		{
 			const uint_t sqlen = (h - hcenter) * (h - hcenter) + (v - vcenter) * (v - vcenter);
			if ((sqlen < lowerbound) || (uperbound < sqlen))
				continue;

			if (sqradius <= sqlen)
			{
				const float extra = (sqlen - sqradius) * inv2radius;
				if (extra < 0.25f)
					setPixel(h, v, color);

				else
					setPixel(h, v, color, 1.0f - extra);
			}
			else if (sqlen <= sqradius_l)
			{
				const float extra = -(sqlen - sqradius_l) * inv2radius_l;
				if (extra < .5f)
					setPixel(h, v, color, 1.0f - extra);
			}
			else
				setPixel(h, v, color);
		}
	}
}

void DrawableCanvas::drawCircle(int hcenter, int vcenter, int radius, uint8_t width, const PixelColorProvider& pixel) const
{
	if (radius < 0)
		return;

	else if (radius == 0)
	{
		if ((0 <= hcenter) && (hcenter < m_Width )
			&& (0 <= vcenter) && (vcenter < m_Height))
		{
			setPixel(hcenter, vcenter, pixel);
		}
		return;
	}

	const uint_t uperbound = radius * (radius + 1);   // (x + 0.5)^2
	const uint_t lowerbound = radius * (radius - 1);  // (x - 0.5)^2
	const uint_t sqradius = radius * radius;
	const uint_t inv2radius = 1.f / (2 * (radius - 1)); //an extra for 2 is the scaling factor

	int hItFrom = hcenter - radius;
	if (hItFrom < 0) { hItFrom = 0; if (abs(hItFrom - hcenter) >= radius) return;}
	int hItTo = hcenter + radius;
	if (hItTo >= m_Width) {  hItTo = m_Width - 1; if (abs(hItTo - hcenter) >= radius) return; }

	int vItFrom = vcenter - radius;
	if (vItFrom < 0) { vItFrom = 0; if (abs(vItFrom - vcenter) >= radius) return; }

	int vItTo = vcenter + radius;
	if (vItTo >= m_Height) { vItTo = m_Height - 1; if (abs(vItTo - vcenter) >= radius) return; }

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
				setPixel(h, v, pixel, extra);
			}
			else
			{
				const float extra = 1.0f + (sqlen - sqradius) * inv2radius;
				setPixel(h, v, pixel, extra);
			}
		}
	}
}

enum Cadrant
{
	First,
	Second,
	Third,
	Fourth
};

static inline Cadrant getCadrant(int xc, int yc, int xp, int yp)
{
	if (yp >= yc)
		return (xp >= xc) ? Cadrant::First : Cadrant::Second;

	else
		return (xp < xc) ? Cadrant::Third : Cadrant::Fourth;
}

static inline Cadrant getCadrant(float angle)
{
	if (angle <= 90.0f)
		return Cadrant::First;

	else if (angle <= 180.0f)
		return Cadrant::Second;

	else if (angle <= 270.0f)
		return Cadrant::Third;

	else
		return Cadrant::Fourth;
}

static inline bool is_point_over_line(float m, float b, int x, int y)
{
	return (y + 1 > (int)(m * x + b));
}

static inline bool is_point_under_line(float m, float b, int x, int y)
{
	return (y <= (int)(m * x + b));
}


static inline bool is_point_in_arc(Cadrant cadAngle, Cadrant cadPoint, float ms, float bs, int h, int v)
{
	if (cadPoint < cadAngle)
		return true;

	else if (cadPoint > cadAngle)
		return false;

	const bool underLine = is_point_under_line(ms, bs, h, v);
	return ((cadAngle == Cadrant::Fourth) | (cadAngle == Cadrant::First))
			? is_point_under_line(ms, bs, h, v)
			: is_point_over_line(ms, bs, h, v);
}

void DrawableCanvas::drawSemiCircle(int hcenter, int vcenter, int radius, int width, const Pixel& color, float startAngle, float endAngle) const
{
	bool inverseAngles = false;

	if (radius < 0)
		return;

	else if (radius == 0)
	{
		if ((0 <= hcenter) && (hcenter < m_Width )
			&& (0 <= vcenter) && (vcenter < m_Height))
		{
			setPixel(hcenter, vcenter, color);
		}
		return;
	}

	if (width > radius) width = radius;

	startAngle = MathCalcs::normalizeDeg(startAngle);
	endAngle = MathCalcs::normalizeDeg(endAngle);
	if (endAngle >= startAngle) inverseAngles = true;

	const Cadrant startCadrant = getCadrant(startAngle);
	const Cadrant endCadrant = getCadrant(endAngle);

	const float ms = ::tan(MathCalcs::deg2rad(startAngle)), bs = vcenter - ms * hcenter;
	const float me = ::tan(MathCalcs::deg2rad(endAngle)), be = vcenter - me * hcenter;

    auto is_point_in_valid = [&](int h, int v) -> bool {
    	const auto pointCadrant = getCadrant(hcenter, vcenter, h, v);
    	const bool test1 = is_point_in_arc(startCadrant, pointCadrant, ms, bs, h, v);
    	const bool test2 = is_point_in_arc(endCadrant, pointCadrant, me, be, h, v);
    	const bool result = test1 ^ test2;
    	return inverseAngles ? !result : result;
    };

	const uint_t radius_l = (radius - width + 1);
	const uint_t uperbound = (radius + 1) * (radius + 1);
	const uint_t lowerbound = (radius_l - 1)* (radius_l - 1);
	const uint_t sqradius = radius * radius;
	const uint_t sqradius_l = radius_l * radius_l;
	const float inv2radius = 1.f / (2 * radius); //an extra for 2 is the scaling factor
	const float inv2radius_l = 1.f / (2 * radius_l); //an extra for 2 is the scaling factor

	int hItFrom = hcenter - radius;
	if (hItFrom < 0) { hItFrom = 0; if (abs(hItFrom - hcenter) >= radius) return;}
	int hItTo = hcenter + radius;
	if (hItTo >= m_Width) {  hItTo = m_Width - 1; if (abs(hItTo - hcenter) >= radius) return; }

	int vItFrom = vcenter - radius;
	if (vItFrom < 0) { vItFrom = 0; if (abs(vItFrom - vcenter) >= radius) return; }

	int vItTo = vcenter + radius;
	if (vItTo >= m_Height) { vItTo = m_Height - 1; if (abs(vItTo - vcenter) >= radius) return; }

	for (int h = hItFrom; h <= hItTo; ++h)
	{
		for (int v = vItFrom; v <= vItTo; ++v)
		{
 			const uint_t sqlen = (h - hcenter) * (h - hcenter) + (v - vcenter) * (v - vcenter);
			if ((sqlen < lowerbound) || (uperbound < sqlen) || !is_point_in_valid(h, v))
				continue;

			if (sqradius <= sqlen)
			{
				const float extra = (sqlen - sqradius) * inv2radius;
				if (extra < 0.25f)
					setPixel(h, v, color);

				else
					setPixel(h, v, color, 1.0f - extra);
			}
			else if (sqlen <= sqradius_l)
			{
				const float extra = -(sqlen - sqradius_l) * inv2radius_l;
				if (extra < .5f)
					setPixel(h, v, color, 1.0f - extra);
			}
			else
				setPixel(h, v, color);
		}
	}
}


void DrawableCanvas::drawSemiCircle(int hcenter, int vcenter, int radius, int width, const PixelColorProvider& pixel, float startAngle, float endAngle) const
{
	bool inverseAngles = false;

	if (radius < 0)
		return;

	else if (radius == 0)
	{
		if ((0 <= hcenter) && (hcenter < m_Width )
			&& (0 <= vcenter) && (vcenter < m_Height))
		{
			setPixel(hcenter, vcenter, pixel);
		}
		return;
	}

	if (width > radius) width = radius;

	startAngle = MathCalcs::normalizeDeg(startAngle);
	endAngle = MathCalcs::normalizeDeg(endAngle);
	if (endAngle >= startAngle) inverseAngles = true;

	const Cadrant startCadrant = getCadrant(startAngle);
	const Cadrant endCadrant = getCadrant(endAngle);

	const float ms = ::tan(MathCalcs::deg2rad(startAngle)), bs = vcenter - ms * hcenter;
	const float me = ::tan(MathCalcs::deg2rad(endAngle)), be = vcenter - me * hcenter;

    auto is_point_in_valid = [&](int h, int v) -> bool {
    	const auto pointCadrant = getCadrant(hcenter, vcenter, h, v);
    	const bool test1 = is_point_in_arc(startCadrant, pointCadrant, ms, bs, h, v);
    	const bool test2 = is_point_in_arc(endCadrant, pointCadrant, me, be, h, v);
    	const bool result = test1 ^ test2;
    	return inverseAngles ? !result : result;
    };

	const uint_t radius_l = (radius - width + 1);
	const uint_t uperbound = (radius + 1) * (radius + 1);
	const uint_t lowerbound = (radius_l - 1)* (radius_l - 1);
	const uint_t sqradius = radius * radius;
	const uint_t sqradius_l = radius_l * radius_l;
	const float inv2radius = 1.f / (2 * radius); //an extra for 2 is the scaling factor
	const float inv2radius_l = 1.f / (2 * radius_l); //an extra for 2 is the scaling factor

	int hItFrom = hcenter - radius;
	if (hItFrom < 0) { hItFrom = 0; if (abs(hItFrom - hcenter) >= radius) return;}
	int hItTo = hcenter + radius;
	if (hItTo >= m_Width) {  hItTo = m_Width - 1; if (abs(hItTo - hcenter) >= radius) return; }

	int vItFrom = vcenter - radius;
	if (vItFrom < 0) { vItFrom = 0; if (abs(vItFrom - vcenter) >= radius) return; }

	int vItTo = vcenter + radius;
	if (vItTo >= m_Height) { vItTo = m_Height - 1; if (abs(vItTo - vcenter) >= radius) return; }

	for (int h = hItFrom; h <= hItTo; ++h)
	{
		for (int v = vItFrom; v <= vItTo; ++v)
		{
 			const uint_t sqlen = (h - hcenter) * (h - hcenter) + (v - vcenter) * (v - vcenter);
			if ((sqlen < lowerbound) || (uperbound < sqlen) || !is_point_in_valid(h, v))
				continue;

			if (sqradius <= sqlen)
			{
				const float extra = (sqlen - sqradius) * inv2radius;
				if (extra < 0.25f)
					setPixel(h, v, pixel);

				else
					setPixel(h, v, pixel, 1.0f - extra);
			}
			else if (sqlen <= sqradius_l)
			{
				const float extra = -(sqlen - sqradius_l) * inv2radius_l;
				if (extra < .5f)
					setPixel(h, v, pixel, 1.0f - extra);
			}
			else
				setPixel(h, v, pixel);
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

