/*
	frame.h
*/

#pragma once

#ifndef _frame_h__
#define _frame_h__

#include "glue.pch"
#include <vector>

namespace rikiGlue
{

class DecodeThread;

class Params
{
public:

	Params() {}
	virtual ~Params() {}

	virtual int operator [] ( int    i ) const = 0;
};

struct Frame
{
public:

	struct Block
	{

		Block( const uint8_t   *src,
		       size_t          srcSz,
		       uint8_t         *dst,
		       size_t          dstSz ) :
			srcData(src),
			srcSize(srcSz),
			dstData(dst),
			dstSize(dstSz)
		{}

		const uint8_t   *srcData;
		size_t          srcSize;

		uint8_t         *dstData;
		size_t          dstSize;
	};
	
	typedef register_t (*operation_t) ( const Frame::Block     &block );

	enum
	{
		kFormatARGB   = 0,
		kFormatBGRA   = 1,
		kFormatEnd
	};

	Frame( size_t          width,
	       size_t          height,
	       DecodeThread    *decoder = NULL ) :
		mDecoder(decoder),
		mWidth(width),
		mHeight(height),
		mRowbytes(mWidth*3),
		mPixels(mRowbytes*mHeight)
	{
	}

	register_t
	operate( operation_t    *ops,
	         size_t         numOps,
	         const uint8_t  *srcData,
	         size_t         srcRowbytes,
	         uint8_t        *dstData,
	         size_t         dstRowbytes );

	register_t
	operate( operation_t    *ops,
	         size_t         numOps,
	         const uint8_t  *srcData,
	         size_t         srcRowbytes )
	{
		return ( operate(ops, numOps, srcData, srcRowbytes, &mPixels[0], mRowbytes) );
	}

	uint8_t*
	bytes()
	{
		return ( &mPixels[0] );
	}

	size_t
	rowBytes()
	{
		return ( mRowbytes );
	}

private:

	DecodeThread   *mDecoder;

	const size_t   mWidth,
	               mHeight,
	               mRowbytes;

	std::vector<uint8_t>  mPixels;

};

register_t
argbToRGB( const Frame::Block    &block );

register_t
bgraToRGB( const Frame::Block    &block );

register_t
rgbToARGB( const Frame::Block    &block );

register_t
rgbToBGRA( const Frame::Block    &block );

register_t 
rsaEncrypt( const Frame::Block    &block );

register_t 
rsaDecrypt( const Frame::Block    &block );

register_t
lutEncrypt( const Frame::Block    &block );

register_t
lutDecrypt( const Frame::Block    &block );

register_t
rChannel( const Frame::Block    &block );

register_t
gChannel( const Frame::Block    &block );

register_t
bChannel( const Frame::Block    &block );


} /* namespace rikiGlue */

#endif /* _frame_h__ */