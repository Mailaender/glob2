/*
  Copyright (C) 2001-2004 Stephane Magnenat & Luc-Olivier de Charrière
  for any question or comment contact us at nct@ysagoon.com or nuage@ysagoon.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef INCLUDED_SDL_GRAPHICCONTEXT_H
#define INCLUDED_SDL_GRAPHICCONTEXT_H

#include "GAGSys.h"
#include "CursorManager.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace GAGCore
{
	//! Color is 4 bytes big but provides easy access to components
	struct Color
	{
		//! Typical usefull alpha values pre-defined
		enum Alpha
		{
			ALPHA_TRANSPARENT = 0, //!< constant for transparent alpha
			ALPHA_OPAQUE = 255 //!< constant for opaque alpha
		};
		
		Uint8 r, g, b, a; //!< component of the color
		
		//! Constructor. Default color is opaque black
		Color() { r = g = b = 0; a = ALPHA_OPAQUE; }
		//! Constructor from components
		Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = ALPHA_OPAQUE) { this->r = r; this->g = g; this->b = b; this->a = a; }
		
		//! Return HSV values in pointers
		void getHSV(float *hue, float *sat, float *lum);
		//! Set color from HLS, alpha unctouched
		void setHSV(float hue, float sat, float lum);
		
		//! pack components in a 32 bits int given SDL screen values
		Uint32 pack() const;
		//! unpack from a 32 bits int given SDL screen values
		void unpack(const Uint32 packedValue);
		
		//! comparaison for inequality
		bool operator<(const Color &o) const { return pack() < o.pack(); }
		//! comparaison for equality
		bool operator==(const Color &o) const { return pack() == o.pack(); }
		
		static Color black; //!< black color (0,0,0)
		static Color white; //!< black color (255,255,255)
	};
	
	//! Deprecated, for compatibility only. Eventually, all Color32 should be removed or changed to Color
	typedef Color Color32;
	
	class Sprite;
	
	//! Font with a given foundery, shape and color
	class Font
	{
	public:
		//! Shape of the font
		enum Shape
		{
			STYLE_NORMAL = 0x00, //!< normal fond
			STYLE_BOLD = 0x01, //!< bold font
			STYLE_ITALIC = 0x02, //!< italic font
			STYLE_UNDERLINE = 0x04, //!< underlined font
		};
		
		//! Style of the font, i.e. a shape and a color
		struct Style
		{
			Shape shape; //!< shape of this style
			Color color; //!< color of this style
			
			//! Constructor. Default is normal with white opaque color
			Style() { shape = STYLE_NORMAL; color = Color::white; }
			
			//! Constructor from shape and color
			Style(Shape _shape, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE) :
				shape(_shape),
				color(r, g, b, a)
			{
			}
			
			//! inequality comparaison
			bool operator<(const Style &o) const
			{
				if (color == o.color)
					return shape < o.shape;
				else
					return color < o.color;
			}
		};
	
	public:
		//! Destructor
		virtual ~Font() { }
	
		// width and height
		virtual int getStringWidth(const char *string) = 0;
		virtual int getStringWidth(const char *string, int len);
		virtual int getStringWidth(const int i);
		virtual int getStringHeight(const char *string) = 0;
		virtual int getStringHeight(const char *string, int len);
		virtual int getStringHeight(const int i);
	
		// Style and color
		virtual void setStyle(Style style) = 0;
		virtual Style getStyle(void) const = 0;
		virtual void pushStyle(Style style) = 0;
		virtual void popStyle(void) = 0;
		
	protected:
		friend class DrawableSurface;
		virtual void drawString(DrawableSurface *Surface, int x, int y, int w, const char *text, Uint8 alpha) = 0;
		virtual void drawString(DrawableSurface *Surface, float x, float y, float w, const char *text, Uint8 alpha) = 0;
	};
	
	//! A surface on which we can draw
	class DrawableSurface
	{
	protected:
		friend struct Color;
		friend class GraphicContext;
		//! the underlying software SDL surface
		SDL_Surface *sdlsurface;
		//! The clipping rect, we do not draw outside it
		SDL_Rect clipRect;
		//! this surface has been modified since latest blit
		bool dirty;
		//! texture index if GPU (GL) is used
		unsigned int texture;
		//! texture divisor
		float texMultX, texMultY;
		
	protected:
		//! draw a vertical line. This function is private because it is only a helper one
		void _drawVertLine(int x, int y, int l, Color color);
		//! draw a horizontal line. This function is private because it is only a helper one
		void _drawHorzLine(int x, int y, int l, Color color);
		
	protected:
		//! Protectedconstructor, only called by GraphicContext
		DrawableSurface() { sdlsurface = NULL; }
		//! allocate textre in GPU for this surface
		void allocateTexture(void);
		//! reset the texture size upon changes
		void initTextureSize(void);
		//! upload surface to GPU
		void uploadToTexture(void);
		//! free texture in GPU for this surface
		void freeGPUTexture(void);
		
	public:
		// New API
		
		// constructors and destructor
		DrawableSurface(const char *imageFileName);
		DrawableSurface(const std::string &imageFileName);
		DrawableSurface(int w, int h);
		DrawableSurface(const SDL_Surface *sourceSurface);
		DrawableSurface *clone(void);
		virtual ~DrawableSurface(void);
		
		// modifiers
		virtual void setRes(int w, int h);
		virtual void getClipRect(int *x, int *y, int *w, int *h);
		virtual void setClipRect(int x, int y, int w, int h);
		virtual void setClipRect(void);
		virtual void nextFrame(void) { }
		virtual bool loadImage(const char *name);
		virtual bool loadImage(const std::string &name);
		virtual void shiftHSV(float hue, float sat, float lum);
		
		// accessors
		virtual int getW(void) { return sdlsurface->w; } 
		virtual int getH(void) { return sdlsurface->h; }
		
		// drawing commands
		virtual void drawPixel(int x, int y, Color color);
		virtual void drawPixel(float x, float y, Color color);
		
		virtual void drawRect(int x, int y, int w, int h, Color color);
		virtual void drawRect(float x, float y, float w, float h, Color color);
		
		virtual void drawFilledRect(int x, int y, int w, int h, Color color);
		virtual void drawFilledRect(float x, float y, float w, float h, Color color);
		
		virtual void drawLine(int x1, int y1, int x2, int y2, Color color);
		virtual void drawLine(float x1, float y1, float x2, float y2, Color color);
		
		virtual void drawCircle(int x, int y, int radius, Color color);
		virtual void drawCircle(float x, float y, float radius, Color color);
		
		virtual void drawSurface(int x, int y, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		virtual void drawSurface(int x, int y, int w, int h, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, float w, float h, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		virtual void drawSurface(int x, int y, DrawableSurface *surface, int sx, int sy, int sw, int sh, Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, DrawableSurface *surface, int sx, int sy, int sw, int sh, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		virtual void drawSurface(int x, int y, int w, int h, DrawableSurface *surface, int sx, int sy, int sw, int sh,  Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, float w, float h, DrawableSurface *surface, int sx, int sy, int sw, int sh, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		void drawSprite(int x, int y, Sprite *sprite, unsigned index = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		void drawSprite(float x, float y, Sprite *sprite, unsigned index = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		void drawSprite(int x, int y, int w, int h, Sprite *sprite, unsigned index = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		void drawSprite(float x, float y, float w, float h, Sprite *sprite, unsigned index = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		void drawString(int x, int y, Font *font, const char *msg, int w = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		void drawString(float x, float y, Font *font, const char *msg, float w = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		void drawString(int x, int y, Font *font, const std::string &msg, int w = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		void drawString(float x, float y, Font *font, const std::string &msg, float w = 0, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		// old API, deprecated, do not use. It is only there for compatibility with existing code
		virtual void drawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawVertLine(int x, int y, int l, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawHorzLine(int x, int y, int l, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawString(int x, int y, Font *font, int i);
	};
	
	//! A GraphicContext is a DrawableSurface that represent the main screen of the application.
	class GraphicContext:public DrawableSurface
	{
	public:
		//! The cursor manager, public to be able to set custom cursors
		CursorManager cursorManager;
		
		//! Flags that define the characteristic of the graphic context
		enum OptionFlags
		{
			DEFAULT = 0,
			USEGPU = 1,
			FULLSCREEN = 2,
			RESIZABLE = 8,
			CUSTOMCURSOR = 16,
		};
		
	protected:
		//! the minimum acceptable resolution
		int minW, minH;
		//! the pointer for iterating through mode list
		SDL_Rect **modes;
		friend class DrawableSurface;
		//! option flags
		Uint32 optionFlags;
		
	public:
		//! Constructor. Create a new window of size (w,h). If useGPU is true, use GPU for accelerated 2D (OpenGL or DX)
		GraphicContext(int w, int h, Uint32 flags);
		//! Destructor
		virtual ~GraphicContext(void);
		
		// modifiers
		virtual bool setRes(int w, int h, Uint32 flags);
		virtual void setRes(int w, int h) { setRes(w, h, optionFlags); }
		virtual void setClipRect(int x, int y, int w, int h);
		virtual void setClipRect(void);
		virtual void nextFrame(void);
		//! This function does not work for GraphicContext
		virtual bool loadImage(const char *name) { return false; }
		//! This function does not work for GraphicContext
		virtual bool loadImage(const std::string &name) { return false; }
		//! This function does not work for GraphicContext
		virtual void shiftHSV(float hue, float sat, float lum) { }
		
		// reimplemented drawing commands for HW (GPU / GL) accelerated version
		virtual void drawPixel(int x, int y, Color color);
		virtual void drawPixel(float x, float y, Color color);
		
		virtual void drawRect(int x, int y, int w, int h, Color color);
		virtual void drawRect(float x, float y, float w, float h, Color color);
		
		virtual void drawFilledRect(int x, int y, int w, int h, Color color);
		virtual void drawFilledRect(float x, float y, float w, float h, Color color);
		
		virtual void drawLine(int x1, int y1, int x2, int y2, Color color);
		virtual void drawLine(float x1, float y1, float x2, float y2, Color color);
		
		virtual void drawCircle(int x, int y, int radius, Color color);
		virtual void drawCircle(float x, float y, float radius, Color color);
		
		virtual void drawSurface(int x, int y, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		virtual void drawSurface(int x, int y, int w, int h, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, float w, float h, DrawableSurface *surface, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		virtual void drawSurface(int x, int y, DrawableSurface *surface, int sx, int sy, int sw, int sh, Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, DrawableSurface *surface, int sx, int sy, int sw, int sh, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		virtual void drawSurface(int x, int y, int w, int h, DrawableSurface *surface, int sx, int sy, int sw, int sh,  Uint8 alpha = Color::ALPHA_OPAQUE);
		virtual void drawSurface(float x, float y, float w, float h, DrawableSurface *surface, int sx, int sy, int sw, int sh, Uint8 alpha = Color::ALPHA_OPAQUE);
		
		// compat
		virtual void drawPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawFilledRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawVertLine(int x, int y, int l, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawHorzLine(int x, int y, int l, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		virtual void drawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = Color::ALPHA_OPAQUE);
		
		// GraphicContext specific methods
		//! Set the minimum acceptable resolution
		virtual void setMinRes(int w = 0, int h = 0);
		//! Set the caption of the window
		virtual void setCaption(const char *title, const char *icon) { SDL_WM_SetCaption(title, icon); }
		//! Begin listing of acceptable video mode, *not thread-safe*
		virtual void beginVideoModeListing(void);
		//! Get the next acceptable video mode in w,h, return false if end of list, *not thread-safe*
		virtual bool getNextVideoMode(int *w, int *h);
		
		//! Save a bmp of the screen to a file, bypass virtual filesystem
		virtual void printScreen(const char *filename);
	};
	
	//! A sprite is a collection of images (frames) that can be displayed one after another to make an animation
	class Sprite
	{
	protected:
		struct RotatedImage
		{
			DrawableSurface *orig;
			typedef std::map<Color32, DrawableSurface *> RotationMap;
			RotationMap rotationMap;
	
			RotatedImage(DrawableSurface *s) { orig = s; }
			~RotatedImage();
		};
	
		std::vector <DrawableSurface *> images;
		std::vector <RotatedImage *> rotated;
		Color actColor;
	
		friend class DrawableSurface;
		// Support functions
		//! Load a frame from two file pointers
		void loadFrame(SDL_RWops *frameStream, SDL_RWops *rotatedStream);
		//! Check if index is within bound and return true, assert false and return false otherwise
		bool checkBound(int index);
		//! Return a rotated drawable surface for actColor, create it if necessary
		virtual DrawableSurface *getRotatedSurface(int index);
	
	public:
		//! Constructor
		Sprite() { }
		//! Destructor
		virtual ~Sprite();
		
		//! Load a sprite from the file, return true if any frame have been loaded
		bool load(const char *filename);
	
		//! Set the (r,g,b) color to a sprite's base color
		virtual void setBaseColor(Uint8 r, Uint8 g, Uint8 b) { actColor = Color(r, g, b); }
		
		//! Return the width of index frame of the sprite
		virtual int getW(int index);
		//! Return the height of index frame of the sprite
		virtual int getH(int index);
		//! Return the number of frame in this sprite
		virtual int getFrameCount(void);
	};
}

#endif
