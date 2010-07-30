/*
   application.cpp
*/

#include "common/application.h"
#include "frame/frame.h"
#include <algorithm>
#include <string>

namespace rikiGlue
{

static Application
	sApplication;

Application&
Application::instance()
{
	return ( sApplication );
}

template <class T> static void
nop( T& )
{
}

template <class T> static void
deleter( T    *frame )
{
	delete frame;
}

template <class T, class P> static void
createThread( T   *&thread,
              P   param )
{
	thread = new T(param);
	if ( thread && thread->create() != 0 )
	{
		delete thread;
		thread = NULL;
	}
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
	mDMTXThread(NULL),
	mDMTXInstrThread(NULL),
	mRect(0.f, 0.f, 1.f, 1.f),
	mLocked(false)
{
}

Application::~Application()
{
	stopThreads();
}

void
Application::start()
{
	startThreads();
	loadCommands();
}

void
Application::startThreads()
{
	createThread(mPixelDecodeThread);
	createThread(mDMTXInstrThread);
	createThread(mDMTXThread, mDMTXInstrThread);
}

void
Application::stop()
{
	stopThreads();
	mInstructions.clear();
}

void
Application::stopThreads()
{
	destroyThread(mPixelDecodeThread, nop<ThreadBlock>);
	destroyThread(mDMTXThread, deleter<Frame>);
	destroyThread(mDMTXInstrThread, DMTXDecode::finished);
}

void
Application::dmtxFrame( Frame    *inFrame )
{
	std::auto_ptr<Frame> frame(inFrame);
	if ( !mDMTXThread )
	{
		DMTXReader  dmtx(mDMTXInstrThread);
		if ( dmtx.scan(inFrame) )
			frame.release();
		return;
	}

	mDMTXThread->lock();
	if ( mDMTXThread->queue().size() == 0 )
		mDMTXThread->queue().push_back(frame.get());
	else
	{
		Frame *dropFrame = mDMTXThread->queue()[0];
		mDMTXThread->queue()[0] = frame.get();
		delete dropFrame;
	}
	frame.release();
	mDMTXThread->unlock();
	mDMTXThread->signal();
}


const static register_t
	kWidth = 180-20,
	kHeight = 180-20,
	kOriginX = (1920/2)-(kWidth/2),
	kOriginY = (1080/2)-(kHeight/2);

bool
Application::addInstruction( const uint8_t       *bytes,
                             size_t              len,
                             const Context::Rect &rect )
{
	mRect.width = rect.width/kWidth;
	mRect.height = rect.height/kHeight;
	mRect.height = mRect.width;
	mRect.origin.x = rect.origin.x - (kOriginX*mRect.width);
	mRect.origin.y = rect.origin.y - (kOriginY*mRect.height);

	mLocked = true;

	const uint8_t   *end = bytes;
	while ( len )
	{
		const uint8_t val = *end;
		if ( val == ' ' || val == 0 )
		{
			std::string name(bytes, end);
			Commands_t::const_iterator itr = mCommands.find(name);
			if ( itr == mCommands.end() )
				return ( false );

			size_t argLen;
			const char *args;
			if ( val == 0 )
			{
				args = reinterpret_cast<const char*>(end);
				argLen = 0;
			}
			else
			{
				args = reinterpret_cast<const char*>(end+1);
				argLen = len-1;
			}
		
			std::auto_ptr<Command> cmd( itr->second(args, argLen) );
			if ( cmd.get() == NULL )
				return ( false );

			mInstructions.lock();
				mInstructions.push_back(cmd.get());
				cmd.release();
			mInstructions.unlock();
		}
		++end;
		--len;
	}
	return ( false );
}

void
Application::process( Context   &ctx )
{
	return ( mInstructions.process(ctx) );
}

void
Instructions::process( Context   &ctx )
{
	lock();
		list_t  tmp( mInstructions );
	unlock();

	list_t::iterator itr = tmp.begin();
	while ( itr != tmp.end() )
	{
		if ( (*itr)->doIt(ctx) == false )
			itr = tmp.erase(itr);
		else
			++itr;
	}

	lock();
		std::swap(mInstructions, tmp);
	unlock();
}

void
Instructions::clear()
{
	lock();
		if ( mInstructions.size() )
		{
			list_t tmp;
			std::swap(mInstructions, tmp);
			std::for_each(tmp.begin(), tmp.end(), deleter<Command>);
		}
	unlock();
}

} /* namespace rikiGlue */
