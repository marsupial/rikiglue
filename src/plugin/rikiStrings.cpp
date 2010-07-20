/*
	rikiStrings.cpp
*/

#include "rikiStrings.h"
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

	"Error Correction Encode",
	"Riki Encode Error Correction",
	"Encode a frame with error correction",

	"Error Correction Decode",
	"Riki Decode Error Correction",
	"Decode a frame with error correction",

	"Color Shift",
	"Riki Color Shift Correction",
	"Shift colors into encoded space",
	"n",

	"LUT Transform",
	"Riki Encode LUT",
	"Encode a frame with a lut",

	"LUT Detransform",
	"Riki Decode LUT",
	"Decode a frame with a lut",

	"Rot13",
	"Riki Rot13",
	"Encrypt a frame in rot13",

	"Exception caught"
};

const char*
getStringPtr( uint32_t     i )
{
	assert( i < kStrID_NUMTYPES );
	return ( sStrings[i] );
}