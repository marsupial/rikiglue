/*
	frame.h
*/

#pragma once

#ifndef _frame_h__
#define _frame_h__

#include <vector>

namespace rikiGlue
{

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

	Frame( void    *data,
	       size_t  rowbytes,
	       size_t  width,
	       size_t  height ) :
		mData(reinterpret_cast<uint8_t*>(data)),
		mWidth(width),
		mHeight(height),
		mRowbytes(rowbytes),
		mBlock(width*3)
	{
	}

	register_t
	operate( operation_t    ops[],
	         size_t         nOps );

	register_t
	operate( operation_t    op )
	{
		return ( operate(&op, 1) );
	}

private:

	size_t
	numBlocks() const
	{
		return ( mHeight );
	}

	Block&
	getBlock( size_t    i );

	void
	setBlock( size_t    i,
	          Block     &block );

	uint8_t       *mData;

	const size_t  mWidth,
	              mHeight,
	              mRowbytes;

	Block         mBlock;
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
bChannel( Frame::Block    &block );

} /* namespace rikiGlue */

#endif /* _frame_h__ */