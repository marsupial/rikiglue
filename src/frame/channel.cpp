/*
	channel.cpp
*/

#include "frame/frame.h"

namespace rikiGlue
{

static register_t
chOperation( uint8_t         c,
             Frame::Block    &block )
{
	const register_t n = block.size() / 3;
	uint8_t *data = &block[0];
	for ( register_t i = 0; i < n; ++i )
	{
#if defined(RUNROLL)
		const uint8_t val = data[c];
		*data++ = val;
		*data++ = val;
		*data++ = val;
#else
		memset(&data[0], data[c], 3);
		data += 3;
#endif
	}

	return ( 0 );
}

register_t
rChannel( Frame::Block    &block )
{
	return ( chOperation(0, block) );
}

register_t
gChannel( Frame::Block    &block )
{
	return ( chOperation(1, block) );
}

register_t
bChannel( Frame::Block    &block )
{
	return ( chOperation(2, block) );
}

} /* namespace rikiGlue */
