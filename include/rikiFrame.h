/*
	rikiFrame.h
*/

#pragma once

#ifndef _rikiFrame_h__
#define _rikiFrame_h__

#include <vector>
#include <assert.h>

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
	       size_t  height,
	       uint8_t comp ) :
		mData(reinterpret_cast<uint8_t*>(data)),
		mWidth(width),
		mHeight(height),
		mRowbytes(rowbytes),
		mComponents(comp),
		mBlock(width*3)
	{
	}


	typedef std::vector<uint8_t> Block;

	size_t
	numBlocks() const
	{
		return ( mHeight );
	}

	Block&
	getBlock( size_t    i )
	{
		assert( i < mHeight );

		const uint8_t *srcData = mData + (mRowbytes * i);
		uint8_t *dstData = &mBlock[0];
		for ( register_t x = 0; x < mWidth; ++x)
		{
			++srcData;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
		}

		return ( mBlock );
	}

	void
	setBlock( size_t    i,
	          Block     &block )
	{
		assert( i < mHeight );

		const uint8_t *srcData = &mBlock[0];
		uint8_t *dstData = mData + (mRowbytes * i);
		for ( register_t x = 0; x < mWidth; ++x )
		{
			*dstData++ = 255;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
		}
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
	colorShift( const Params  &param );

	int
	lutEncrypt();

	int
	lutDecrypt();

	int
	rot13();

private:

	void
	lutCrypt( unsigned char    lut[256] );

	void
	lutCrypt( unsigned char    lut[3][256] );


	uint8_t       *mData;

	const size_t  mWidth,
	              mHeight,
	              mRowbytes;

	Block         mBlock;

	const uint8_t mComponents;
};



} /* namespace rikiGlue */

#endif /* _rikiFrame_h__ */