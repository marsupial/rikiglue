/*
   command.cpp
*/

#if defined(_WINDOWS)

#else
	#include <ApplicationServices/ApplicationServices.h>
#endif

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

#if defined(_WINDOWS)

using namespace Gdiplus;
#define mGDIContext (mContext.mGraphics)

void
Command::Context::context_t::transform()
{
	mGraphics.TranslateTransform(0.f, mHeight);
	mGraphics.ScaleTransform(1.f, -1.f);

	const rikiGlue::Rect &rect = Application::instance().transform();
	mGraphics.TranslateTransform(rect.originX, rect.originY);
	mGraphics.ScaleTransform(rect.width, rect.height);
}

Command::Context::Context( Command::Context::context_param_t ctx,
                           const Rect                        &bounds ) :
	mContext(ctx, bounds.height)
{
	mContext.transform();
}

Command::Context::~Context()
{

}

void
Command::Context::setStrokeColor( float_t   r,
		                          float_t   g,
		                          float_t   b,
		                          float_t   a  )
{
	mContext.mStroke = Color( static_cast<BYTE>(a*255.f), static_cast<BYTE>(r*255.f),
	                          static_cast<BYTE>(g*255.f), static_cast<BYTE>(b*255.f) );
}
/*
void
Command::Context::drawText( const std::string &str,
						    const Rect        &location,
                            const float_t     fontSize,
						    const std::string &fontName )
{
	GraphicsState state = mGDIContext.Save();

	Matrix mx1;
	Matrix mx2(1.f, 0.f, 0.f, -1.f, 0.f, fontSize+fontSize/2.f);

	mGDIContext.GetTransform(&mx1);
	mx1.Multiply(&mx2);
	mGDIContext.SetTransform(&mx1);
	
	SolidBrush brush( mContext.mStroke );
	Font       font(wname.data(), fontSize);
	PointF     pt(location.originX, -location.originY);

	mGDIContext.DrawString(wstr.data(), wstr.length(), &font, pt, &brush);
	mGDIContext.Restore(state);
}
*/

void
Command::Context::strokeRect( const Rect    &rect )
{
	Pen pen( mContext.mStroke );
	RectF gdiRect(rect.originX, rect.originY, rect.width, rect.height);
	mGDIContext.DrawRectangles(&pen, &gdiRect, 1);
}

#else

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
		                          float_t   a  )
{
	CGContextSetRGBStrokeColor(mContext, r, g, b, a);
}

void
Command::Context::strokeRect( const Rect    &rect )
{
	const CGRect cgRect = { { rect.originX, rect.originY}, { rect.width, rect.height } };
	CGContextStrokeRect(mContext, cgRect);
}

#endif

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
	doIt( Command::Context   &context )
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
	doIt( Command::Context   &context )
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
