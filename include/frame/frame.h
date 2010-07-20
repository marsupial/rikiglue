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

	int 
	rsaEncrypt();

	int 
	rsaDecrypt();

	int 
	ecEncode();

	int 
	ecDecode();

	int
	lutEncrypt();

	int
	lutDecrypt();

private:

	typedef std::vector<uint8_t> Block;

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

	void
	lutCrypt( unsigned char    lut[256] );

	void
	lutCrypt( unsigned char    lut[3][256] );


	uint8_t       *mData;

	const size_t  mWidth,
	              mHeight,
	              mRowbytes;

	Block         mBlock;
};

} /* namespace rikiGlue */

#endif /* _frame_h__ */