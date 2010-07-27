
#include <AEConfig.h>
#include <AE_EffectVers.h>

// #define AE_PROC_INTELx64

#ifndef AE_OS_WIN
	#include <AE_General.r>
#endif

#define RNAME  "RSA Encrypt"
#define RENTRY "rikiRSAEncrypt"
#define RPIPL  1600
#include "pipl.i.r"

#define RNAME  "RSA Decrypt"
#define RENTRY "rikiRSADecrypt"
#define RPIPL  1610
#include "pipl.i.r"


#define RNAME  "LUT Encode"
#define RENTRY "rikiEncLut"
#define RPIPL  1700
#include "pipl.i.r"

#define RNAME  "LUT Decode"
#define RENTRY "rikiDecLut"
#define RPIPL  1710
#include "pipl.i.r"


#define RNAME  "CSS"
#define RENTRY "rikiCSS"
#define RPIPL  1800
#include "pipl.i.r"


#define RNAME  "AES Encrypt"
#define RENTRY "rikiAESEncrypt"
#define RPIPL  1900
#include "pipl.i.r"

#define RNAME  "AES Decrypt"
#define RENTRY "rikiAESDecrypt"
#define RPIPL  1910
#include "pipl.i.r"