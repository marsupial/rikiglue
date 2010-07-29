/*
	application.h
*/

#pragma once

#ifndef _application_h__
#define _application_h__

#include "frame/threads.h"

namespace rikiGlue
{

class Application
{
public:

	Application();
	~Application();

	void
	startThreads();

	void
	stopThreads();

	void
	dmtxFrame( Frame    *inFrame );

	DecodeThread*
	decoderThread()
	{
		return ( mPixelDecodeThread );
	}

private:

	DecodeThread    *mPixelDecodeThread;

	DMTXThread      *mDMTXThread;
	DMTXInstrThread *mDMTXInstrThread;
};

} /* namespace rikiGlue */

#endif /* _application_h__ */