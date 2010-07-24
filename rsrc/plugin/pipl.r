
#include <AEConfig.h>
#include <AE_EffectVers.h>

// #define AE_PROC_INTELx64

#ifndef AE_OS_WIN
	#include <AE_General.r>
#endif

#define RNAME  "Encrypt"
#define RENTRY "rikiEncrypt"
#define RPIPL  1600
#include "pipl.i.r"

#define RNAME  "Decrypt"
#define RENTRY "rikiDecrypt"
#define RPIPL  1610
#include "pipl.i.r"


#define RNAME  "EC Encode"
#define RENTRY "rikiEncode"
#define RPIPL  1700
#include "pipl.i.r"

#define RNAME  "EC Decode"
#define RENTRY "rikiDecode"
#define RPIPL  1710
#include "pipl.i.r"



#define RNAME  "LUT Encode"
#define RENTRY "rikiEncLut"
#define RPIPL  1800
#include "pipl.i.r"

#define RNAME  "LUT Decode"
#define RENTRY "rikiDecLut"
#define RPIPL  1810
#include "pipl.i.r"


#define RNAME  "REC 709"
#define RENTRY "riki709"
#define RPIPL  1900
#include "pipl.i.r"