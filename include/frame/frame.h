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

	typedef std::vector<uint8_t> Block;
	typedef register_t (*operation_t) ( Frame::Block     &block );

	enum
	{
		kFormatARGB   = 0,
		kFormatBGRA   = 1,
		kFormatEnd
	};

	Frame( void    *data,
	       size_t  rowbytes,
	       size_t  width,
	       size_t  height,
	       uint8_t format = kFormatARGB ) :
		mData(reinterpret_cast<uint8_t*>(data)),
		mWidth(width),
		mHeight(height),
		mRowbytes(rowbytes),
		mBlock(width*3),
		mFormat(format)
	{
	}

	register_t
	operate( operation_t    ops[],
	         size_t         numOps,
	         DecodeThread   *thread = NULL );

	register_t
	operate( operation_t    op,
	         DecodeThread   *thread = NULL )
	{
		return ( operate(&op, 1, thread) );
	}

	void
	setBlock( size_t    i,
	          Block     &block );

private:

	size_t
	numBlocks() const
	{
		return ( mHeight );
	}

	Block&
	getBlock( size_t    i );

	uint8_t       *mData;

	const size_t   mWidth,
	               mHeight,
	               mRowbytes;

	Block          mBlock;
	const uint8_t  mFormat;
};

register_t 
rsaEncrypt( Frame::Block    &block );

register_t 
rsaDecrypt( Frame::Block    &block );

register_t 
ecEncode( Frame::Block    &block );

register_t 
ecDecode( Frame::Block    &block );

register_t
lutEncrypt( Frame::Block    &block );

register_t
lutDecrypt( Frame::Block    &block );

register_t
rChannel( Frame::Block    &block );

register_t
gChannel( Frame::Block    &block );

register_t
bChannel( Frame::Block    &block );

register_t
rec709( Frame::Block     &block );

} /* namespace rikiGlue */

#endif /* _frame_h__ */