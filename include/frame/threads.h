/*
	threads.h
*/

#pragma once

#ifndef _fthreads_h__
#define _fthreads_h__

#include "threads/threads.h"
#include "frame/block.h"

namespace rikiGlue
{

class Frame;

struct ThreadBlock : public Block
{
	ThreadBlock( const uint8_t   *src,
	             size_t          srcSz,
	             uint8_t         *dst,
	             size_t          dstSz,
	             const Block::operation_t *opers,
	             size_t                   nOpers ) :
		Block(src, srcSz, dst, dstSz),
		ops(opers),
		numOps(nOpers)
	{}
	
	const Block::operation_t *ops;
	size_t                   numOps;
};

typedef threads::Worker<ThreadBlock>  DecodeThread;
typedef threads::Worker<Frame*> DMTXThread;

} /* namespace rikiGlue */

#endif /* _fthreads_h__ */

