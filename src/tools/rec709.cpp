/*
	rec709.cpp
*/

#include <stdio.h>
#include <math.h>

extern "C" int
main( int           argc,
      const char    *argv )
{
	#define CLIP(b, t, v)   ( (v) <= (b) ? (b) : (v) >= (t) ? (t) : (v) )
	#define REC_709(L)      ( (L) <= 0.018 ? (L) * 4.5 : (1.099 * pow((L), 0.45) - 0.099) )

	int rec709[256];

	printf("unsigned char kRec709[256] = \n{");
	for ( int i = 0; i < 256; ++i )
	{
		rec709[i] = CLIP( 0, 255, static_cast<int>(0.5 + 255.0 * REC_709(static_cast<double>(i)/255.0)) );
		if ( i != 0 ) printf(",");
		if ( i % 10 == 0 ) printf("\n\t");
		
		printf(" 0x%02X", rec709[i]);
	}
	printf("\n};\n");

	return ( 0 );
}