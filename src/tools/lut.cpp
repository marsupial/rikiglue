#include <set>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <iomanip>

typedef std::vector<unsigned char>  lut_t;
typedef std::set<unsigned char>     set_t;

static int
usage( const char    *filePath )
{
	printf("usage: %s n [d]\n", filePath);
	return ( -1 );
}

static void
printLut( const char    *name,
          lut_t         &lut,
          register_t    dist = 0 )
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

		if ( dist == 0 )
			outLut[ val ] = i;
		else
		{
			for ( register_t d = 0; d < dist; ++d )
				outLut[val + d] = i;
		}
	}
	
	std::cout << std::endl << "};" << std::endl;
	std::cout.setf(f);

	lut.clear();
	lut = outLut;
}

extern int
main( int    argc,
      const char     **argv )
{
	if ( argc < 2 )
		return ( usage(argv[0]) );

	register_t n = atoi(argv[1]);
	lut_t      lut(n);
	set_t      set;

	srand(clock() * clock());;

	while ( set.size() < n )
	{
		unsigned char r = rand() % n;

//if ( r % 51 == 0 )
		if ( set.find(r) == set.end() )

		{
			set.insert(r);
			lut[r] = set.size()-1;

//eMap[r+1] = eMap.size()-1;
//eMap[r+2] = eMap.size()-1;
//eMap[r+3] = eMap.size()-1;
		}
		srand(clock() * clock() + set.size());
	}

	if ( argc > 2 )
	{
		lut_t      elut, dlut(atoi(argv[2]));
		const register_t dist = dlut.size() / n,
		                 hdist = dist / 2;
//std::cout << "distribute " << dist << std::endl;
		for ( register_t  i = 0; i < n; ++i )
		{
			const uint8_t dval = lut[i] * dist;
//std::cout << " dval: " << (int)dval << std::endl;
			const uint8_t val = dval + hdist;
			for ( register_t d = 0; d < dist; ++d )
			{
				elut.push_back(val);
				dlut[dval+d] = (i*dist) + hdist;
			}
		}

		printLut("kEncryptLut", elut);
		printLut("kDecryptLut", dlut);
	}

	printLut("kEncryptLut", lut);
	printLut("kDecryptLut", lut);

	return ( 0 );
}

