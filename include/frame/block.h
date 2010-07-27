/*
	block.h
*/

#pragma once

#ifndef _block_h__
#define _block_h__

#include "glue.pch"

namespace rikiGlue
{

struct Block
{
	typedef register_t (*operation_t) ( const Block     &block );

	Block( const uint8_t   *src,
		   size_t          srcSz,
		   uint8_t         *dst,
		   size_t          dstSz ) :
		srcData(src),
		srcSize(srcSz),
		dstData(dst),
		dstSize(dstSz)
	{}

	const uint8_t   *srcData;
	size_t          srcSize;

	uint8_t         *dstData;
	size_t          dstSize;
};

} /* namespace rikiGlue */

#endif /* _block_h__ */