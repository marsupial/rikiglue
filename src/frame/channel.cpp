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
		memset(&data[0], data[c], 3);
		data += 3;
	}

	return ( 0 );
}

register_t
bChannel( Frame::Block    &block )
{
	return ( chOperation(2, block) );
}

} /* namespace rikiGlue */
