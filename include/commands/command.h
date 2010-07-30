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

} /* namespace rikiGlue */

#endif /* _command_h__ */