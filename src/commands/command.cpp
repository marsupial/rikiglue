/*
   command.cpp
*/

#include <sstream>

#include "common/application.h"
#include "commands/command.h"
#include "commands/context.h"
#include "frame/operations.h"

namespace rikiGlue
{

static bool
parsePoint( const char       *&args,
            size_t           &len,
            Context::Point   &pt )
{
	if ( len <= 5 || args[0] != 'p' || args[1] != ' ')
		return ( false );
		
	std::string str(args+1, len-1);
	std::istringstream strm(str);
	if ( ! (strm >> pt.x) )
		return ( false );
	if ( ! (strm >> pt.y) )
		return ( false );

	const size_t p = (std::min)(static_cast<size_t>(strm.tellg())+2, len);
	len -= p;
	args += p;
	return ( true );
}

static bool
parseRect( const char        *&args,
           size_t            &len,
           Context::Rect     &rect )
{
	if ( len <= 9 || args[0] != 'r' || args[1] != ' ')
		return ( false );
		
	std::string str(args+1, len-1);
	std::istringstream strm(str);
	if ( ! (strm >> rect.origin.x) )
		return ( false );
	if ( ! (strm >> rect.origin.y) )
		return ( false );
	if ( ! (strm >> rect.width) )
		return ( false );
	if ( rect.width == 0 )
		return ( false );
	if ( ! (strm >> rect.height) )
		return ( false );
	if ( rect.height == 0 )
		return ( false );
	
	const size_t p = (std::min)(static_cast<size_t>(strm.tellg())+2, len);
	len -= p;
	args += p;
	return ( true );
}

static bool
parseFrame( const char        *&args,
            size_t            &len,
            register_t        &n )
{
	if ( len <= 3 || args[0] != 't' || args[1] != ' ')
		return ( false );

	std::string str(args+1, len-1);
	std::istringstream strm(str);

	if ( ! (strm >> n) )
		return ( false );
	if ( n == 0 )
		return ( false );

	const size_t p = (std::min)(static_cast<size_t>(strm.tellg())+2, len);
	len -= p;
	args += p;
	return ( true );
}

template < register_t F(const Block    &block) >
class SubRect : public TimeCommand
{
public:

	static Command*
	create( const char       *args,
	        size_t           len )
	{
		if ( len == 0 )
			return ( NULL );
		
		Context::Rect rect;
		if ( ! parseRect(args, len, rect) )
			return (NULL);

		register_t n = 1;
		if ( len )
			parseFrame(args, len, n);

		return ( new SubRect(n, rect) );
	}

	virtual ~SubRect()
	{
	}

	virtual bool
	doIt( Context   &context,
	      float_t   t )
	{
		context.blockOperation(mRect, F);
		return ( true );
	}

private:

	SubRect( register_t          n,
	         const Context::Rect &rect ) :
		TimeCommand(n),
		mRect(rect)
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

// r 0 0 0 0
// p 0 0
// rsaEncrypt r 0 0 200 200 t 10 sin d 

} /* namespace rikiGlue */
