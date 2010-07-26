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
#else
	#include <pthread.h>
	#define USE_PTHREADS   1
#endif

namespace threads
{

#if defined(USE_PTHREADS)
	typedef pthread_t       thread_t;
	typedef pthread_mutex_t mutex_t;
	typedef pthread_cond_t  condition_t;
#else
	struct thread_t { HANDLE    handle; DWORD  id; };
	typedef HANDLE          mutex_t;
	typedef HANDLE          condition_t;
#endif

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
	broadcast();

	void
	wait( mutex_t    *mutex );

private:

	condition_t  mCondition;
};

} /* namespace threads */

#include "frame/frame.h"
#include <vector>

namespace rikiGlue
{

struct ThreadBlock : public Frame::Block
{
	ThreadBlock( const uint8_t   *src,
	             size_t          srcSz,
	             uint8_t         *dst,
	             size_t          dstSz,
	             const Frame::operation_t *opers,
	             size_t                   nOpers ) :
		Block(src, srcSz, dst, dstSz),
		ops(opers),
		numOps(nOpers)
	{}
	
	const Frame::operation_t *ops;
	size_t                   numOps;
};

typedef std::vector<ThreadBlock>   BlockQueue;

class DecodeThread : public threads::Thread
{
public:

	enum
	{
		kWaiting  = 0,
		kActive   = 1,
		kExit     = 2
	};

	DecodeThread() :
		mState(kActive)
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
		mState = kExit;
	}

	uint8_t
	state() const
	{
		return ( mState );
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

	void
	broadcast()
	{
		mCondition.broadcast();
	}

	BlockQueue&
	queue()
	{
		return ( mBlocks );
	}

private:

	threads::Mutex      mMutex;
	threads::Condition  mCondition;
	BlockQueue          mBlocks;
	volatile uint8_t             mState;
};

} /* namespace rikiGlue */

#endif /* _threads_h__ */
