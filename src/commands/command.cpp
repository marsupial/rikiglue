/*
   command.cpp
*/

#include <ApplicationServices/ApplicationServices.h>
#include "common/application.h"
#include "commands/command.h"

namespace rikiGlue
{

Rect::Rect( float_t   x,
            float_t   y,
            float_t   w,
            float_t   h ) :
	originX(x),
	originY(y),
	width(w),
	height(h)
{
}

Command::Context::Context( Command::Context::context_t   ctx,
                           const Rect                    &bounds ) :
	mContext(ctx)
{
	CGContextSaveGState(mContext);

	const rikiGlue::Rect &rect = Application::instance().transform();
	CGContextTranslateCTM(mContext, rect.originX, rect.originY);
	CGContextScaleCTM(mContext, rect.width, rect.height);
}

Command::Context::~Context()
{
	CGContextRestoreGState(mContext);
}

void
Command::Context::setStrokeColor( float_t   r,
		                          float_t   g,
		                          float_t   b,
		                          float_t   a  ) const
{
	CGContextSetRGBStrokeColor(mContext, r, g, b, a);
}

void
Command::Context::strokeRect( const Rect    &rect ) const
{
	const CGRect cgRect = { { rect.originX, rect.originY}, { rect.width, rect.height } };
	CGContextStrokeRect(mContext, cgRect);
}

class Lower : public Command
{
public:

	static Command*
	create( const char       *args,
	        size_t           len )
	{
		return ( new Lower(args, len) );
	}

	Lower( const char      *args,
	       size_t           len ) :
		mArguments(args, len)
	{
	}

	~Lower()
	{
	}

	virtual bool
	doIt( const Command::Context   &context )
	{
		printf("<Lower> [%s]\n", mArguments.c_str());
		return ( false );
	}

private:
	std::string    mArguments;
};

class DrawLock : public Command
{
public:

	static Command*
	create( const char       *args,
	        size_t           len )
	{
		return ( new DrawLock(args, len) );
	}

	DrawLock( const char      *args,
	          size_t          len )
	{
	}

	virtual ~DrawLock()
	{
	}

	virtual bool
	doIt( const Command::Context   &context )
	{
		if ( Application::instance().locked() )
			context.setStrokeColor(0.0, 1.0, 0);
		else
			context.setStrokeColor(1.0, 0.0, 0);

		Rect drect(0,0,1920,1080);
		context.strokeRect(drect);
		return ( true );
	}

private:
};

void
Application::start()
{
	startThreads();
	mCommands[ "Lower" ] = Lower::create;
	mCommands[ "drawLock" ] = DrawLock::create;
}

} /* namespace rikiGlue */
