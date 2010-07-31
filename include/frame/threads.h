/*
	threads.h
*/

#pragma once

#ifndef _fthreads_h__
#define _fthreads_h__

#include "threads/threads.h"
#include "frame/block.h"

struct DmtxDecode_struct;
struct DmtxRegion_struct;

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

struct DMTXDecode
{
	Frame             *frame;
	DmtxDecode_struct *dec;
	DmtxRegion_struct *reg;

	static void
	finished( DMTXDecode  &decode );
};

typedef threads::Worker<ThreadBlock> DecodeThread;

typedef threads::Worker<DMTXDecode>  DMTXInstrThread;

class DMTXReader
{
public:
	typedef long  timeout_t;

	DMTXReader( DMTXInstrThread  *instThread = NULL ) :
		mInstThread(instThread)
	{}
	~DMTXReader() {}

	bool
	scan( Frame   *frame );  // if returns true, takes ownership of frame

private:

	bool
	find( Frame                 *frame,
	      DmtxDecode_struct     *dec,
	      timeout_t             timeout );

	void
	process( Frame              *frame,
	         DmtxDecode_struct  *dec,
	         DmtxRegion_struct  *reg );

	DMTXInstrThread  *mInstThread;
};

class DMTXThread : public DMTXReader, public threads::Worker<Frame*>
{
public:

	typedef long  timeout_t;

	DMTXThread( DMTXInstrThread  *instThread = NULL ) :
		DMTXReader(instThread)
	{}
	virtual ~DMTXThread() {}

private:

};


} /* namespace rikiGlue */

#endif /* _fthreads_h__ */

