/*
	rsaEncrypt.cpp
*/

#include "frame/frame.h"

#include "rj/rijndael.h"

namespace rikiGlue
{

static const char *kKey =
	"MIICXQIBAAKBgQC+KyuRXOUxsPWt2J/NIMCc4jc8HtBYgoZsyafXjfJmAlhFOGbw"
	"+uLRVUoDsnaV62m5oL6NC1QJthyxgYNhad9xxkP2vot8j2LI+1unCiskGNglTd5f"
	"Q89MPuWvv2IphM40NXc6J79EdQz7X8zvjmI550i2Sq08BZYz4n1ejUuxDQIDAQAB"
	"AoGATESsGISVNzStk6eJj49PMJxZYrdI//Z5SzAlLII1cUwHctuIDaXRSftWz9zH"
	"SqE7qcMaKR8d+TcckuRKeMspXZojNgbvxrBBMgD7a8Z9aJIjTCRPukdM/wjRdueH"
	"uAXbfPmN7gnOBsl7ZxNYEHNA+2oA2HIcmd+RMR3RMKbDdtECQQD0wT9TeHdBZ6oi"
	"ADQSzSLXEfo4b45zTh8U5YTF33kSpMqLh2s9hPZKnQ4TmwDxofeEXeE3TQ5vf4ZA"
	"5T0DJIo/AkEAxufkPT2qQcmyy1tnGqcP8uwXm6Xq4zZGPGxmGtV7SlmxxFKcxvgb"
	"rfsLWOdxwVHSx0/yOisxoupMFl7+NEk5swJBAKRAb8wHgxishfttYYTgj6n/kYmA"
	"K/vriGZQpSrZjvnZB2hdx7UwXsyx23mCwz5lcKMvEW44UyC2VxWEAc7zHacCQBqB"
	"OhPDDNX5Z57bjjCqBGENftPZ1yPxzogstapruiMdMqgDjiqhUybq5gcxyCHHonu2"
	"toNz0AhyijMM83jKLSsCQQDpn+P55Nm4xCpxXgKEwQ6cPWoaI5WXl4mNPi2cw4Cl"
	"L2FWhu8NGXi8IZUX6fhPndTHTYf344eFVtErii7c4lCw";

int
Frame::rsaEncrypt()
{
	rijndael_ctx   ctx;
	rijndael_set_key(&ctx, (u_char*) kKey, 128, 1);

	for ( register_t i = 0; i < numBlocks(); ++i )
	{
		Block &block = getBlock(i);

		const register_t rsaLen = 16;
		const size_t     blockLen = block.size();

		for ( size_t j = 0; j+rsaLen <= blockLen; j += rsaLen )
			rijndael_encrypt(&ctx, &block[j], &block[j]);
		
		setBlock(i, block);
	}

	return ( 0 );
}

int
Frame::rsaDecrypt()
{
	rijndael_ctx   ctx;
	rijndael_set_key(&ctx, (u_char*) kKey, 128, 0);

	for ( register_t i = 0; i < numBlocks(); ++i )
	{
		Block &block = getBlock(i);

		const register_t rsaLen = 16;
		const size_t     blockLen = block.size();

		for ( size_t j = 0; j+rsaLen <= blockLen; j += rsaLen )
			rijndael_decrypt(&ctx, &block[j], &block[j]);
		
		setBlock(i, block);
	}

	return ( 0 );
}

} /* namespace rikiGlue */