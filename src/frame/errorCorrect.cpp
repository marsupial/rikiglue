/*
	errorCorrect.cpp
*/

#include "frame/frame.h"

#include "ec/fixed.h"
#include "ec/ccsds.h"

namespace rikiGlue
{

int 
Frame::ecEncode()
{
	const register_t blockSize = NN;
	const register_t dataLen = ( NN - NROOTS );

	for ( register_t i = 0; i < numBlocks(); ++i )
	{
		Block &block = getBlock(i);

		const register_t completeBlocks = block.size() / blockSize;
		const register_t truncBlock = 0; // block.size() - (blockSize*completeBlocks);

		uint8_t *data = &block[0];
		for ( register_t j = 0; j < completeBlocks; ++j )
		{
			ENCODE_RS(data, data+dataLen);
			data += blockSize;
		}
		if ( truncBlock )
			ENCODE_RS(data, data + truncBlock);

		setBlock(i, block);
	}

	return ( 0 );
}

int 
Frame::ecDecode( )
{
	const register_t blockSize = NN;

	int derrors = 0;
	int derrlocs[NROOTS];
	int erasures = 0;

	for ( register_t i = 0; i < numBlocks(); ++i )
	{
		Block &block = getBlock(i);

		const register_t completeBlocks = block.size() / blockSize;
		const register_t truncBlock = 0; // block.size() - (blockSize*completeBlocks);

		uint8_t *data = &block[0];
		for ( register_t j = 0; j < completeBlocks; ++j )
		{
			derrors = DECODE_RS(data, derrlocs, erasures);
			data += blockSize;
			if ( derrors )
				printf("found %d errors\n", derrors);			
		}
		if ( truncBlock )
		{
			derrors = DECODE_RS(data, derrlocs, erasures);
			if ( derrors )
				printf("found %d errors\n", derrors);
		}

		setBlock(i, block);
	}

	return ( 0 );
}


} /* namespace rikiGlue */