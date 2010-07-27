/*
	dmt.cpp
*/

#include <stdio.h>
#include <string.h>

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#include "dmtx/dmtx.h"
#include "common.cpp"

static int
usage( const char    *filePath )
{
	printf("usage: %s [string] <filepath> \n", filePath);
	return ( -1 );
}

static int
writeImage( const char    *filePath,
            DmtxImage     &img )
{
	bool result = false;

	CFURLRef urlRef = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, reinterpret_cast<const UInt8*>(filePath), strlen(filePath), false);
	CGImageDestinationRef imageDest = CGImageDestinationCreateWithURL(urlRef, kUTTypePNG, 1, NULL);
	if ( imageDest != NULL )
	{
		CGColorSpaceRef	colorspace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
		CGDataProviderRef provider = CGDataProviderCreateWithData(nil, img.pxl, img.width * 3 * img.height, nil);
		CGImageRef image = CGImageCreate(img.width, img.height, 8, 24, img.width * 3, colorspace, kCGImageAlphaNone, provider, NULL, false, kCGRenderingIntentDefault);
		CGImageDestinationAddImage(imageDest, image, NULL);

		result = CGImageDestinationFinalize(imageDest);

		CFRelease(imageDest);
		CGImageRelease(image);
		CGDataProviderRelease(provider);
		CGColorSpaceRelease(colorspace);
	}
	else
		result = false;
	
	CFRelease(urlRef);
	return ( result );
}

extern "C" int
main( int        argc,
      const char **argv )
{
	bytes_t    bytes;
	const char *filePath = NULL;
	if ( argc < 2 )
		return ( usage(argv[0]) );
	else if ( argc > 2 )
	{
		filePath = argv[2];
		bytes.assign( argv[1], argv[1]+strlen(argv[1]));
	}
	else
	{
		filePath = argv[1];
		int rval = fileToBytes(stdin, bytes);
		if ( rval != 0 )
			return ( rval );
	}
	if ( bytes[ bytes.size()-1 ] )
		bytes.push_back(0);

	printf("Encoding %d bytes\n", bytes.size());

   /* 1) ENCODE a new Data Matrix barcode image (in memory only) */
	DmtxEncode *enc = dmtxEncodeCreate();
	dmtxEncodeDataMatrix(enc, bytes.size(), &bytes[0]);
	writeImage(filePath, *(enc->image));
	dmtxEncodeDestroy(&enc);

	return ( 0 );
}


