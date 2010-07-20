
#if defined(AE_PROC_INTELx64)

	resource 'PiPL' (RPIPL) {
		{	/* array properties: 12 elements */

			/* [1] */
			Kind {
				AEEffect
			},

			/* [2] */
			Name {
				RNAME
			},

			/* [3] */
			Category {
				"RikiGlue"
			},

	#ifdef AE_OS_WIN
			CodeWin64X86     { RENTRY },
	#else
			CodeMacIntel64   { RENTRY },
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
				"BRSN " RNAME
			},

			/* [12] */
			AE_Reserved_Info {
				8
			}
		}
	};

#else

	resource 'PiPL' (RPIPL+1) {
		{	/* array properties: 12 elements */

			/* [1] */
			Kind {
				AEEffect
			},

			/* [2] */
			Name {
				RNAME
			},

			/* [3] */
			Category {
				"RikiGlue"
			},

	#ifdef AE_OS_WIN
			CodeWin32X86     { RENTRY },
	#else
			CodeMachOPowerPC { RENTRY },
			CodeMacIntel32   { RENTRY },
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
				"BRSN " RNAME
			},

			/* [12] */
			AE_Reserved_Info {
				8
			}
		}
	};

#endif

#undef RNAME
#undef RENTRY
#undef RPIPL