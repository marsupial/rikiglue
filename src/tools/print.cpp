/*
  print.cpp
*/

#include <vector>
#include <iostream>
#include <iomanip>

typedef std::vector<unsigned char>  lut_t;

static int
usage( const char    *filePath )
{
	printf("usage: %s [d]\n", filePath);
	return ( -1 );
}

static void
printLut( const char    *name,
          lut_t         &lut )
{
	std::cout << "static const uint8_t " << name << "[" << lut.size() << "] =" << std::endl << "{";

	const size_t   n = lut.size();
	lut_t          outLut(n);

	const std::ios::fmtflags f = std::cout.flags();
	std::cout.setf(std::ios::uppercase);
	std::cout.setf(std::ios::hex, std::ios::basefield);
	std::cout.fill('0');

	for ( register_t i = 0; i < n; ++i )
	{
		if ( i != 0 ) std::cout << ",";
		if ( i % 10 == 0 ) std::cout << std::endl << "\t";

		const register_t val = lut[i];
		std::cout << " 0x" << std::setw(2) << val;

		outLut[ val ] = i;
	}
	
	std::cout << std::endl << "};" << std::endl;
	std::cout.setf(f);

	lut.swap(outLut);
}

extern "C" int
main( int         argc,
      const char  **argv )
{
	bool decrypt = false;
	for ( int i = 0; i < argc; ++i )
	{
		if ( argv[i][0] == 'd' )
			decrypt = (argv[i][1] == 0);
	}

	lut_t    lut;
	uint8_t  val;
	while ( ::fread(&val, sizeof(val), 1, stdin) == sizeof(val) )
		lut.push_back(val);

	if ( feof(stdin) == 0 )
		return ( errno );

	printLut("kEncryptLut", lut);
	if ( decrypt )
		printLut("kDecryptLut", lut);
		
	return ( 0 );
}

