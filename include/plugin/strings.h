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

	kRSAEncrypt, 
	kStrID_RSAEncryptName,
	kStrID_RSAEncryptDescription,

	kRSADecrypt, 
	kStrID_RSADecryptName,
	kStrID_RSADecryptDescription,

	kEncryptLut,
	kStrID_ELutName,
	kStrID_ELutDescription,

	kDecryptLut,
	kStrID_DLutName,
	kStrID_DLutDescription,

	kCSS,
	kStrID_CSSName,
	kStrID_CSSDescription,

	kAESEncrypt, 
	kStrID_AESEncryptName,
	kStrID_AESEncryptDescription,

	kAESDecrypt, 
	kStrID_AESDecryptName,
	kStrID_AESDecryptDescription,

	kStrID_Exception,
	kStrID_NUMTYPES

} StrIDType;

const char*
getStringPtr( uint32_t     strNum );

#define STR getStringPtr

#endif /* _strings_h__ */