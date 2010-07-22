/*
	strings.h
*/

#pragma once

#ifndef _strings_h__
#define _strings_h__

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

	kEncryptLut,
	kStrID_ELutName,
	kStrID_ELutDescription,

	kDecryptLut,
	kStrID_DLutName,
	kStrID_DLutDescription,

	kRec709,
	kStrID_Rec709Name,
	kStrID_Rec709Description,

	kStrID_Exception,
	kStrID_NUMTYPES

} StrIDType;

const char*
getStringPtr( uint32_t     strNum );

#define STR getStringPtr

#endif /* _strings_h__ */