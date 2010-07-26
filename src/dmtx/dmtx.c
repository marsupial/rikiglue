/*
libdmtx - Data Matrix Encoding/Decoding Library

Copyright (c) 2008 Mike Laughton

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact: mike@dragonflylogic.com
*/

/* $Id: dmtx.c 418 2008-09-04 19:22:15Z mblaughton $ */

/**
 * @file dmtx.c
 * @brief Main libdmtx source file
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include "dmtx/dmtx.h"
#include "dmtx/dmtxstatic.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef CALLBACK_POINT_PLOT
#define CALLBACK_POINT_PLOT(a,b,c,d) ((void) 0)
#endif

#ifndef CALLBACK_POINT_XFRM
#define CALLBACK_POINT_XFRM(a,b,c,d) ((void) 0)
#endif

#ifndef CALLBACK_MODULE
#define CALLBACK_MODULE(a,b,c,d,e) ((void) 0)
#endif

#ifndef CALLBACK_MATRIX
#define CALLBACK_MATRIX(a,b) ((void) 0)
#endif

#ifndef CALLBACK_FINAL
#define CALLBACK_FINAL(a,b) ((void) 0)
#endif

#include "dmtxtime.c"
#include "dmtxscangrid.c"
#include "dmtxmessage.c"
#include "dmtxregion.c"
#include "dmtxdecode.c"
#include "dmtxencode.c"
#include "dmtxplacemod.c"
#include "dmtxfec.c"
#include "dmtxreedsol.c"
#include "dmtxsymbol.c"
#include "dmtxvector2.c"
#include "dmtxmatrix3.c"
#include "dmtxcolor3.c"
#include "dmtximage.c"

extern char *
dmtxVersion(void)
{
   return DMTX_VERSION;
}

extern int
dmtxDecode( const unsigned char *pixels,
            int                 width,
            int                 height )
{
	DmtxImage img = { 0 };
	img.compass = (DmtxCompassEdge*) calloc(width * height, sizeof(DmtxCompassEdge));
	if ( img.compass == NULL )
		return ( -1 );

	img.pxl = pixels;
	img.pageCount = 1;
	img.width = img.widthScaled = width;
	img.height = img.heightScaled = height;
	img.scale = 1;
	img.xMin = img.xMinScaled = 0;
	img.xMax = img.xMaxScaled = width - 1;
	img.yMin = img.yMinScaled = 0;
	img.yMax = img.yMaxScaled = height - 1;


	DmtxDecode dec = dmtxDecodeStructInit(&img);

	DmtxTime   msec = dmtxTimeAdd(dmtxTimeNow(), 1000);
	DmtxRegion reg = dmtxDecodeFindNextRegion(&dec, &msec);
	while ( reg.found >= DMTX_REGION_FOUND )
	{
		DmtxMessage *msg = dmtxDecodeMatrixRegion(&img, &reg, -1);
		if ( msg != NULL )
		{
			fputs("output: \"", stdout);
			fwrite(msg->output, sizeof(unsigned char), msg->outputIdx, stdout);
			fputs("\"\n\n", stdout);
			dmtxMessageFree(&msg);
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
		msec = dmtxTimeAdd(dmtxTimeNow(), 1000);
		reg = dmtxDecodeFindNextRegion(&dec, &msec);
	}

	dmtxDecodeStructDeInit(&dec);
	free(img.compass);
	return ( 0 );
}
