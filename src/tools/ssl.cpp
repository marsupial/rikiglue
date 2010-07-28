/*
    ssl.cpp
*/

#include <stdio.h>
#include <string.h>

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "common.cpp"

static const char rnd_seed[] = "Apartment 21 (Tomorrow Can Shut Up and Go Away)";

static int
usage( const char    *filePath )
{
	printf("usage: %s [d] <key file>\n", filePath);
	return ( -1 );
}

static int
passwordCB( char    *buf,
            int     size,
            int     rwflag,
            void    *userData )
{
	::printf("Enter pass phrase for \"%s\" then Enter, Ctrl-D\n", userData ? static_cast<const char*>(userData) : "");
	
	bytes_t pass;
	if ( fileToBytes(stdin, pass) != 0 )
		return ( 0 );

	size_t len = pass.size();

	if ( len <= 0 )
		return 0;
	else if ( len > size )
		len = size;
	else
		len -= 1;

	::memcpy(buf, &pass[0], len);
	return ( len );
}

static int
loadKey( const char    *file,
         bool          decrypt,
         RSA           **rsa )
{
	BIO *bio = BIO_new( BIO_s_file_internal() );
	if ( bio == NULL )
		return ( -1 );

	if ( BIO_read_filename(bio, file) <= 0 )
	{
		BIO_free(bio);
		return ( -1 );
	}

	*rsa = NULL; //RSA_new();

	if ( decrypt )
		PEM_read_bio_RSAPrivateKey(bio, rsa, passwordCB, NULL);
	else
		PEM_read_bio_RSA_PUBKEY(bio, rsa, passwordCB, NULL);

	ERR_print_errors_fp(stdout);

	BIO_free(bio);

	if( decrypt && RSA_check_key(*rsa) <= 0 )
	{
		RSA_free(*rsa);
		printf("The rsa key is not valid\n");
		return ( -1 );
	}

	return ( 0 );
}

static int
crypto( RSA            *rsa,
        bool           decrypt,
        const bytes_t  &bytes,
        bytes_t        &crypt )
{
fprintf(stderr, "bytes: %d\n", static_cast<int>(bytes.size()));
fprintf(stderr, "RSA_size: %d\n", RSA_size(rsa));
	typedef int (*cryptop_t) (int, const unsigned char*, unsigned char*, RSA*, int);

	cryptop_t    cryptOp = ( decrypt ? RSA_private_decrypt : RSA_public_encrypt );
	const size_t len = bytes.size(),
	             rsaSz = RSA_size(rsa),
	             blen = rsaSz - ( decrypt ? 0 : 11 ); 
	size_t       tNum = 0;
	bytes_t      buf( rsaSz );

	while ( tNum < len )
	{
		int num = ( decrypt ? blen : (std::min)(blen, len-tNum) );
fprintf(stderr, "request: %d bytes\n", num);
		num = cryptOp(num, &bytes[tNum], &buf[0], rsa, RSA_PKCS1_PADDING);
fprintf(stderr, "crypted: %d bytes\n", num);
		if ( num < 0 )
			return ( -1 );
		else if ( num == 0 )
			return ( 0 );
		
		const size_t oldSZ = crypt.size(),
		             addSZ = ( decrypt ? num : rsaSz );
		crypt.resize(oldSZ + addSZ);
		::memcpy(&crypt[oldSZ], &buf[0], addSZ);
		tNum += blen; //( decrypt ? rsaSz : blen );
	}
fprintf(stderr, "Crypted: %d bytes, crypt size: %d\n", static_cast<int>(tNum), static_cast<int>(crypt.size()));
	return ( tNum == 0 );
}

static int
crypto( const char     *key,
        bool           decrypt,
        const bytes_t  &bytes,
        bytes_t        &crypt )
{
    CRYPTO_malloc_debug_init();
    CRYPTO_dbg_set_options(V_CRYPTO_MDEBUG_ALL);
    CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
    RAND_seed(rnd_seed, sizeof(rnd_seed)); /* or OAEP may fail */

	RSA *rsa = NULL;

	int rval = loadKey(key, decrypt, &rsa);
	if ( rval == 0 )
		rval = crypto(rsa, decrypt, bytes, crypt);

	RSA_free(rsa);
	ERR_print_errors_fp(stdout);

    CRYPTO_cleanup_all_ex_data();
	EVP_cleanup();
	ERR_remove_state(0);
    CRYPTO_mem_leaks_fp(stderr);

	return ( rval );
}

extern "C" int
main( int           argc,
      const char    **argv )
{
	if ( argc < 2 )
		return ( usage(argv[0]) );

	int   rval = 0;
	bool  decrypt = testFlag(argc, argv, 'd');

	bytes_t bytes;
	rval = fileToBytes(stdin, bytes);
	if ( rval != 0 )
		return ( rval );

	bytes_t crypt;
	rval = crypto(argv[argc-1], decrypt, bytes, crypt);
fprintf(stderr, "crypto returned: %d\n", rval);
	if ( rval == 0 )
		fwrite(&crypt[0], sizeof(bytes_t::value_type), crypt.size(), stdout);

	return ( 0 );
}