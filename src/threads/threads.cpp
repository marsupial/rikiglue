/*
	threads.cpp
*/

#include "threads/threads.h"
#include <stdexcept>

namespace rikiGlue
{

DecodeThread::~DecodeThread()
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

void
DecodeThread::decode()
{
	BlockQueue::iterator itr = mBlocks.begin();
	if ( itr != mBlocks.end() )
	{
		ThreadBlock block = *itr;
		mBlocks.erase(itr);
		mMutex.unlock();
		
		for ( register_t i = 0; i < block.numOps; ++i )
		{
			if ( block.ops[i](block) )
				throw ( std::runtime_error("operation error") );
		}
	}
	else
		mMutex.unlock();
}

void
DecodeThread::run()
{
	mMutex.lock();

	while ( true )
	{
		while ( mBlocks.empty() )
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

		decode();
		mMutex.lock();
	}
}

} /* namespace rikiGlue */
