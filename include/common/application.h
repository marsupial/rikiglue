/*
	application.h
*/

#pragma once

#ifndef _application_h__
#define _application_h__

#include "frame/threads.h"
#include "commands/command.h"

#include <list>
#include <string>
#include <map>

namespace rikiGlue
{

class Instructions
{
public:

	typedef std::list<Command*>  list_t;

	Instructions() {}
	~Instructions() { clear(); }

	void
	lock()
	{
		return ( mMutex.lock() );
	}
	
	void
	push_back( list_t::value_type   cmd )
	{
		return ( mInstructions.push_back(cmd) );
	}

	void
	unlock()
	{
		return ( mMutex.unlock() );
	}

	void
	process( Command::Context   &ctx );

	void
	clear();

private:

	list_t         mInstructions;
	threads::Mutex mMutex;
};

class Application
{
public:

	static Application&
	instance();

	Application();
	~Application();

	void
	start();

	void
	stop();

	void
	dmtxFrame( Frame    *inFrame );

	bool
	addInstruction( const uint8_t    *bytes,
	                size_t           len,
	                const Rect       &rect );

	void
	process( Command::Context   &ctx );

	void
	notLocked()
	{
		mLocked = false;
	}

	DecodeThread*
	decoderThread()
	{
		return ( mPixelDecodeThread );
	}

	const Rect&
	transform() const
	{
		return ( mRect );
	}

	bool
	locked() const
	{
		return ( mLocked );
	}

private:

	typedef std::map<std::string, Command::createOp>  Commands_t;

	void
	startThreads();

	void
	stopThreads();

	DecodeThread    *mPixelDecodeThread;

	DMTXThread      *mDMTXThread;
	DMTXInstrThread *mDMTXInstrThread;
	
	Commands_t     mCommands;
	Instructions   mInstructions;

	Rect           mRect;
	bool           mLocked;
};

} /* namespace rikiGlue */

#endif /* _application_h__ */