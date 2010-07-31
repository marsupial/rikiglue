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

#include <vector>

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

template <class T>
class Worker : public Thread
{
public:

	typedef std::vector<T> Queue;

	enum
	{
		kWaiting  = 0,
		kActive   = 1,
		kExit     = 2
	};

	Worker() :
		mState(kActive)
	{}

	virtual ~Worker()
	{
		mMutex.lock();
		if ( mState != kExit )
			mState = kExit;
		mMutex.unlock();
		
		mCondition.signal();
		
		mMutex.lock();
			// pthread_cancel(mThread);
		mMutex.unlock();
	}

	// mutex must be locked before calling, and is unlocked on exit
	void
	work()
	{
		typename Queue::iterator itr = mQueue.begin();
		if ( itr != mQueue.end() )
		{
			T param = *itr;
			mQueue.erase(itr);
			mMutex.unlock();
			
			work(param);
		}
		else
			mMutex.unlock();
	}

	void
	work( T    &param );

	virtual void
	run()
	{
		mMutex.lock();

		while ( true )
		{
			while ( mQueue.empty() )
			{
				mState = kWaiting;
				mCondition.wait(mMutex);
				if ( mState == kExit )
				{
					mMutex.unlock();
					return;
				}
				mState = kActive;
			}

			work();
			if ( mState == kExit )
				return;

			mMutex.lock();
		}
	}

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

	Queue&
	queue()
	{
		return ( mQueue );
	}

private:

	threads::Mutex      mMutex;
	threads::Condition  mCondition;
	Queue               mQueue;
	uint8_t             mState;
};

} /* namespace threads */

#endif /* _threads_h__ */
