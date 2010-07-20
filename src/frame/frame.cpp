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

register_t
Frame::operate( operation_t    *ops,
                size_t         nOps )
{
	for ( register_t i = 0; i < numBlocks(); ++i )
	{
		Block &block = getBlock(i);

		for ( register_t o = 0; o < nOps; ++o )
		{
			if ( register_t rval = ops[o](block) )
				return ( rval );
		}

		setBlock(i, block);
	}
	return ( 0 );
}

} /* namespace rikiGlue */
