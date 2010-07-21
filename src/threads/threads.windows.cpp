/*
	threads.windows.cpp
*/

#include "threads/threads.h"
#include <stdexcept>

namespace threads
{

static DWORD WINAPI
threadRunner( void    *runner )
{
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

	return ( NULL );
}

Thread::Thread()
{
	mThread.handle = INVALID_HANDLE_VALUE;
	mThread.id = 0;
}

Thread::~Thread()
{
	if ( mThread.handle != INVALID_HANDLE_VALUE )
	{
		::WaitForSingleObject(mThread.handle, INFINITE);
		::CloseHandle(mThread.handle);

	}
}

register_t
Thread::create()
{
	mThread.handle = ::CreateThread(0, 0, threadRunner, this, 0, &mThread.id);
	if ( mThread.handle == INVALID_HANDLE_VALUE )
		return ( -1 );

	return ( 0 );
}

Mutex::Mutex() :
	mMutex( ::CreateMutex(0, FALSE, 0) )
{
	if ( mMutex == NULL )
		throw ( std::runtime_error("CreateMutex") );
}

Mutex::~Mutex() 
{
	::CloseHandle(mMutex);
}

void
Mutex::lock()
{
	if ( ::WaitForSingleObject(mMutex, INFINITE) == WAIT_FAILED )
		throw ( std::runtime_error("WaitForSingleObject") );
}

void
Mutex::unlock()
{
	if ( ::ReleaseMutex(mMutex) == 0 )
		throw ( std::runtime_error("ReleaseMutex") );
}

Condition::Condition() :
	mCondition( ::CreateEvent(0, FALSE, FALSE, L"gabbaCondition") )
{
	if ( mCondition == NULL )
		throw ( std::runtime_error("CreateEvent") );
}

Condition::~Condition() 
{
	::CloseHandle(mCondition);
}

void
Condition::signal()
{
	if ( ::SetEvent(mCondition) == 0 )
		throw ( std::runtime_error("SetEvent") );
}

void
Condition::wait( mutex_t    *mutex )
{
	HANDLE mMutex = *mutex;
	if ( ::ReleaseMutex(mMutex) == 0 )
		throw ( std::runtime_error("ReleaseMutex") );

	if ( ::WaitForSingleObject(mCondition, INFINITE) ==  WAIT_FAILED )
		throw ( std::runtime_error("WaitForSingleObject") );

	if ( ::WaitForSingleObject(mMutex, INFINITE) == WAIT_FAILED )
		throw ( std::runtime_error("WaitForSingleObject") );
}

} /* namespace threads */

