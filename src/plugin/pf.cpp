/*
	pf.cpp
*/

#include <entry.h>
#include <AE_Effect.h>
#include <AE_EffectCB.h>
#include <AE_Macros.h>
#include <Param_Utils.h>

#include "plugin/pf.h"
#include "plugin/strings.h"
#include "frame/frame.h"
#include "frame/operations.h"

namespace rikiGlue
{

static PF_Err 
about( PF_InData      *in_data,	
       PF_OutData     *out_data,
       uint8_t        selector )
{	
	in_data->utils->ansi.sprintf( out_data->return_msg, "%s v%d.%d\r%s",
	                              STR(selector + kStrID_Name), MAJOR_VERSION, MINOR_VERSION, 
	                              STR(selector + kStrID_Description));

	return ( PF_Err_NONE );
}

static PF_Err 
globalSetup( PF_InData      *in_data,	
             PF_OutData     *out_data )
{
	out_data->my_version = PF_VERSION( MAJOR_VERSION, MINOR_VERSION, BUG_VERSION, STAGE_VERSION, BUILD_VERSION);

	out_data->out_flags = 	0;
	out_data->out_flags2 =  0;

	return ( PF_Err_NONE );
}

static PF_Err 
globalSetdown( PF_InData      *in_data,	
               PF_OutData     *out_data )
{
	return ( PF_Err_NONE );
}

static PF_Err 
paramSetup( PF_InData      *in_data,	
            PF_OutData     *out_data,
            uint8_t        selector )
{
	out_data->num_params = kNumParamIDs;
	return ( PF_Err_NONE );
}

static PF_Err 
sequenceSetdown( PF_InData      *in_data,	
                 PF_OutData     *out_data )
{
	if ( in_data->sequence_data )
	{
		PF_DISPOSE_HANDLE(in_data->sequence_data);
		out_data->sequence_data = NULL;
	}
	return ( PF_Err_NONE );
}

static PF_Err 
sequenceFlatten( PF_InData      *in_data,	
                 PF_OutData     *out_data )
{
	if ( in_data->sequence_data )
		PF_DISPOSE_HANDLE(in_data->sequence_data);

	return ( PF_Err_NONE );
}

static PF_Err 
sequenceReSetup( PF_InData      *in_data,
                 PF_OutData     *out_data )
{
	return ( PF_Err_NONE );
}

static PF_Err 
sequenceSetup( PF_InData      *in_data,	
               PF_OutData     *out_data )
{
	return ( sequenceReSetup(in_data, out_data) );
}

static PF_Err 
copyInput( PF_InData           *in_data,	
           PF_EffectWorld      *input,
           PF_EffectWorld      *output,
           PF_SmartRenderExtra *extra = NULL )
{
	if ( output )
	{
		if ( extra )
			AERR( extra->cb->checkout_layer_pixels(in_data->effect_ref, kInputParam, &input) );
		
		AERR( PF_COPY(input, output, NULL, NULL) );
	}
	return ( PF_Err_NONE );
}

static A_Err
rikiGlue( PF_Cmd        cmd,
          PF_InData     *in_data,
          PF_OutData    *out_data,
          PF_ParamDef   *params[],
          PF_LayerDef   *output,
          void          *extra,
          uint8_t       selector )
{
	PF_Err	err = PF_Err_NONE;
	
	try
	{
		switch (cmd) {

			case PF_Cmd_ABOUT:
				err = about(in_data, out_data, selector);
				break;

			case PF_Cmd_GLOBAL_SETUP:
				err = globalSetup(in_data, out_data);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = globalSetdown(in_data, out_data);
				break;

			case PF_Cmd_PARAMS_SETUP:
				err = paramSetup(in_data, out_data, selector);
				break;

			case PF_Cmd_SEQUENCE_SETUP:
				err = sequenceSetup(in_data, out_data);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = sequenceReSetup(in_data, out_data);
				break;
			case PF_Cmd_SEQUENCE_FLATTEN:
				err = sequenceFlatten(in_data, out_data);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = sequenceSetdown(in_data, out_data);
				break;

			case PF_Cmd_RENDER:
				{
					Frame::operation_t  ops[3] = { argbToRGB, NULL, rgbToARGB };
					Frame  frame(output->width, output->height);

					switch ( selector )
					{
						case kRSAEncrypt:
							ops[1] = rsaEncrypt;
							break;

						case kRSADecrypt:
							ops[1] = rsaDecrypt;
							break;

						case kEncryptLut:
							ops[1] = lutEncrypt;
							break;

						case kDecryptLut:
							ops[1] = lutDecrypt;
							break;

						case kCSS:
							ops[1] = css;
							break;

						case kAESEncrypt:
							ops[1] = aesEncrypt;
							break;

						case kAESDecrypt:
							ops[1] = aesDecrypt;
							break;

					}
					if ( ops[1] )
					{
						err = frame.operate(&ops[0], 1, (uint8_t*)params[kInputParam]->u.ld.data, params[kInputParam]->u.ld.rowbytes);
						if ( err == PF_Err_NONE )
						{
							err = frame.operate(&ops[1], 1, frame.bytes(), frame.rowBytes());
							if ( err == PF_Err_NONE )
								err = frame.operate(&ops[2], 1, frame.bytes(), frame.rowBytes(), (uint8_t*)output->data, output->rowbytes);
						}
					}
					else
						err = copyInput(in_data, &params[kInputParam]->u.ld, output, NULL);

				}
				break;
		}
	}
	catch(...)
	{
		in_data->utils->ansi.sprintf( out_data->return_msg, "%s v%d.%d\r%s",
		                              STR(selector + kStrID_Name), MAJOR_VERSION, MINOR_VERSION, 
		                              STR(kStrID_Exception) );
								
		err = PF_Err_BAD_CALLBACK_PARAM;
	}
	return err;
}

} /* namespace rikiGlue */

extern "C"
{

DllExport A_Err
rikiRSAEncrypt( PF_Cmd        cmd,
                PF_InData     *in_data,
                PF_OutData    *out_data,
                PF_ParamDef   *params[],
                PF_LayerDef   *output,
                void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kRSAEncrypt) );
}

DllExport A_Err
rikiRSADecrypt( PF_Cmd        cmd,
                PF_InData     *in_data,
                PF_OutData    *out_data,
                PF_ParamDef   *params[],
                PF_LayerDef   *output,
                void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kRSADecrypt) );
}

DllExport A_Err
rikiEncLut( PF_Cmd        cmd,
            PF_InData     *in_data,
            PF_OutData    *out_data,
            PF_ParamDef   *params[],
            PF_LayerDef   *output,
            void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kEncryptLut) );
}

DllExport A_Err
rikiDecLut( PF_Cmd        cmd,
           PF_InData     *in_data,
           PF_OutData    *out_data,
           PF_ParamDef   *params[],
           PF_LayerDef   *output,
           void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kDecryptLut) );
}

DllExport A_Err
rikiCSS( PF_Cmd        cmd,
           PF_InData     *in_data,
           PF_OutData    *out_data,
           PF_ParamDef   *params[],
           PF_LayerDef   *output,
           void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kCSS) );
}

DllExport A_Err
rikiAESEncrypt( PF_Cmd        cmd,
             PF_InData     *in_data,
             PF_OutData    *out_data,
             PF_ParamDef   *params[],
             PF_LayerDef   *output,
             void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kAESEncrypt) );
}

DllExport A_Err
rikiAESDecrypt( PF_Cmd        cmd,
             PF_InData     *in_data,
             PF_OutData    *out_data,
             PF_ParamDef   *params[],
             PF_LayerDef   *output,
             void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kAESDecrypt) );
}

}