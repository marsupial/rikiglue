/*
	command.h
*/

#pragma once

#ifndef _command_h__
#define _command_h__

namespace rikiGlue
{

class Context;

class Command
{
public:

	typedef Command* (*createOp)( const char    *args,
	                              size_t        len );

	Command() {}
	virtual ~Command() {}

	virtual bool
	doIt( Context   &context ) = 0;

private:

};

class TimeCommand : public Command
{
public:

	TimeCommand( register_t   n = 1 ) :
		mNumFrames(n),
		mFrame(0)
	{}
	virtual ~TimeCommand() {}

	virtual bool
	doIt( Context   &context )
	{
		if ( doIt(context, static_cast<float_t>(++mFrame)/static_cast<float_t>(mNumFrames)) == false )
			return ( false );
		return ( mFrame < mNumFrames );
	}

	virtual bool
	doIt( Context   &context,
	      float_t   t ) = 0;
	
private:

	register_t  mFrame,
	            mNumFrames;
};

} /* namespace rikiGlue */

#endif /* _command_h__ */