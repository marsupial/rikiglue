/*
	rot13.cpp
*/

#include "rikiFrame.h"

namespace rikiGlue
{

int
Frame::rot13()
{
	for ( register_t i = 0; i < numBlocks(); ++i )
	{
		Block &block = getBlock(i);
		const size_t blockSize = block.size();

		for ( size_t j = 0; j < blockSize; ++j )
		{
			uint8_t current = block[j];
			uint8_t offset = 0;
			if ( current >= 'a' && current <= 'z' )
				offset = 'a';
			else if (current >= 'A' && current <= 'Z')
				offset = 'A';
				
			if (offset != 0)
			{
				if ( current - offset < 13 )
					current = current + 13;
				else
					current = current - 13;
			}
			
			block[j] = current;
		}
		
		setBlock(i, block);
	}

	return ( 0 );
}
		
} /* namespace rikiGlue */