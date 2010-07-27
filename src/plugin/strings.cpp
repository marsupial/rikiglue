/*
	strings.cpp
*/

#include "strings.h"
#include <assert.h>

static const char*
     sStrings[kStrID_NUMTYPES] =
{
	"RSA Encryption",
	"Riki RSA Encryption",
	"Encrypt a frame with rsa",

	"RSA Decryption",
	"Riki RSA Decryption",
	"Decrypt a frame with rsa",

	"LUT Transform",
	"Riki Encode LUT",
	"Encode a frame with a lut",

	"LUT Detransform",
	"Riki Decode LUT",
	"Decode a frame with a lut",

	"CSS transform",
	"Riki CSS transform",
	"Decode a frame with css",

	"AES Encryption",
	"Riki AES encryption",
	"Encrypt a frame with aes",

	"AES Decryption",
	"Riki AES decryption",
	"Decrypt a frame with aes",

	"Exception caught"
};

const char*
getStringPtr( uint32_t     i )
{
	assert( i < kStrID_NUMTYPES );
	return ( sStrings[i] );
}