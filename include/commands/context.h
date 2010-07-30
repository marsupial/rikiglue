/*
	context.h
*/

#pragma once

#ifndef _context_h__
#define _context_h__

#include <string>
#include <vector>

#if defined(_WINDOWS)
	#include <gdiplus.h>
#endif

#include "frame/block.h"

namespace rikiGlue
{

typedef float float_t;

class Context
{

#if defined(_WINDOWS)
	typedef HDC                 context_param_t;
	typedef HBITMAP             image_t;
	struct context_t
	{
		context_t( HDC       hdc,
				   float_t   h ) :
			mGraphics(hdc),
			mStroke(0,0,0),
			mFill(0,0,0),
			mHeight(h)
		{}
		
		void
		transform();

		Gdiplus::Graphics  mGraphics;
		Gdiplus::Color     mStroke;
		Gdiplus::Color     mFill;
		float_t            mHeight;
	};
#else
	typedef struct CGContext*   context_t;
	typedef context_t           context_param_t;
	typedef struct CGImage*     image_t;         
#endif

public:

	struct Point
	{
		Point( float_t  ix = 0,
			   float_t  iy = 0 ) :
			x(ix),
			y(iy)
		{}
		
		float_t  x, y;
	};

	struct Rect
	{
		Rect( float_t   x = 0,
			  float_t   y = 0,
			  float_t   w = 0,
			  float_t   h = 0 ) :
			origin(x,y),
			width(w),
			height(h)
		{}

	public:

		Point   origin;
		float_t width,
				height;
	};
	
	typedef std::vector<uint8_t>  bytes_t;

	Context( context_param_t  context,
	         image_t          image,
			 const Rect       &bounds );

	~Context();

	void
	setStrokeColor( float_t   r,
					float_t   g,
					float_t   b,
					float_t   a = 1.0 );

	void
	setFillColor( float_t   r,
				  float_t   g,
				  float_t   b,
				  float_t   a = 1.0 );

	void
	strokeRect( const Rect   &rect );

	void
	fillRect( const Rect   &rect );

	void
	drawText( const std::string  &str,
			  const Point        &loc,
			  const float_t      fontSize,
			  const std::string  &font );

	bool
	blockOperation( const Rect         &rect,
	                Block::operation_t op );

private:

	context_t    mContext;
	image_t      mImage;
};

} /* namespace rikiGlue */

#endif /* _context_h__ */