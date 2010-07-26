
#include <stdio.h>
#include <stdlib.h>

extern "C" int
main( int    argc,
      const char    **argv )
{
	const int limit = ( argc > 1 ? atol(argv[1]) : 2335 );
	char val = 0;
	for ( int i = 0; i < limit; ++i )
	{
		if ( ++val == 0 )
			val = 1;
		fwrite(&val, sizeof(val), 1, stdout);	
	}
	val = 0;
	fwrite(&val, sizeof(val), 1, stdout);

	return ( 0 );
}

