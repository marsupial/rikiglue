/*
	rikiPF.h
*/

#pragma once

#ifndef _rikiPF_h__
#define _rikiPF_h__

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1
// #define PF_DEEP_COLOR_AWARE 1

#include "rikiFrame.h"

namespace rikiGlue
{

typedef Frame Zone;


/* Parameter defaults */

enum
{
	kInputParam              ,
	kNumParams
};

enum
{
	kInputParamID   ,
	kNumParamIDs    ,
};

enum
{
	kColorShiftSlider = kInputParam + 1
};

enum
{
	kColorShiftSliderID = kInputParamID + 1,
	kColorShiftNumParamIDs
};

} /* namespace rikiGlue */

#endif /* _rikiPF_h__ */