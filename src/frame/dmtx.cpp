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
*/
bool
compareCorners( const DmtxCorners    &c0,
                const DmtxCorners    &c1 )
{
	return ( ((c0.c00.X > (c1.c00.X-1.0)) && (c0.c10.X < (c1.c10.X+1.0))) && 
	         ((c0.c00.Y > (c1.c00.Y-1.0)) && (c0.c11.Y < (c1.c11.Y+1.0))) );

//	(c0.c00.Y > (c1.c00.Y-1.0)) && (c0.c11.Y < (c1.c11.Y+1.0))
}

extern int
dmtxDecode( uint8_t  *pixels,
            int      width,
            int      height )
{
printf("dmtxDecode entered\n");


	DmtxImage img = { 0 };
	img.compass = (DmtxCompassEdge*) calloc(width * height, sizeof(DmtxCompassEdge));
	if ( img.compass == NULL )
		return ( -1 );


	img.pxl = reinterpret_cast<DmtxRgb*>(pixels);
	img.pageCount = 1;
	img.width = img.widthScaled = width;
	img.height = img.heightScaled = height;
	img.scale = 1;
	img.xMin = img.xMinScaled = 0;
	img.xMax = img.xMaxScaled = width - 1;
	img.yMin = img.yMinScaled = 0;
	img.yMax = img.yMaxScaled = height - 1;

/*
16, 118 96, 118
16, 36	96, 36
*/

	const DmtxCornerLoc cf = static_cast<DmtxCornerLoc>( DmtxCorner00 | DmtxCorner10 | DmtxCorner11 | DmtxCorner01 );
	DmtxCorners corners = { cf, {0,0}, {0,0}, {0,0}, {0,0} };
	int found = 0;
	std::string  lastFound;

	DmtxDecode dec = dmtxDecodeStructInit(&img);

	dmtxDecodeSetProp(&dec, DmtxPropXmin, 0);
	dmtxDecodeSetProp(&dec, DmtxPropXmax, 100);
	dmtxDecodeSetProp(&dec, DmtxPropYmin, 0);
	dmtxDecodeSetProp(&dec, DmtxPropYmax, 100);

	DmtxTime   msec = dmtxTimeAdd(dmtxTimeNow(), 1000/25);
	DmtxRegion reg = dmtxDecodeFindNextRegion(&dec, &msec);
	while ( reg.found >= DMTX_REGION_FOUND )
	{
		if ( compareCorners(reg.corners, corners) )
		{
				printf("SAME\n");
				break;
		}
		memcpy(&corners, &reg.corners, sizeof(DmtxCorners));

		DmtxMessage *msg = dmtxDecodeMatrixRegion(&img, &reg, -1);
		if ( msg != NULL )
		{
			printf("found: \"");
			fwrite(msg->output, sizeof(unsigned char), msg->outputIdx, stdout);
			printf("\"\n");
/*
			std::string output(reinterpret_cast<const char*>(msg->output), msg->outputIdx);
			printf("output: \"%s\"\n", output.c_str(), found++);
			dmtxMessageFree(&msg);
			if ( output == lastFound )
				break;
			lastFound = output;
*/
		}
/*
		//const int full = (DmtxCorner00 | DmtxCorner10 | DmtxCorner11 | DmtxCorner01);
		const int known = reg.corners.known ;
		if ( known & DmtxCorner00 )
		{
			if ( known & DmtxCorner10 )
				dec.grid.jumpSize = reg.corners.c10.X - reg.corners.c00.X;
			else if ( known & DmtxCorner11 )
				dec.grid.jumpSize = reg.corners.c11.X - reg.corners.c00.X;
			else if ( known & DmtxCorner01 )
				dec.grid.jumpSize = reg.corners.c01.Y - reg.corners.c00.Y;
		}
		else if ( known & DmtxCorner10 )
		{
			if ( known & DmtxCorner01 )
				dec.grid.jumpSize = reg.corners.c10.X - reg.corners.c01.X;
			else if ( known & DmtxCorner11 )
				dec.grid.jumpSize = reg.corners.c11.Y - reg.corners.c10.Y;
			else if ( known & DmtxCorner01 )
				dec.grid.jumpSize = reg.corners.c01.Y - reg.corners.c10.Y;
		}
		else if ( known & DmtxCorner11 )
		{
			if ( known & DmtxCorner01 )
				dec.grid.jumpSize = reg.corners.c11.X - reg.corners.c01.X;
		}
*/
		msec = dmtxTimeAdd(dmtxTimeNow(), 1000/25);
		reg = dmtxDecodeFindNextRegion(&dec, &msec);
	}

	dmtxDecodeStructDeInit(&dec);
	free(img.compass);
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
