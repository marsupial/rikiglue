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

	kEncryptLut,
	kStrID_ELutName,
	kStrID_ELutDescription,

	kDecryptLut,
	kStrID_DLutName,
	kStrID_DLutDescription,

	kStrID_Exception,
	kStrID_NUMTYPES

} StrIDType;

const char*
getStringPtr( uint32_t     strNum );

#define STR getStringPtr

#endif /* _strings_h__ */