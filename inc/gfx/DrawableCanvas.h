/*
 * DrawableCanvas.h
 *
 *  Created on: Jun 17, 2020
 *      Author: IulianPopa
 */

#pragma once

#include "gfx/Canvas.h"
#include "gfx/Font.h"
#include "Character.h"

class PixelColorProvider;

class DrawableCanvas : public Canvas {
public:
	DrawableCanvas(unsigned int width, unsigned int height)
		: Canvas(width, height)
	{}

	virtual ~DrawableCanvas() = default;

	virtual void drawLine(int hfrom, int vfrom, int hto, int vto, const Pixel& color) const;
	virtual void drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& provider) const;

	virtual void drawLine(int hfrom, int vfrom, int hto, int vto, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const;
	virtual void drawLine(int hfrom, int vfrom, int hto, int vto, const PixelColorProvider& provider, uint8_t dashFill, uint8_t dashSkip) const;

	virtual void drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const Pixel& color) const;
	virtual void drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const PixelColorProvider& provider) const;

	virtual void drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const;
	virtual void drawSlopedLine(int hfrom, int vfrom, int length, float clockWiseDeg, const PixelColorProvider& provider, uint8_t dashFill, uint8_t dashSkip) const;

	virtual void drawHLine (int hfrom, int vfrom, int length, int width, const Pixel& color) const;
	virtual void drawHLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& provider) const;

	virtual void drawHLine (int hfrom, int vfrom, int length, int width, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const;
	virtual void drawHLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& provider, uint8_t dashFill, uint8_t dashSkip) const;

	virtual void drawVLine (int hfrom, int vfrom, int length, int width, const Pixel& color) const;
	virtual void drawVLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& provider) const;

	virtual void drawVLine (int hfrom, int vfrom, int length, int width, const Pixel& color, uint8_t dashFill, uint8_t dashSkip) const;
	virtual void drawVLine (int hfrom, int vfrom, int length, int width, const PixelColorProvider& provider, uint8_t dashFill, uint8_t dashSkip) const;

	virtual void drawSemiCircle(int hcenter, int vcenter, int radius, int width, const PixelColorProvider& provider, float startAngle = 0.0, float endAngle = 360.0) const;
	virtual void drawSemiCircle(int hcenter, int vcenter, int radius, int width, const PixelColorProvider& provider, float startAngle, float endAngle, uint8_t dashFill, uint8_t dashSkip) const;

	virtual void copyHorizontal(int hfrom, int vfrom, unsigned int width, int hto, int vto) const;
	virtual void copyVertical(int hfrom, int vfrom, unsigned int height, int hto, int vto) const;

	virtual void drawCanvas(const Canvas& canvas, int hto = 0, int vto = 0, int width = 0, int height = 0, Pixel skipColor = Pixel(), bool useSkipColor = false) const;
	virtual void drawMaskCanvas(const Canvas& canvas, const Pixel maskColor, int hto = 0, int vto = 0, int width = 0, int height = 0) const;
	virtual void fillColor(const Pixel& color) const;
	virtual void fillPatern(const Canvas& canvas, bool repeatHorizontally  = true, bool repeatVertically = true, Pixel skipColor = Pixel(), bool useSkipColor = false) const;
	virtual void fillMaskPatern(const Canvas& canvas, const Pixel maskColor, bool repeatHorizontally = true, bool repeatVertically = true) const;
	virtual void stretchCanvas(const Canvas& canvas, int hto = 0, int vto = 0, int width = 0, int height = 0) const;

	virtual void writeText(const TUI_CHAR* string, const Font& font, const PixelColorProvider& provider, int hto = 0, int vto = 0, float multiply = 1.0f, bool vertically = false) const;

protected:
	void drawLetter(const Character& c, const PixelColorProvider& color, int hto, int vto, float multiply) const;
	constexpr int pixelPos(const int h, const int v) const { return v * m_Width + h; }
};
