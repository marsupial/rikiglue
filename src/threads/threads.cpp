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
	if ( mExit != true )
		mExit = true;
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
		block.mFrame->setBlock(itr->first, block.mBlock);
		mBlocks.erase(itr);
	}

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
			mCondition.wait(mMutex);
			if ( mExit )
			{
				mMutex.unlock();
				return;
			}
		}

		decode();
		mMutex.lock();
	}
}

} /* namespace rikiGlue */
