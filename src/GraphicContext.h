/*
    Copyright (C) 2001, 2002 Stephane Magnenat & Luc-Olivier de Charriere
    for any question or comment contact us at nct@ysagoon.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef __GRAPHICCONTEXT_H
#define __GRAPHICCONTEXT_H

#include "Header.h"


class Font
{
public:
	virtual ~Font() { }
	virtual int getStringWidth(const char *string) const=0;
	virtual int getStringHeight(const char *string) const=0;
	virtual bool printable(char c)=0;
};


class Sprite
{
public:
	virtual ~Sprite() { }
	virtual void enableBaseColor(Uint8 r, Uint8 g, Uint8 b)=0;
	virtual void disableBaseColor(void)=0;
	virtual int getW(int index)=0;
	virtual int getH(int index)=0;
};



class DrawableSurface
{
public:
	enum GraphicContextType
	{
		GC_SDL=0,
		GC_GL=1
	};

	enum Alpha
	{
		ALPHA_OPAQUE=255
	};

	enum ResolutionFlags
	{
		DEFAULT=0,
		FULLSCREEN=1,
		HWACCELERATED=2
	};

public:
	virtual ~DrawableSurface(void) { }
	virtual bool setRes(int w, int h, int depth=32, Uint32 flags=DEFAULT)=0;
	virtual void setAlpha(bool usePerPixelAlpha=false, Uint8 alphaValue=ALPHA_OPAQUE)=0;
	virtual int getW(void)=0;
	virtual int getH(void)=0;
	virtual void setClipRect(int x, int y, int w, int h)=0;
	virtual void setClipRect(void)=0;
	virtual void drawSprite(int x, int y, Sprite *sprite, int index=0)=0;
	virtual void drawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a=ALPHA_OPAQUE)=0;
	virtual void drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a=ALPHA_OPAQUE)=0;
	virtual void drawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a=ALPHA_OPAQUE)=0;
	virtual void drawVertLine(int x, int y, int l, Uint8 r, Uint8 g, Uint8 b, Uint8 a=ALPHA_OPAQUE)=0;
	virtual void drawHorzLine(int x, int y, int l, Uint8 r, Uint8 g, Uint8 b, Uint8 a=ALPHA_OPAQUE)=0;
	virtual void drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a=ALPHA_OPAQUE)=0;
	virtual void drawCircle(int x, int y, int ray, Uint8 r, Uint8 g, Uint8 b, Uint8 a=ALPHA_OPAQUE)=0;
	virtual void drawString(int x, int y, const Font *font, const char *msg, ...)=0;
	virtual void drawSurface(int x, int y, DrawableSurface *surface)=0;
	virtual void updateRects(SDL_Rect *rects, int size)=0;
	virtual void updateRect(int x, int y, int w, int h)=0;
};


class GraphicContext:public virtual DrawableSurface
{
public:
	virtual ~GraphicContext(void) { }

	//! this must be called before any Drawable Surface method.
	virtual bool setRes(int w, int h, int depth=32, Uint32 flags=DEFAULT)=0;
	virtual void setCaption(const char *title, const char *icon)=0;

	virtual void dbgprintf(const char *msg, ...)=0;

	virtual Sprite *loadSprite(const char *name)=0;
	virtual Font *loadFont(const char *name)=0;
	virtual DrawableSurface *createDrawableSurface(void)=0;

	virtual void nextFrame(void)=0;

	static GraphicContext *createGraphicContext(GraphicContextType type);
};






#endif
