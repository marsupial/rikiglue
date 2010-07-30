/*
	command.h
*/

#pragma once

#ifndef _command_h__
#define _command_h__

#if defined(_WINDOWS)
	#include <gdiplus.h>
#endif

namespace rikiGlue
{

typedef float float_t;

class Rect
{
public:

	Rect( float_t   x = 0,
	      float_t   y = 0,
	      float_t   w = 0,
	      float_t   h = 0 );

public:

	float_t  originX, originY;
	float_t  width, height;
};

class Command
{
public:

	typedef Command* (*createOp)( const char    *args,
	                              size_t        len );
			
	class Context
	{
	public:
		#if defined(_WINDOWS)
			typedef HDC                 context_param_t;
			struct context_t
			{
				context_t( HDC       hdc,
				           float_t   h ) :
					mGraphics(hdc),
					mStroke(0,0,0),
					// mFill(0,0,0),
					mHeight(h)
				{}
				
				void
				transform();

				Gdiplus::Graphics  mGraphics;
				Gdiplus::Color     mStroke;
				// Gdiplus::Color     mFill;
				float_t            mHeight;
			};
		#else
			typedef struct CGContext *  context_t;
			typedef context_t           context_param_t;             
		#endif
	
		Context( context_param_t  context,
		         const Rect       &bounds );

		~Context();

		void
		setStrokeColor( float_t   r,
		                float_t   g,
		                float_t   b,
		                float_t   a = 1.0 );

		void
		strokeRect( const Rect   &rect );
/*
		void
		drawText( const std::string  &str,
		          const Rect         &location,
                  const float_t      fontSize,
		          const std::string  &font );
*/
	private:

		context_t    mContext;
	};


	Command() {}
	virtual ~Command() {}

	virtual bool
	doIt( Command::Context   &context ) = 0;

private:

};

} /* namespace rikiGlue */

#endif /* _command_h__ */