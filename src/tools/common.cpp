/*
  common.cpp
*/

#include <vector>
#include <errno.h>

typedef std::vector<unsigned char>  bytes_t;

static bool
testFlag( int           argc,
          const char    **argv,
          const char    f )
{
	bool hasFlag = false;
	for ( int i = 0; i < argc; ++i )
	{
		if ( argv[i][0] == f && argv[i][1] == 0 )
			return ( true );
	}

	return ( false );
}

static int
fileToBytes( FILE    *file,
             bytes_t &bytes )
{
	const size_t kBufferSize = 256;
	size_t       nRead = kBufferSize,
	             tRead = bytes.size();

	while ( nRead == kBufferSize)
	{
		bytes.resize(tRead + kBufferSize);
		nRead = ::fread(&bytes[tRead], sizeof(bytes_t::value_type), kBufferSize, file);
		tRead += nRead;
	}

	bytes.resize(tRead);

	if ( feof(file) == 0 )
		return ( errno ? errno : -1 );

	return ( 0 );
}


