/*
	frame.cpp
*/

#include "frame/frame.h"
#include "threads/threads.h"
#include <assert.h>

namespace rikiGlue
{

Frame::Block&
Frame::getBlock( size_t    i )
{
	assert( i < mHeight );
	assert(mFormat < kFormatEnd);

	const uint8_t *srcData = mData + (mRowbytes * i);
	uint8_t *dstData = &mBlock[0];

	if ( mFormat == kFormatARGB )
	{
		for ( register_t x = 0; x < mWidth; ++x)
		{
	#if defined(RUNROLL)
			++srcData;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
	#else
			memcpy(dstData, &srcData[1], 3);
			dstData += 3;
			srcData += 4;
	#endif
		}
	}
	else
	{
		for ( register_t x = 0; x < mWidth; ++x )
		{
			dstData[0] = srcData[2];
			dstData[1] = srcData[1];
			dstData[2] = srcData[0];
			srcData += 4;
			dstData += 3;
		}
	}


	return ( mBlock );
}

void
Frame::setBlock( size_t    i,
	             Block     &block )
{
	assert( i < mHeight );
	assert(mFormat < kFormatEnd);

	const uint8_t *srcData = &block[0];
	uint8_t *dstData = mData + (mRowbytes * i);

	if ( mFormat == kFormatARGB )
	{
		for ( register_t x = 0; x < mWidth; ++x )
		{
			*dstData++ = 255;
	#if defined(RUNROLL)
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
			*dstData++ = *srcData++;
	#else
			memcpy(dstData, srcData, 3);
			dstData += 3;
			srcData += 3;
	#endif
		}
	}
	else
	{
		for ( register_t x = 0; x < mWidth; ++x )
		{
			dstData[0] = srcData[2];
			dstData[1] = srcData[1];
			dstData[2] = srcData[0];
			srcData += 3;
			dstData += 4;
		}
	}
}

register_t
Frame::operate( operation_t    *ops,
                size_t         numOps,
                DecodeThread   *inDecoder )
{
	if ( inDecoder )
	{
		DecodeThread &decoder = *inDecoder;

		for ( register_t i = 0; i < numBlocks(); ++i )
		{
			const Block &block = getBlock(i);

			decoder.lock();
			decoder.queue()[i] = ThreadBlock(this, ops, numOps);
			decoder.queue()[i].mBlock = block;
			decoder.unlock();

			decoder.signal();
		}

		decoder.lock();
		while ( decoder.queue().size() )
		{
			decoder.decode();
			decoder.lock();
		}
		decoder.unlock();
	}
	else
	{
		for ( register_t i = 0; i < numBlocks(); ++i )
		{
			Block &block = getBlock(i);

			for ( register_t o = 0; o < numOps; ++o )
			{
				if ( register_t rval = ops[o](block) )
					return ( rval );
			}

			setBlock(i, block);
		}
	}

	return ( 0 );
}

} /* namespace rikiGlue */
