/*
	frame.cpp
*/

#include "frame/frame.h"
#include <assert.h>

namespace rikiGlue
{

register_t
argbToRGB( const Frame::Block    &block )
{
	assert(block.dstSize < block.srcSize );

	const uint8_t  *srcData = block.srcData;
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < block.dstSize; i += 3 )
	{
		srcData++;
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
	return ( 0 );
}

register_t
bgraToRGB( const Frame::Block    &block )
{
	assert(block.dstSize < block.srcSize );

	const uint8_t  *srcData = block.srcData;
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < block.dstSize; i += 3 )
	{
		dstData[0] = srcData[2];
		dstData[1] = srcData[1];
		dstData[2] = srcData[0];
		srcData += 4;
		dstData += 3;
	}
	return ( 0 );
}

register_t
rgbToARGB( const Frame::Block    &block )
{
	assert(block.dstSize > block.srcSize );

	//const uint8_t  *srcData = block.srcData;
	uint8_t  *srcData = const_cast<uint8_t*>(block.srcData);
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < block.srcSize; i += 3 )
	{
			*dstData++ = 255;
	#if defined(RUNROLL)
			uint8_t b = srcData[2];
			*dstData++ = *srcData; *srcData++ = b;
			*dstData++ = *srcData; *srcData++ = b;
			*dstData++ = *srcData++;
	#else
			memcpy(dstData, srcData, 3);
			memset(srcData, srcData[2], 3);
			dstData += 3;
			srcData += 3;
	#endif
	}
	return ( 0 );
}

register_t
rgbToBGRA( const Frame::Block    &block )
{
	assert(block.dstSize > block.srcSize );

	//const uint8_t  *srcData = block.srcData;
	uint8_t  *srcData = const_cast<uint8_t*>(block.srcData);
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < block.srcSize; i += 3 )
	{
		dstData[0] = srcData[2];
		dstData[1] = srcData[1];
		dstData[2] = srcData[0];
		memset(srcData, srcData[2], 3);
		srcData += 3;
		dstData += 4;
	}
	return ( 0 );
}
		  
register_t
Frame::operate( const operation_t    *ops,
                size_t               numOps,
                const uint8_t        *srcData,
                size_t               srcRowbytes,
                uint8_t              *dstData,
                size_t               dstRowbytes )

{
	if ( mDecoder )
	{
		for ( register_t i = 0; i < mHeight; ++i )
		{
			ThreadBlock block(srcData, srcRowbytes, dstData, dstRowbytes, ops, numOps);

			mDecoder->lock();
			mDecoder->queue().push_back(block);
			mDecoder->unlock();

			mDecoder->signal();

			srcData += srcRowbytes;
			dstData += dstRowbytes;
		}

		mDecoder->lock();
		while ( mDecoder->queue().size() || mDecoder->state() == DecodeThread::kActive )
		{
			mDecoder->work();
			mDecoder->lock();
		}
		mDecoder->unlock();
	}
	else
	{
		for ( register_t i = 0; i < mHeight; ++i )
		{
			Block block(srcData, srcRowbytes, dstData, dstRowbytes);

			for ( register_t o = 0; o < numOps; ++o )
			{
				if ( register_t rval = ops[o](block) )
					return ( rval );
			}

			srcData += srcRowbytes;
			dstData += dstRowbytes;
		}
	}

	return ( 0 );
}

} /* namespace rikiGlue */
