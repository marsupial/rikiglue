/*
	colorShift.cpp
*/

#include "rikiFrame.h"
#include "rikiPF.h"

namespace rikiGlue
{

int 
Frame::colorShift( const Params     &params )
{
	const float steps = static_cast<float>( params[kColorShiftSlider] );
	const float step = 255.0 / steps;
	const float stepN = 255.0 / (steps-1.0);

	for ( register_t i = 0; i < numBlocks(); ++i )
	{
		Block &block = getBlock(i);
		const size_t blocklen = block.size();

		for ( size_t j = 0; j < blocklen; ++j )
		{
			switch ( unsigned char val = block[j] )
			{
				default:
				{
					float stepped = ceil( static_cast<float>(val) / step ) - 1.0;
					unsigned char iValue = stepped * stepN;
					block[j] = iValue;
				}
					break;

				case 0:
				case 255:
					break;
			}
		}
		
		setBlock(i, block);
	}

	return ( 0 );
}

} /* namespace rikiGlue */