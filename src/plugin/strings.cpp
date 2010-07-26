/*
	strings.cpp
*/

#include "strings.h"
#include <assert.h>

static const char*
     sStrings[kStrID_NUMTYPES] =
{
	"Encryption",
	"Riki Encryption",
	"Encrypt a frame",

	"Decryption",
	"Riki Decryption",
	"Decrypt a frame",

	"LUT Transform",
	"Riki Encode LUT",
	"Encode a frame with a lut",

	"LUT Detransform",
	"Riki Decode LUT",
	"Decode a frame with a lut",

	"Exception caught"
};

const char*
getStringPtr( uint32_t     i )
{
	assert( i < kStrID_NUMTYPES );
	return ( sStrings[i] );
}