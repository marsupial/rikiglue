/*
	dmtx.cpp
*/

#include "common/application.h"
#include "frame/frame.h"
#include "dmtx/dmtx.h"
#include <string>

/*
#import <Cocoa/Cocoa.h>
NSDate* date() { return ( [ NSDate date ] ); }
void time(NSDate *date) { double i = - [ date timeIntervalSinceNow ]; printf("time: %f, %f fps\n", i, 1.0/i); }
*/

namespace rikiGlue
{


struct LastRect
{
	DmtxPixelLoc  tl;
	DmtxPixelLoc  br;
	bool          valid;
};

static const int kEdge = 10;
static LastRect sLastRect = { {0,0}, {0,0}, false };

void
DMTXDecode::finished( DMTXDecode  &decode )
{
	dmtxRegionDestroy(&decode.reg);
	dmtxImageDestroy(&decode.dec->image);
	dmtxDecodeDestroy(&decode.dec);
	delete decode.frame;
}

inline DMTXThread::timeout_t
timeoutCalc( Frame    *frame )
{
	return ( (frame->width() * frame->height()) / 350 );
}

static void
dmtxDecode( DMTXDecode   &decode )
{
	DmtxMessage *msg = dmtxDecodeMatrixRegion(decode.dec, decode.reg, DmtxUndefined);
	if ( msg != NULL )
	{
		const Context::Rect rect( sLastRect.tl.X+kEdge, sLastRect.br.Y+kEdge,
		                         (sLastRect.br.X-sLastRect.tl.X)-(kEdge*2),
		                         (sLastRect.tl.Y-sLastRect.br.Y)-(kEdge*2)+1 );
		Application::instance().addInstruction(msg->output, msg->outputIdx, rect);
		dmtxMessageDestroy(&msg);
	}

	DMTXDecode::finished(decode);
}

void
DMTXReader::process( Frame         *frame,
                     DmtxDecode    *dec,
                     DmtxRegion    *reg )
{
	sLastRect.tl.X = reg->leftLoc.X-kEdge;
	sLastRect.tl.Y = reg->topLoc.Y+kEdge;
	sLastRect.br.X = reg->rightLoc.X+kEdge;
	sLastRect.br.Y = reg->bottomLoc.Y-kEdge;
	sLastRect.valid  = true;

	DMTXDecode  decode = { frame, dec, reg };
	if ( mInstThread )
	{
		mInstThread->lock();
			mInstThread->queue().push_back(decode);
		mInstThread->unlock();
		mInstThread->signal();
	}
	else
		dmtxDecode(decode);
}

bool
DMTXReader::find( Frame         *frame,
                  DmtxDecode    *dec,
                  timeout_t     timeout )
{
	DmtxTime   msec = dmtxTimeAdd(dmtxTimeNow(), timeout);
	DmtxRegion *reg = dmtxRegionFindNext(dec, &msec);

	if ( reg != NULL )
	{
		process(frame, dec, reg);
		return ( true );
	}

	return ( false );
}

bool
DMTXReader::scan( Frame    *frame )
{
	DmtxImage  *img = dmtxImageCreate(frame->bytes(), frame->width(), frame->height(), DmtxPack24bppRGB);

#if defined(_WINDOWS)
	dmtxImageSetProp(img, DmtxPropImageFlip, DmtxFlipY);
#endif

	timeout_t   timeout;
	DmtxDecode  *dec = dmtxDecodeCreate(img, 1);

	if ( sLastRect.valid )
	{
		dmtxDecodeSetProp(dec, DmtxPropXmin, sLastRect.tl.X);
		dmtxDecodeSetProp(dec, DmtxPropXmax, sLastRect.br.X);
		dmtxDecodeSetProp(dec, DmtxPropYmin, sLastRect.br.Y);
		dmtxDecodeSetProp(dec, DmtxPropYmax, sLastRect.tl.Y);
		timeout = 1000 / 25;
	}
	else
		timeout = timeoutCalc(frame);

	if ( find(frame, dec, timeout) )
		return ( true );
	
	if ( sLastRect.valid )
	{
		sLastRect.valid = false;
		dmtxDecodeSetProp(dec, DmtxPropXmin, 0);
		dmtxDecodeSetProp(dec, DmtxPropXmax, frame->width());
		dmtxDecodeSetProp(dec, DmtxPropYmin, 0);
		dmtxDecodeSetProp(dec, DmtxPropYmax, frame->height());
		
		timeout = timeoutCalc(frame);
		if ( find(frame, dec, timeout) )
			return ( true );
	}
/*
	if ( find(frame, dec, timeout) )
		return ( true );
*/
	dmtxDecodeDestroy(&dec);
	dmtxImageDestroy(&img);
	
	Application::instance().notLocked();

	return ( false );
}

} /* namespace rikiGlue */

namespace threads
{

template <> void
Worker<rikiGlue::Frame*>::work( rikiGlue::Frame   *&frame )
{
	if ( static_cast<rikiGlue::DMTXThread*>(this)->scan(frame) == false )
		delete frame;
}

template <> void
rikiGlue::DMTXInstrThread::work( rikiGlue::DMTXDecode   &decode )
{
	rikiGlue::dmtxDecode(decode);
}

} /* namespace threads */
