/*
   context.cpp
*/

#include <ApplicationServices/ApplicationServices.h>

#include "commands/context.h"
#include "common/application.h"
#include "frame/frame.h"
#include "frame/operations.h"

namespace rikiGlue
{

Context::Context( CGContextRef  ctx,
                  CGImageRef    image,
                  const Rect    &bounds ) :
	mContext(ctx),
	mImage(image)
{
	CGContextSaveGState(mContext);

	const Context::Rect &rect = Application::instance().transform();
	CGContextTranslateCTM(mContext, rect.origin.x, rect.origin.y);
	CGContextScaleCTM(mContext, rect.width, rect.height);
}

Context::~Context()
{
	CGContextRestoreGState(mContext);
}

void
Context::setStrokeColor( float_t   r,
		                 float_t   g,
		                 float_t   b,
		                 float_t   a )
{
	CGContextSetRGBStrokeColor(mContext, r, g, b, a);
}

void
Context::setFillColor( float_t   r,
		               float_t   g,
		               float_t   b,
		               float_t   a )
{
	CGContextSetRGBFillColor(mContext, r, g, b, a);
}

void
Context::drawText( const std::string  &str,
                   const Point        &loc,
                   const float_t      fontSize,
                   const std::string  &font )
{
	CGContextSelectFont(mContext, font.c_str(), fontSize, kCGEncodingMacRoman);
	CGContextSetFontSize(mContext, fontSize/12.f);
	CGContextShowTextAtPoint(mContext, loc.x, loc.y, str.c_str(), str.length());
}

static CGRect
toCGRect( const Context::Rect    &rect )
{
	CGRect c = { { rect.origin.x, rect.origin.y}, { rect.width, rect.height } };
	return ( c );
}

void
Context::strokeRect( const Rect    &rect )
{
	CGContextStrokeRect(mContext, toCGRect(rect) );
}

bool
Context::blockOperation( const Rect         &rect,
                         Block::operation_t op )
{
	CGAffineTransform xform = CGContextGetCTM(mContext);
	const CGRect cgRect = toCGRect(rect);

	CGRect cgRectX = CGRectApplyAffineTransform(cgRect, xform);
	const size_t imageHeight = CGImageGetHeight(mImage);
	cgRectX.origin.y = imageHeight-(cgRectX.origin.y+cgRectX.size.height);

	const size_t width = static_cast<size_t>(cgRectX.size.width+0.5f);
	size_t height = static_cast<size_t>(cgRectX.size.height+0.5f);
	const size_t rowBytes = width * 4;
	std::vector<uint8_t> argb(rowBytes * height);
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGContextRef    contextRef = CGBitmapContextCreate(&argb[0], width, height, 8, rowBytes, colorSpace, kCGImageAlphaPremultipliedFirst);

	CGColorSpaceRelease(colorSpace);
	if ( contextRef == NULL )
	{
		argb.resize(0);
		return ( false );
	}

	CGImageRef imageRef = CGImageCreateWithImageInRect(mImage, cgRectX);
	CGRect iRect = { {0,0}, {CGImageGetWidth(imageRef), CGImageGetHeight(imageRef)} };
	CGContextDrawImage(contextRef, iRect, imageRef);
	CGImageRelease(imageRef);

	Frame frame(width, height, Application::instance().decoderThread());
	const Frame::operation_t  rgbOps[3] = { argbToRGB, op, rgbToARGB  };
	frame.operate(&rgbOps[0], 1, &argb[0], rowBytes);
	frame.operate(&rgbOps[1], 1, frame.bytes(), frame.rowBytes());
	frame.operate(&rgbOps[2], 1, frame.bytes(), frame.rowBytes(), &argb[0], rowBytes);

	imageRef = CGBitmapContextCreateImage(contextRef);
	CGContextRelease(contextRef);
//	CGContextSaveGState(mContext);
//		CGContextConcatCTM(mContext, CGAffineTransformInvert(xform));
//		CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, CGImageGetHeight(mImage));
//		CGContextConcatCTM(mContext, flipVertical);
		CGContextDrawImage(mContext, cgRect, imageRef);
		//CGContextConcatCTM(mContext, xform);
//	CGContextRestoreGState(mContext);
	CGImageRelease(imageRef);
}
			  
} /* namespace rikiGlue */
