/*
  printStruct.cpp
*/

#include <vector>
#include <iostream>
#include <iomanip>

#include "common.cpp"

static void
printStruct( const char    *name,
             register_t    n,
             bytes_t       &bytes )
{
	const size_t   sz = bytes.size()/n;
	std::cout << "static const uint8_t " << name << "[" << n << "][" << sz << "] =" << std::endl;
	std::cout << "{";

	const std::ios::fmtflags f = std::cout.flags();
	std::cout.setf(std::ios::uppercase);
	std::cout.setf(std::ios::hex, std::ios::basefield);
	std::cout.fill('0');


	for ( register_t i = 0; i < n; ++i )
	{
		std::cout << std::endl << "{";
	//	bytes_t        inverse(n);
	//	if ( xform ) inverse.resize(n);

		for ( register_t j = 0; j < sz; ++j )
		{
			if ( j != 0 ) std::cout << ",";
			if ( j % 10 == 0 ) std::cout << std::endl << "\t";

			const register_t val = bytes[(i*sz)+j];
			std::cout << " 0x" << std::setw(2) << val;

			//if ( xform ) inverse[ val ] = (i*sz)+j;
		}
		std::cout << std::endl << "},";
	}
	
	std::cout << std::endl << "};" << std::endl;
	std::cout.setf(f);

	//if ( xform ) bytes.swap(inverse);
}

static int
usage( const char    *filePath )
{
	printf("usage: %s [name] [n]\n", filePath);
	return ( -1 );
}

extern "C" int
main( int         argc,
      const char  **argv )
{
	register_t n = 1;
	if ( argc > 1 )
	{
		n = atol(argv[1]);
		if ( n == 0 )
			return ( usage(argv[0]) );
	}
	
	bytes_t bytes;
	if ( fileToBytes(stdin, bytes) != 0 )
		return ( -1 );

	printStruct( argc > 2 ? argv[2] : "kEncrypted", n, bytes);
	return ( 0 );
}

