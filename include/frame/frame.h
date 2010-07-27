/*
	frame.h
*/

#pragma once

#ifndef _frame_h__
#define _frame_h__

#include "frame/threads.h"

namespace rikiGlue
{

struct Frame
{
public:

	typedef rikiGlue::Block     Block;
	typedef Block::operation_t  operation_t;

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
		mPixels( new uint8_t[mRowbytes*mHeight] )
	{
	}

	~Frame()
	{
		if ( mPixels )
			delete [] mPixels;
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
		return ( operate(ops, numOps, srcData, srcRowbytes, mPixels, mRowbytes) );
	}

	uint8_t*
	bytes()
	{
		return ( mPixels );
	}

	size_t
	rowBytes() const
	{
		return ( mRowbytes );
	}

	size_t
	width() const
	{
		return ( mWidth );
	}
	
	size_t
	height() const
	{
		return ( mHeight );
	}

	uint8_t*
	release()
	{
		uint8_t *pixels = mPixels;
		mPixels = NULL;
		return ( pixels );
	}

private:

	DecodeThread   *mDecoder;

	const size_t   mWidth,
	               mHeight,
	               mRowbytes;

	uint8_t        *mPixels;

};

} /* namespace rikiGlue */

#endif /* _frame_h__ */