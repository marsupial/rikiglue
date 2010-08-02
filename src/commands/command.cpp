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
	if ( len <= 5 || args[0] != 'p' || !isspace(args[1]) )
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
	if ( len <= 9 || args[0] != 'r' || !isspace(args[1]) )
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
	if ( len <= 3 || args[0] != 't' || !isspace(args[1]) )
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

template < register_t E(const Block    &block), register_t D(const Block    &block) >
class SubRectChoice : public TimeCommand
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
		bool op = 0;
		if ( len )
		{
			parseFrame(args, len, n);

			if ( len >= 2 && args[0] == '1' && ( (len==2) || isspace(args[1]) ) )
				op = 1;
		}

		return ( new SubRectChoice(n, rect, op) );
	}

	virtual ~SubRectChoice()
	{
	}

	virtual bool
	doIt( Context   &context,
	      float_t   t )
	{
		if ( mOp )
			context.blockOperation(mRect, D);
		else
			context.blockOperation(mRect, E);
		return ( true );
	}

private:

	SubRectChoice( register_t          n,
	               const Context::Rect &rect,
	               bool                op ) :
		TimeCommand(n),
		mRect(rect),
		mOp(op)
	{
	}

	Context::Rect   mRect;
	const bool      mOp;
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

static bool sDrawLock = false;

class DrawLock : public Command
{
public:

	static Command*
	create( const char       *args,
	        size_t           len )
	{
		if ( sDrawLock )
			return ( NULL );
		sDrawLock = true;
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

extern "C" int
ssh_main( int, const char**);


class SSH : public Command
{
public:

	static Command*
	create( const char       *args,
	        size_t           len )
	{
		if ( len < 4 )
			return ( NULL );

		bool encrypted = false;
		if ( args[0] == 'e' && isspace(args[1]) )
		{
			encrypted = true;
			args+=2;
			len-=2;
		}
		return ( new SSH(encrypted, args, len) );
	}

	virtual ~SSH()
	{
	}

	virtual bool
	doIt( Context   &context )
	{
		Context::bytes_t decrypt;
		
		if ( mEncrypted )
		{
			if ( !Application::instance().decrypt(reinterpret_cast<const uint8_t*>(mString.data()), mString.length(), decrypt) )
				return ( false );
		}
		else
			decrypt.assign(reinterpret_cast<const uint8_t*>(mString.data()), reinterpret_cast<const uint8_t*>(mString.data())+mString.length());

		if ( decrypt[ decrypt.size()-1 ] != 0 )
			decrypt.push_back(0);

		const char key[] = " ";
		const char *str = reinterpret_cast<const char*>(&decrypt[0]);

		std::vector<const char*> argv(1, "ssh");
		argv.push_back(str);

		const char *pch = strpbrk(str, key);
		while ( pch != NULL && argv.size() < 5 )
		{
			*const_cast<char*>(pch) = 0;
			argv.push_back(++pch);
			pch = strpbrk(pch, key);
		}

		if ( argv.size() > 4 )
			ssh_main(argv.size(), &argv[0]);

		if ( decrypt.size() )
		{
			memset(&decrypt[0], 'X', decrypt.size());
			decrypt.clear();
		}
		
		return ( false );
	}

private:

	SSH( bool        encrypted,
	     const char  *str,
	     size_t      len ) :
		mString(str, len),
		mEncrypted(encrypted)
	{
	}

	const std::string    mString;
	const bool           mEncrypted;
};
// echo "user domain pass touch ~/dmtx.txt" | ./ssl ../data/public.key | ./dmtx -t "ssh e " -r ../sourceimages/dmtx/ssh.png

void
Application::loadCommands()
{
	mCommands[ "drawLock" ] = DrawLock::create;
	mCommands[ "drawText" ] = DrawText::create;
	mCommands[ "rsa" ] = SubRectChoice<rsaEncrypt, rsaDecrypt>::create;
	mCommands[ "gChannel" ] = SubRect<gChannel>::create;
	mCommands[ "ssh" ] = SSH::create;
}

} /* namespace rikiGlue */
