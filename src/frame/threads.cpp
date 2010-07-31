/*
	threads.cpp
*/

#include "frame/threads.h"
#include <stdexcept>

namespace threads
{

template <> void
rikiGlue::DecodeThread::work( rikiGlue::ThreadBlock    &block )
{
	for ( register_t i = 0; i < block.numOps; ++i )
	{
		if ( block.ops[i](block) )
			throw ( std::runtime_error("operation error") );
	}
}

} /* namespace rikiGlue */
