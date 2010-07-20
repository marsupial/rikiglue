/*
	errorCorrect.cpp
*/

#include "frame/frame.h"

#include "ec/fixed.h"
#include "ec/ccsds.h"

namespace rikiGlue
{

register_t 
ecEncode( Frame::Block     &block )
{
	const register_t blockSize = NN;
	const register_t dataLen = ( NN - NROOTS );
	const register_t completeBuffers = block.size() / blockSize;
	const register_t truncBuffers = block.size() % blockSize;

	uint8_t *data = &block[0];
	for ( register_t i = 0; i < completeBuffers; ++i )
	{
		ENCODE_RS(data, data+dataLen);
		data += blockSize;
	}

	if ( truncBuffers > NROOTS )
		ENCODE_RS(data, data + (truncBuffers-NROOTS));

	return ( 0 );
}

register_t 
ecDecode( Frame::Block     &block )
{
	const register_t blockSize = NN;
	const register_t completeBuffers = block.size() / blockSize;
	const register_t truncBuffers = block.size() % blockSize;

	int derrors = 0;
	int derrlocs[NROOTS];
	int erasures = 0;

	uint8_t *data = &block[0];
	for ( register_t i = 0; i < completeBuffers; ++i )
	{
		derrors = DECODE_RS(data, derrlocs, erasures);
		data += blockSize;
		if ( derrors )
			printf("found %d errors\n", derrors);			
	}

	if ( truncBuffers > NROOTS )
	{
		derrors = DECODE_RS(data, derrlocs, erasures);
		if ( derrors )
			printf("found %d errors\n", derrors);
	}

	return ( 0 );
}

} /* namespace rikiGlue */
