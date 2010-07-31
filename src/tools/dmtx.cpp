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
	printf("usage: %s [-t string] [-s size] [-r] <filepath> \n", filePath);
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
	if ( argc < 2 )
		return ( usage(argv[0]) );

	bytes_t        bytes;
	int            rval;
	const char     *filePath = argv[--argc];
	DmtxSymbolSize size = DmtxSymbolSquareAuto;

	for ( int i = 1; i < argc; ++i )
	{
		const char *arg = argv[i];
		if ( arg[0] == '-' )
		{

	
			switch ( arg[1] )
			{
				case 'r':
					rval = fileToBytes(stdin, bytes);
					if ( rval != 0 )
						return ( rval );
					break;

				case 't':
					if ( ++i >= argc )
						return ( usage(argv[0]) );
					bytes.insert(bytes.end(), argv[i], argv[i]+strlen(argv[i]));
					break;
			
				case 's':
					if ( ++i >= argc )
						return ( usage(argv[0]) );
					size = static_cast<DmtxSymbolSize>(atol(argv[i]));
					break;
			}
		}
	}
	if ( bytes.empty() )
	{
		rval = fileToBytes(stdin, bytes);
		if ( rval != 0 )
			return ( rval );
	}

	uint8_t &b = bytes[ bytes.size()-1 ];
	switch ( b )
	{
		case '\r':
		case '\n':
		case EOF:
			b = 0;
		case 0:
			break;

		default:
			bytes.push_back(0);
	}

//printf("Encoding \"%s\"\n", &bytes[0]);

	printf("Encoding %d bytes\n", bytes.size());

	DmtxEncode *enc = dmtxEncodeCreate();
	dmtxEncodeSetProp(enc, DmtxPropSizeRequest, size);
	dmtxEncodeDataMatrix(enc, bytes.size(), &bytes[0]);
	writeImage(filePath, *(enc->image));
	dmtxEncodeDestroy(&enc);

	return ( 0 );
}


