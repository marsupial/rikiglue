/*
	threads.pthreads.cpp
*/

#include "threads/threads.h"
#include <stdexcept>

namespace threads
{

static void*
threadRunner( void    *runner )
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	try
	{
		Thread *thread = static_cast<Thread*>(runner);
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
		pthread_t thread = mThread;
		mThread = pthread_self();
		pthread_join(thread, NULL);
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
Condition::broadcast()
{
	if ( pthread_cond_broadcast(&mCondition) != 0 )
		throw ( std::runtime_error("pthread_cond_broadcast") );
}

void
Condition::wait( mutex_t    *mutex )
{
	if ( int rval = pthread_cond_wait(&mCondition, mutex) )
		throw ( std::runtime_error("pthread_cond_wait") );
}

} /* namespace threads */

