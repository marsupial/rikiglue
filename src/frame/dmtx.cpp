/*
	dmtx.cpp
*/

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

static const int kEdge = 13;

struct LastRect
{
	DmtxPixelLoc  tl;
	DmtxPixelLoc  br;
	bool          valid;
};
static LastRect sLastRect = { {0,0}, {0,0}, false };

// in thread vs lock and push : 700 vs. 1400
// dealloc 2s .5fps vs:
// 2s .5 .01 70fps

static void
dmtxFound( DmtxDecode   *dec,
           DmtxRegion   *reg )
{
	sLastRect.tl.X = reg->leftLoc.X-kEdge;
	sLastRect.tl.Y = reg->topLoc.Y+kEdge;
	sLastRect.br.X = reg->rightLoc.X+kEdge;
	sLastRect.br.Y = reg->bottomLoc.Y-kEdge;
	sLastRect.valid  = true;

	DmtxMessage *msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
	if ( msg != NULL )
	{

		printf("found: \"");
		fwrite(msg->output, sizeof(unsigned char), msg->outputIdx, stdout);
		printf("\"\n");
		dmtxMessageDestroy(&msg);
	}
	// std::string output(reinterpret_cast<const char*>(msg->output), msg->outputIdx);
}

static bool
dmtxFind( DmtxDecode    *dec,
          long          timeoutMS )
{
	DmtxTime   msec = dmtxTimeAdd(dmtxTimeNow(), timeoutMS);
	DmtxRegion *reg = dmtxRegionFindNext(dec, &msec);

	if ( reg != NULL )
	{
		dmtxFound(dec, reg);
		dmtxRegionDestroy(&reg);
		return ( true );
	}

	return ( false );
}

inline int32_t
timeoutCalc( register_t   width,
             register_t   height )
{
	return ( (width*height) / 350 );
}

static int
dmtxDecode( uint8_t       *pixels,
            register_t    width,
            register_t    height )
{
	DmtxImage  *img = dmtxImageCreate(pixels, width, height, DmtxPack24bppRGB);

#if defined(_WINDOWS)
	dmtxImageSetProp(img, DmtxPropImageFlip, DmtxFlipY);
#endif

	int32_t    timeout;
	DmtxDecode *dec = dmtxDecodeCreate(img, 1);

	if ( sLastRect.valid )
	{
		dmtxDecodeSetProp(dec, DmtxPropXmin, sLastRect.tl.X);
		dmtxDecodeSetProp(dec, DmtxPropXmax, sLastRect.br.X);
		dmtxDecodeSetProp(dec, DmtxPropYmin, sLastRect.br.Y);
		dmtxDecodeSetProp(dec, DmtxPropYmax, sLastRect.tl.Y);
		timeout = 1000 / 25;
	}
	else
		timeout = timeoutCalc(width, height);

	if ( dmtxFind(dec, timeout) == false )
	{
		printf("not found with timeout: %d and valid: %d\n", timeout, sLastRect.valid);
		if ( sLastRect.valid )
		{
			sLastRect.valid = false;
			dmtxDecodeSetProp(dec, DmtxPropXmin, 0);
			dmtxDecodeSetProp(dec, DmtxPropXmax, width);
			dmtxDecodeSetProp(dec, DmtxPropYmin, 0);
			dmtxDecodeSetProp(dec, DmtxPropYmax, height);
			dmtxFind(dec, timeoutCalc(width, height));
		}
	}

	dmtxDecodeDestroy(&dec);
	dmtxImageDestroy(&img);

	return ( 0 );
}

} /* namespace rikiGlue */

namespace threads
{

template <> void
rikiGlue::DMTXThread::work( rikiGlue::Frame   *&frame )
{
	rikiGlue::dmtxDecode(frame->bytes(), frame->width(), frame->height());
	delete frame;
}

} /* namespace threads */
