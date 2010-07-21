/*
	threads.h
*/

#pragma once

#ifndef _threads_h__
#define _threads_h__

#if defined(_WINDOWS)
	#include "glue.pch"
	#include <windows.h>
	#define USE_WINTHREADS   1

	struct thread_t { HANDLE    handle; DWORD  id; };
	typedef HANDLE          mutex_t;
	typedef HANDLE          condition_t;

#else
	#include <pthread.h>
	#define USE_PTHREADS   1

	typedef pthread_t       thread_t;
	typedef pthread_mutex_t mutex_t;
	typedef pthread_cond_t  condition_t;
#endif

namespace threads
{

class Thread
{
public:
	Thread();
	virtual ~Thread();
	
	register_t
	create();

	virtual void
	run() = 0;

protected:

	thread_t   mThread;
};

class Mutex
{
public:
	Mutex();
	~Mutex();

	void
	lock();
	
	void
	unlock();

	operator mutex_t* () 
	{
		return ( &mMutex );
	}

private:

	mutex_t  mMutex;
};

class Condition
{
public:
	Condition();
	~Condition();

	void
	signal();

	void
	wait( mutex_t    *mutex );

private:

	condition_t  mCondition;
};

} /* namespace threads */

#include "frame/frame.h"
#include <map>

namespace rikiGlue
{

struct ThreadBlock
{
	enum
	{
		kWaiting     = 0,
		kProcessing  = 1,
	};

	ThreadBlock( Frame                *frame  = NULL,
	             Frame::operation_t   *ops = NULL,
	             size_t               numOps = 0 ) :
		mProcess(kWaiting),
		mFrame(frame),
		mOps(ops),
		mNumOps(numOps)
	{}

		
	uint8_t            mProcess;
	Frame              *mFrame;
	Frame::operation_t *mOps;
	size_t             mNumOps;
	Frame::Block       mBlock;
};

typedef std::map<int, ThreadBlock>   BlockQueue;

class DecodeThread : public threads::Thread
{
public:

	DecodeThread() :
		mExit(false)
	{}

	virtual ~DecodeThread();

	// mutex must be locked before calling, and is unlocked on exit
	void
	decode();

	virtual void
	run();

	void
	exit()
	{
		mExit = true;
	}

	void
	lock()
	{
		mMutex.lock();
	}

	void
	unlock()
	{
		mMutex.unlock();
	}
	
	void
	signal()
	{
		mCondition.signal();
	}

	BlockQueue&
	queue()
	{
		return (mBlocks );
	}

private:

	threads::Mutex      mMutex;
	threads::Condition  mCondition;
	BlockQueue          mBlocks;
	bool                mExit;
};

} /* namespace rikiGlue */

#endif /* _threads_h__ */
