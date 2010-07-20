/*
	frame.cpp
*/

#include "frame/frame.h"
#include <assert.h>

namespace rikiGlue
{

Frame::Block&
Frame::getBlock( size_t    i )
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
Frame::setBlock( size_t    i,
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

}