/*
   application.cpp
*/

#include "common/application.h"
#include "frame/frame.h"
#include <algorithm>

namespace rikiGlue
{

template <class T> static void
nop( T& )
{
}

static void
frameDelete( Frame    *frame )
{
	delete frame;
}

template <class T> static void
createThread( T   *&thread )
{
	thread = new T;
	if ( thread && thread->create() != 0 )
	{
		delete thread;
		thread = NULL;
	}
}

template <class T, class F> static void
destroyThread( T   *&inThread,
               F   op = NULL )
{
	if ( inThread )
	{
		T *thread = inThread;
		inThread = NULL;
		typename T::Queue queue;
		thread->lock();
			std::swap(queue, thread->queue());
			thread->exit();
		thread->unlock();
		thread->signal();
		
		std::for_each(queue.begin(), queue.end(), op);
		delete thread;
	}
}

Application::Application() :
	mPixelDecodeThread(NULL),
	mInstructionDecodeThread(NULL)
{
}

Application::~Application()
{
	stopThreads();
}

void
Application::startThreads()
{
	createThread(mPixelDecodeThread);
	createThread(mInstructionDecodeThread);
}

void
Application::stopThreads()
{
	destroyThread(mPixelDecodeThread, nop<ThreadBlock>);
	destroyThread(mInstructionDecodeThread, frameDelete);
}

void
Application::dmtxFrame( Frame    *inFrame )
{
	std::auto_ptr<Frame> frame(inFrame);
	if ( !mInstructionDecodeThread )
		return;

	mInstructionDecodeThread->lock();
	if ( mInstructionDecodeThread->queue().size() == 0 )
		mInstructionDecodeThread->queue().push_back(frame.get());
	else
	{
		Frame *dropFrame = mInstructionDecodeThread->queue()[0];
		mInstructionDecodeThread->queue()[0] = frame.get();
		delete dropFrame;
	}
	frame.release();
	mInstructionDecodeThread->unlock();
	mInstructionDecodeThread->signal();
}

} /* namespace rikiGlue */
