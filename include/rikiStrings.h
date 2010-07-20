/*
	rikiStrings.h
*/

#pragma once

#ifndef _rikiStrings_h__
#define _rikiStrings_h__

#include "platform/types.h"

enum
{
	kStrID_Name         = 1,
	kStrID_Description  = 2,
};

typedef enum
{

	kEncrypt, 
	kStrID_EncryptName,
	kStrID_EncryptDescription,

	kDecrypt, 
	kStrID_DecryptName,
	kStrID_DecryptDescription,

	kEncode,
	kStrID_EncodeName,
	kStrID_EncodeDescription,

	kDecode,
	kStrID_DecodeName,
	kStrID_DecodeDescription,

	kColorShift,
	kStrID_ColorShiftName,
	kStrID_ColorShiftDescription,
	kStrID_ColorLevel,

	kEncryptLut,
	kStrID_ELutName,
	kStrID_ELutDescription,

	kDecryptLut,
	kStrID_DLutName,
	kStrID_DLutDescription,

	kRot13,
	kStrID_Rot13Name,
	kStrID_Rot13Description,

	kStrID_Exception,
	kStrID_NUMTYPES

} StrIDType;

const char*
getStringPtr( uint32_t     strNum );

#define STR getStringPtr

#endif /* _rikiStrings_h__ */