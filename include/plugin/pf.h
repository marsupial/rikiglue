/*
	pf.h
*/

#pragma once

#ifndef _pf_h__
#define _pf_h__

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1

#define AERR(x)         do { A_Err err = (x); if ( err != ::A_Err_NONE ) return (err); } while(0)

namespace rikiGlue
{

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

} /* namespace rikiGlue */

#endif /* _pf_h__ */