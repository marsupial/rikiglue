/*
	threads.cpp
*/

#include "threads/threads.h"
#include <stdexcept>

namespace threads
{

static void*
threadRunner( void    *runner )
{
	try
	{
		Thread *thread = static_cast<Thread*>(runner);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		thread->run();
	}
	catch ( std::exception &exc )
	{
		printf("exception: %s\n", exc.what());
	}
	catch ( ... )
	{
		printf("unknown exception\n");
	}

	pthread_exit(0);
	return ( NULL );
}

Thread::Thread() :
	mThread( pthread_self() )
{
}

Thread::~Thread()
{
	if ( ! pthread_equal(mThread, pthread_self()) )
	{
		pthread_cancel(mThread);
		pthread_join(mThread, NULL);
	}
}

register_t
Thread::create()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_t thread;

	const int rval = pthread_create(&thread, &attr, threadRunner, static_cast<Thread*>(this));
	pthread_attr_destroy(&attr);

	if ( rval != 0 )
		return ( rval );

	mThread = thread;
	return ( 0 );
}

Mutex::Mutex()
{
	if ( pthread_mutex_init(&mMutex, NULL) != 0 )
		throw ( std::runtime_error("pthread_mutex_init") );
}

Mutex::~Mutex() 
{
	pthread_mutex_destroy(&mMutex);
}

void
Mutex::lock()
{
	if ( int rval = pthread_mutex_lock(&mMutex) )
		throw ( std::runtime_error("pthread_mutex_lock") );
}

void
Mutex::unlock()
{
	if ( int rval = pthread_mutex_unlock(&mMutex) )
		throw ( std::runtime_error("pthread_mutex_unlock") );
}

Condition::Condition()
{
	if ( pthread_cond_init(&mCondition, NULL) != 0 )
		throw ( std::runtime_error("pthread_cond_init") );
}

Condition::~Condition() 
{
	pthread_cond_destroy(&mCondition);
}

void
Condition::signal()
{
	if ( pthread_cond_signal(&mCondition) != 0 )
		throw ( std::runtime_error("pthread_cond_signal") );
}

void
Condition::wait( mutex_t    *mutex )
{
	if ( int rval = pthread_cond_wait(&mCondition, mutex) )
		throw ( std::runtime_error("pthread_cond_wait") );
}

} /* namespace threads */

namespace rikiGlue
{

// 	

DecodeThread::~DecodeThread()
{
	mCondition.signal();
}

void
DecodeThread::decode( bool    setting )
{
	BlockQueue::iterator itr = mBlocks.begin();
	ThreadBlock  &block = itr->second;
	if ( block.mProcess == ThreadBlock::kWaiting )
	{
		block.mProcess = ThreadBlock::kProcessing;
		mMutex.unlock();
		
		for ( register_t i = 0; i < block.mNumOps; ++i )
		{
			if ( block.mOps[i](block.mBlock) )
				throw ( std::runtime_error("operation error") );
		}		

		mMutex.lock();
		block.mProcess = ThreadBlock::kSetBack;
	}

	if ( block.mProcess == ThreadBlock::kSetBack && setting )
	{
		block.mProcess = ThreadBlock::kDone;
		mMutex.unlock();

		block.mFrame->setBlock(itr->first, block.mBlock);
		mMutex.lock();
		mBlocks.erase(itr);
	}
	else
	{
		mMutex.unlock();
		mMutex.lock();
	}
}

void
DecodeThread::run()
{
	mMutex.lock();

	while ( true )
	{
		while ( mBlocks.empty() )
		{
			mCondition.wait(mMutex);
			if ( mExit )
			{
				mMutex.unlock();
				return;
			}
		}

		decode(mSetting);
	}
}

} /* namespace rikiGlue */
