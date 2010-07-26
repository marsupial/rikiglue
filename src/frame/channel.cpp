/*
	channel.cpp
*/

#include "frame/frame.h"

namespace rikiGlue
{

static register_t
chOperation( uint8_t               c,
             const Frame::Block    &block )
{
	const register_t n = block.dstSize / 3;
	const uint8_t *srcData = block.srcData;
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < n; ++i )
	{
#if defined(RUNROLL)
		const uint8_t val = block.srcData[c];
		*dstData++ = val;
		*dstData++ = val;
		*dstData++ = val;
#else
		memset(dstData, srcData[c], 3);
		dstData += 3;
		srcData += 3;
#endif
	}

	return ( 0 );
}

register_t
rChannel( const Frame::Block    &block )
{
	return ( chOperation(0, block) );
}

register_t
gChannel( const Frame::Block    &block )
{
	return ( chOperation(1, block) );
}

register_t
bChannel( const Frame::Block    &block )
{
	return ( chOperation(2, block) );
}

} /* namespace rikiGlue */
