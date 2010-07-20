/*
	errorCorrect.cpp
*/

#include "rikiFrame.h"

#define FIXED 1
#define CCSDS 1
#include "ec/fixed.h"
#include "ec/ccsds.h"

#include "ec/rs/encode_rs.c"
#include "ec/rs/decode_rs.c"

void encode_rs_ccsds(unsigned char *data,unsigned char *parity);
int decode_rs_ccsds(unsigned char *data,int *eras_pos,int no_eras);

/*
#undef ENCODE_RS
#define ENCODE_RS encode_rs_ccsds

#undef DECODE_RS
#define DECODE_RS decode_rs_ccsds
*/

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