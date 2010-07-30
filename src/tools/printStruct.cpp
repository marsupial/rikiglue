/*
  printStruct.cpp
*/

#include <vector>
#include <iostream>
#include <iomanip>

#include "common.cpp"

static void
printStruct( const char    *name,
             bytes_t       &bytes )
{
	std::cout << "static const uint8_t " << name << "[" << bytes.size() << "] =" << std::endl << "{";

	const size_t   n = bytes.size();

//	bytes_t        inverse(n);
//	if ( xform ) inverse.resize(n);

	const std::ios::fmtflags f = std::cout.flags();
	std::cout.setf(std::ios::uppercase);
	std::cout.setf(std::ios::hex, std::ios::basefield);
	std::cout.fill('0');

	for ( register_t i = 0; i < n; ++i )
	{
		if ( i != 0 ) std::cout << ",";
		if ( i % 10 == 0 ) std::cout << std::endl << "\t";

		const register_t val = bytes[i];
		std::cout << " 0x" << std::setw(2) << val;

		//if ( xform ) inverse[ val ] = i;
	}
	
	std::cout << std::endl << "};" << std::endl;
	std::cout.setf(f);

	//if ( xform ) bytes.swap(inverse);
}

extern "C" int
main( int         argc,
      const char  **argv )
{
	bytes_t bytes;
	if ( fileToBytes(stdin, bytes) != 0 )
		return ( -1 );

	printStruct( argc > 1 ? argv[1] : "kEncrypted", bytes);
	return ( 0 );
}

