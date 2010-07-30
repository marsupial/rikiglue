/*
	command.h
*/

#pragma once

#ifndef _command_h__
#define _command_h__


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
			typedef HDC           context_t;
		#else
			typedef struct CGContext *  context_t;
		#endif
	
		Context( context_t    context,
		         const Rect   &bounds );

		~Context();

		void
		setStrokeColor( float_t   r,
		                float_t   g,
		                float_t   b,
		                float_t   a = 1.0 ) const;

		void
		strokeRect( const Rect   &rect ) const;
	private:

		context_t    mContext;
	};


	Command() {}
	virtual ~Command() {}

	virtual bool
	doIt( const Command::Context   &context ) = 0;

private:

};

} /* namespace rikiGlue */

#endif /* _command_h__ */