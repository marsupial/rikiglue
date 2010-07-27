/*
	dmtx.cpp
*/

#include "frame/frame.h"
#include "dmtx/dmtx.h"
#include <string>

namespace rikiGlue
{
/*
	c01   c11
	c00   c10

bool
compareCorners( const DmtxCorners    &c0,
                const DmtxCorners    &c1 )
{
	return ( ((c0.c00.X > (c1.c00.X-1.0)) && (c0.c10.X < (c1.c10.X+1.0))) && 
	         ((c0.c00.Y > (c1.c00.Y-1.0)) && (c0.c11.Y < (c1.c11.Y+1.0))) );

//	(c0.c00.Y > (c1.c00.Y-1.0)) && (c0.c11.Y < (c1.c11.Y+1.0))
}
*/

static int
dmtxDecode( uint8_t  *pixels,
            int      width,
            int      height )
{
	DmtxImage  *img = dmtxImageCreate(pixels, width, height, DmtxPack24bppRGB);
	DmtxDecode *dec = dmtxDecodeCreate(img, 1);

	std::string  lastFound;
/*	dmtxDecodeSetProp(&dec, DmtxPropXmin, 0);
	dmtxDecodeSetProp(&dec, DmtxPropXmax, 100);
	dmtxDecodeSetProp(&dec, DmtxPropYmin, 0);
	dmtxDecodeSetProp(&dec, DmtxPropYmax, 100);
*/
	DmtxTime   msec = dmtxTimeAdd(dmtxTimeNow(), 1000/25);
	DmtxRegion *reg = dmtxRegionFindNext(dec, &msec);
	while ( reg != NULL )
	{
		DmtxMessage *msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
		if ( msg != NULL )
		{
			printf("found: \"");
			fwrite(msg->output, sizeof(unsigned char), msg->outputIdx, stdout);
			printf("\"\n");
			dmtxMessageDestroy(&msg);
/*
			std::string output(reinterpret_cast<const char*>(msg->output), msg->outputIdx);
			printf("output: \"%s\"\n", output.c_str(), found++);
			dmtxMessageFree(&msg);
			if ( output == lastFound )
				break;
			lastFound = output;
*/
		}
		dmtxRegionDestroy(&reg);
		msec = dmtxTimeAdd(dmtxTimeNow(), 1000/25);
		reg = dmtxRegionFindNext(dec, &msec);
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
