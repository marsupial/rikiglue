/*
   command.cpp
*/


#include "common/application.h"
#include "commands/command.h"
#include "commands/context.h"
#include "frame/operations.h"

namespace rikiGlue
{

template < register_t F(const Block    &block) >
class SubRect : public Command
{
public:

	static Command*
	create( const char       *args,
	        size_t           len )
	{
		return ( new SubRect(args, len) );
	}

	virtual ~SubRect()
	{
	}

	virtual bool
	doIt( Context   &context )
	{
		Context::Rect rect(0, 1080-200,200,200);
		context.blockOperation(rect, F);
		return ( false );
	}

private:

	SubRect( const char     *args,
	         size_t         len )
	{
	}

	Context::Rect   mRect;
};

class DrawText : public Command
{
public:

	static Command*
	create( const char       *args,
	        size_t           len )
	{
		return ( new DrawText(args, len) );
	}

	virtual ~DrawText()
	{
	}

	virtual bool
	doIt( Context   &context )
	{
		context.drawText(mString, mPoint, mFontSize, mFont);
		return ( false );
	}

private:

	DrawText( const char     *args,
	          size_t         len )
	{
	}

	std::string     mString;

	Context::Point  mPoint;
	float_t         mFontSize;
	std::string     mFont;
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

	virtual ~DrawLock()
	{
	}

	virtual bool
	doIt( Context   &context )
	{
		if ( Application::instance().locked() )
			context.setStrokeColor(0.0, 1.0, 0);
		else
			context.setStrokeColor(1.0, 0.0, 0);

		Context::Rect drect(0,0,1920,1080);
		context.strokeRect(drect);
		return ( true );
	}

private:

	DrawLock( const char      *args,
	          size_t          len )
	{
	}
};

void
Application::loadCommands()
{
	mCommands[ "drawLock" ] = DrawLock::create;
	mCommands[ "drawText" ] = DrawText::create;
	mCommands[ "rsaEncrypt" ] = SubRect<rsaEncrypt>::create;
	mCommands[ "gChannel" ] = SubRect<gChannel>::create;
}

} /* namespace rikiGlue */
