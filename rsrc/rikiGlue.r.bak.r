
#include "AEConfig.h"
#include "AE_EffectVers.h"
	
#ifndef AE_OS_WIN
	#include <AE_General.r>
#endif

#if defined(AE_PROC_INTELx64)

	resource 'PiPL' (16000) {
		{	/* array properties: 12 elements */

			/* [1] */
			Kind {
				AEEffect
			},

			/* [2] */
			Name {
				"RikiGlue"
			},

			/* [3] */
			Category {
				"RikiGlue"
			},

	#ifdef AE_OS_WIN
			CodeWin64X86     {"fbxRender"},
	#else
			CodeMacIntel64   {"fbxRender"},
	#endif
			/* [6] */
			AE_PiPL_Version {
				2,
				0
			},

			/* [7] */
			AE_Effect_Spec_Version {
				PF_PLUG_IN_VERSION,
				PF_PLUG_IN_SUBVERS
			},

			/* [8] */
			AE_Effect_Version {
				0x00080001	/* 1.0 */
			},

			/* [9] */
			AE_Effect_Info_Flags {
				0
			},

			/* [10] */
			AE_Effect_Global_OutFlags {
				0
			},

			AE_Effect_Global_OutFlags_2 {
				0
			},

			/* [11] */
			AE_Effect_Match_Name {
				"BRSN RikiGlue"
			},

			/* [12] */
			AE_Reserved_Info {
				8
			}
		}
	};

#else

	resource 'PiPL' (16001) {
		{	/* array properties: 12 elements */

			/* [1] */
			Kind {
				AEEffect
			},

			/* [2] */
			Name {
				"RikiGlue"
			},

			/* [3] */
			Category {
				"RikiGlue"
			},

	#ifdef AE_OS_WIN
			CodeWin32X86     {"fbxRender"},
	#else
			CodeMachOPowerPC {"fbxRender"},
			CodeMacIntel32   {"fbxRender"},
	#endif
			/* [6] */
			AE_PiPL_Version {
				2,
				0
			},

			/* [7] */
			AE_Effect_Spec_Version {
				PF_PLUG_IN_VERSION,
				PF_PLUG_IN_SUBVERS
			},

			/* [8] */
			AE_Effect_Version {
				0x00080001	/* 1.0 */
			},

			/* [9] */
			AE_Effect_Info_Flags {
				0
			},

			/* [10] */
			AE_Effect_Global_OutFlags {
				0
			},

			AE_Effect_Global_OutFlags_2 {
				0
			},

			/* [11] */
			AE_Effect_Match_Name {
				"BRSN RikiGlue"
			},

			/* [12] */
			AE_Reserved_Info {
				8
			}
		}
	};

#endif
