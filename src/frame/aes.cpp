/*
   aes.cpp
*/

#include <stdlib.h>
#include <string.h>
#include "frame/operations.h"

namespace aes
{

/* AES Implementation by X-N2O
 * Started:  15:41:35 - 18 Nov 2009
 * Finished: 20:03:59 - 21 Nov 2009
 * Logarithm, S-Box, and RCON tables are not hardcoded
 * Instead they are generated when the program starts
 * All of the code below is based from the AES specification
 * You can find it at http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 * This is only a proof of concept, and should not be considered as the most efficient implementation
 *
 * This work is licensed under the Creative Commons Attribution 3.0 Unported License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative Commons:
 * 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 */
 
#define AES_RPOL      0x011b // reduction polynomial (x^8 + x^4 + x^3 + x + 1)
#define AES_GEN       0x03   // gf(2^8) generator    (x^4 + 1)
#define AES_SBOX_CC   0x63   // S-Box C constant

#define KEY_128 (128/8)
#define KEY_192 (192/8)
#define KEY_256 (256/8)

#define aes_mul(a, b) ((a)&&(b)?sILogT[(sLogT[(a)]+sLogT[(b)])%0xff]:0)
#define aes_inv(a)    ((a)?sILogT[0xff-sLogT[(a)]]:0)

typedef struct
{
	uint8_t  state[4][4];
	int32_t  kcol;
	uint32_t rounds;
	uint32_t keysched[0];
} ctx_t;

static uint8_t sLogT[256], sILogT[256];
static uint8_t sSBox[256], sISBox[256];

static inline uint8_t
mulManual( uint8_t    a,
           uint8_t    b )
{
	register uint16_t ac;
	register uint8_t ret;

	ac = a;
	ret = 0;

	while ( b )
	{
		if ( b & 0x01 )
			ret ^= ac;

		ac <<= 1;
		b >>= 1;

		if ( ac & 0x0100 )
			ac ^= AES_RPOL;
	}

	return ret;
}

static inline uint32_t
subword( uint32_t    w )
{
	return ( sSBox[w & 0x000000ff] | (sSBox[(w & 0x0000ff00) >> 8] << 8) |
	        (sSBox[(w & 0x00ff0000) >> 16] << 16) | (sSBox[(w & 0xff000000) >> 24] << 24) );
}

static inline uint32_t
rotword( uint32_t    w )
{
	// May seem a bit different from the spec
	// It was changed because uint32_t is represented with little-endian convention on x86
	// Should not depend on architecture, but this is only a POC
	return ( ((w & 0x000000ff) << 24) | ((w & 0x0000ff00) >> 8) |
	         ((w & 0x00ff0000) >> 8) | ((w & 0xff000000) >> 8) );
}

static void
subBytes( ctx_t    *ctx )
{
	int32_t i;

	for ( i = 0; i < 16; i++ )
	{
		int32_t x, y;

		x = i & 0x03;
		y = i >> 2;
		ctx->state[x][y] = sSBox[ctx->state[x][y]];
	}
}

static void
shiftRows( ctx_t    *ctx )
{
	uint8_t nstate[4][4];
	int32_t i;

	for ( i = 0; i < 16; i++ )
	{
		int32_t x, y;

		x = i & 0x03;
		y = i >> 2;
		nstate[x][y] = ctx->state[x][(y+x) & 0x03];
	}

	memcpy(ctx->state, nstate, sizeof(ctx->state));
}

static void
mixColumns( ctx_t     *ctx )
{
	uint8_t nstate[4][4];
	int32_t i;
	
	for ( i = 0; i < 4; i++ )
	{
		nstate[0][i] = aes_mul(0x02, ctx->state[0][i]) ^ 
		               aes_mul(0x03, ctx->state[1][i]) ^
		               ctx->state[2][i] ^
		               ctx->state[3][i];
		nstate[1][i] = ctx->state[0][i] ^
		               aes_mul(0x02, ctx->state[1][i]) ^
		               aes_mul(0x03, ctx->state[2][i]) ^
		               ctx->state[3][i];
		nstate[2][i] = ctx->state[0][i] ^
		               ctx->state[1][i] ^
		               aes_mul(0x02, ctx->state[2][i]) ^
		               aes_mul(0x03, ctx->state[3][i]);
		nstate[3][i] = aes_mul(0x03, ctx->state[0][i]) ^
		               ctx->state[1][i] ^
		               ctx->state[2][i] ^
		               aes_mul(0x02, ctx->state[3][i]);
	}

	memcpy(ctx->state, nstate, sizeof(ctx->state));
}

static void
addRoundKey( ctx_t     *ctx,
             int32_t   round )
{
	int32_t i;

	for ( i = 0; i < 16; i++ )
	{
		int32_t x, y;

		x = i & 0x03;
		y = i >> 2;
		ctx->state[x][y] = ctx->state[x][y] ^ ((ctx->keysched[round*4+y] & (0xff << (x*8))) >> (x*8));
	}
}


static void
keyExpansion( ctx_t     *ctx )
{
	uint32_t temp;
	uint32_t rcon;
	register int32_t i;

	rcon = 0x00000001;
	for ( i = ctx->kcol; i < (4*(ctx->rounds+1)); i++ )
	{
		temp = ctx->keysched[i-1];
		if(!(i%ctx->kcol))
		{
			temp = subword(rotword(temp)) ^ rcon;
			rcon = aes_mul(rcon, 2);
		}
		else if(ctx->kcol > 6 && i%ctx->kcol == 4)
			temp = subword(temp);

		ctx->keysched[i] = ctx->keysched[i-ctx->kcol] ^ temp;
	}
}

static void
init()
{
	int32_t i;
	uint8_t gen;

	// build logarithm table and it's inverse
	gen = 1;
	for ( i = 0; i <= 0xff; i++ )
	{
		sLogT[gen]  = i;
		sILogT[i]   = gen;
		gen = mulManual(gen, AES_GEN);
	}

	// build S-Box and it's inverse
	for ( i = 0; i <= 0xff; i++ )
	{
		char bi;
		uint8_t inv = aes_inv(i);

		sSBox[i] = 0;
		for ( bi = 0; bi < 8; bi++ )
		{
			// based on transformation 5.1
			// could also be done with a loop based on the matrix
			sSBox[i] |= ((inv & (1<<bi)?1:0) ^
			                  (inv & (1 << ((bi+4) & 7))?1:0) ^
			                  (inv & (1 << ((bi+5) & 7))?1:0) ^
			                  (inv & (1 << ((bi+6) & 7))?1:0) ^
			                  (inv & (1 << ((bi+7) & 7))?1:0) ^
			                  (AES_SBOX_CC & (1 << bi)?1:0) ) << bi;
		}
		sISBox[sSBox[i]] = i;
	}
	// warning: quickhack
	sSBox[1] = 0x7c;
	sISBox[0x7c] = 1;
	sISBox[0x63] = 0;
}

static ctx_t*
allocContext( uint8_t    *key,
              uint32_t   keyLen )
{
	ctx_t *ctx;
	size_t rounds;
	size_t ks_size;

	switch (keyLen)
	{
		case 16: // 128-bit key
			rounds = 10;
			break;

		case 24: // 192-bit key
			rounds = 12;
			break;

		case 32: // 256-bit key
			rounds = 14;
			break;

		default:
			return ( NULL );
	}

	ks_size = 4*(rounds+1)*sizeof(uint32_t);
	ctx = (ctx_t*) malloc(sizeof(ctx_t)+ks_size);
	if(ctx)
	{
		ctx->rounds = rounds;
		ctx->kcol = keyLen/4;
		memcpy(ctx->keysched, key, keyLen);
		keyExpansion(ctx);
	}

	return ( ctx );
}

static void
invShiftRows( ctx_t    *ctx )
{
	uint8_t nstate[4][4];
	int32_t i;

	for ( i = 0; i < 16; i++ )
	{
		int32_t x, y;

		x = i & 0x03;
		y = i >> 2;
		nstate[x][(y+x) & 0x03] = ctx->state[x][y];
	}

	memcpy(ctx->state, nstate, sizeof(ctx->state));
}

static void
invSubBytes( ctx_t    *ctx )
{
	int32_t i;

	for ( i = 0; i < 16; i++ )
	{
		int32_t x, y;

		x = i & 0x03;
		y = i >> 2;
		ctx->state[x][y] = sISBox[ctx->state[x][y]];
	}
}

static void
invMixColumns( ctx_t    *ctx )
{
	uint8_t nstate[4][4];
	int32_t i;
	
	for ( i = 0; i < 4; i++ )
	{
		nstate[0][i] = aes_mul(0x0e, ctx->state[0][i]) ^ 
		               aes_mul(0x0b, ctx->state[1][i]) ^
		               aes_mul(0x0d, ctx->state[2][i]) ^
		               aes_mul(0x09, ctx->state[3][i]);
		nstate[1][i] = aes_mul(0x09, ctx->state[0][i]) ^
		               aes_mul(0x0e, ctx->state[1][i]) ^
		               aes_mul(0x0b, ctx->state[2][i]) ^
		               aes_mul(0x0d, ctx->state[3][i]);
		nstate[2][i] = aes_mul(0x0d, ctx->state[0][i]) ^
		               aes_mul(0x09, ctx->state[1][i]) ^
		               aes_mul(0x0e, ctx->state[2][i]) ^
		               aes_mul(0x0b, ctx->state[3][i]);
		nstate[3][i] = aes_mul(0x0b, ctx->state[0][i]) ^
		               aes_mul(0x0d, ctx->state[1][i]) ^
		               aes_mul(0x09, ctx->state[2][i]) ^
		               aes_mul(0x0e, ctx->state[3][i]);
	}

	memcpy(ctx->state, nstate, sizeof(ctx->state));
}

static void
encrypt( ctx_t         *ctx,
         const uint8_t input[16],
         uint8_t       output[16] )
{
	int32_t i;

	// copy input to state
	for ( i = 0; i < 16; i++ )
		ctx->state[i & 0x03][i >> 2] = input[i];

	addRoundKey(ctx, 0);

	for ( i = 1; i < ctx->rounds; i++ )
	{
		subBytes(ctx);
		shiftRows(ctx);
		mixColumns(ctx);
		addRoundKey(ctx, i);
	}

	subBytes(ctx);
	shiftRows(ctx);
	addRoundKey(ctx, ctx->rounds);

	// copy state to output
	for ( i = 0; i < 16; i++ )
		output[i] = ctx->state[i & 0x03][i >> 2];
}

static void
decrypt( ctx_t         *ctx,
         const uint8_t input[16],
         uint8_t       output[16] )
{
	int32_t i, j;

	// copy input to state
	for ( i = 0; i < 16; i++ )
		ctx->state[i & 0x03][i >> 2] = input[i];

	addRoundKey(ctx, ctx->rounds);
	for ( i = ctx->rounds-1; i >= 1; i-- )
	{
		invShiftRows(ctx);
		invSubBytes(ctx);
		addRoundKey(ctx, i);
		invMixColumns(ctx);
	}

	invShiftRows(ctx);
	invSubBytes(ctx);
	addRoundKey(ctx, 0);

	// copy state to output
	for ( i = 0; i < 16; i++ )
		output[i] = ctx->state[i & 0x03][i >> 2];
}

static void
freeContext( ctx_t    *ctx )
{
	free(ctx);
}

} /* namespace aes */

namespace rikiGlue
{

static bool sInited = false;

static register_t
aesOperation( const Block    &block,
              void (*op)(aes::ctx_t*, const uint8_t*, uint8_t*) )
{
	using namespace aes;

	if ( !sInited )
	{
		init();
		sInited = true;
	}

	uint8_t key[KEY_128] = "uber strong key";

	ctx_t *ctx = allocContext(key, sizeof(key));
	if ( !ctx )
		return ( -1 );

	uint8_t *dst = block.dstData;
	const register_t n = block.dstSize / 16;
	for ( register_t i = 0; i < n; ++i )
	{
		op(ctx, dst, dst);
		dst += 16;
	}
	freeContext(ctx);
	return ( 0 );
}

register_t
aesEncrypt( const Block    &block )
{
	return ( aesOperation(block, aes::encrypt) );
}

register_t
aesDecrypt( const Block    &block )
{
	return ( aesOperation(block, aes::decrypt) );
}

} /* namespace rikiGlue */


