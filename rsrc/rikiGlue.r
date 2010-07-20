
#include "AEConfig.h"
#include "AE_EffectVers.h"
	
#ifndef AE_OS_WIN
	#include <AE_General.r>
#endif

#define RNAME  "Encrypt"
#define RENTRY "rikiEncrypt"
#define RPIPL  1600
#include "rikiGlue.include.r"

#define RNAME  "Decrypt"
#define RENTRY "rikiDecrypt"
#define RPIPL  1610
#include "rikiGlue.include.r"


#define RNAME  "EC Encode"
#define RENTRY "rikiEncode"
#define RPIPL  1620
#include "rikiGlue.include.r"

#define RNAME  "EC Decode"
#define RENTRY "rikiDecode"
#define RPIPL  1630
#include "rikiGlue.include.r"


#define RNAME  "Color Shift"
#define RENTRY "rikiColorShift"
#define RPIPL  1640
#include "rikiGlue.include.r"

#define RNAME  "Lut Encode"
#define RENTRY "rikiEncLut"
#define RPIPL  1650
#include "rikiGlue.include.r"

#define RNAME  "LUT Decode"
#define RENTRY "rikiDecLut"
#define RPIPL  1660
#include "rikiGlue.include.r"

#define RNAME  "Rot13"
#define RENTRY "rikiRot13"
#define RPIPL  1670
#include "rikiGlue.include.r"