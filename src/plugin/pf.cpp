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

namespace rikiGlue
{

class PFParams : public Params
{
public:

	PFParams( const ::PF_ParamDef * const params[] ) :
		mParams(params)
	{}
	virtual ~PFParams() {}

	virtual int operator [] ( int    i ) const
	{
		return ( mParams[i]->u.sd.value );
	}

private:
	const ::PF_ParamDef * const *mParams;
};

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
					err = copyInput(in_data, &params[kInputParam]->u.ld, output, NULL);
					if ( err == PF_Err_NONE )
					{
						Frame  frame(output->data, output->rowbytes, output->width, output->height);

						switch ( selector )
						{
							case kEncode:
								err = frame.operate(&ecEncode);
								break;

							case kDecode:
								err = frame.operate(&ecDecode);
								break;

							case kEncrypt:
								err = frame.operate(&rsaEncrypt);
								break;

							case kDecrypt:
								err = frame.operate(&rsaDecrypt);
								break;

							case kEncryptLut:
								err = frame.operate(&lutEncrypt);
								break;

							case kDecryptLut:
								err = frame.operate(&lutDecrypt);
								break;

							case kRec709:
								err = frame.operate(&rec709);
								break;
						}
					}
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
rikiEncrypt( PF_Cmd        cmd,
             PF_InData     *in_data,
             PF_OutData    *out_data,
             PF_ParamDef   *params[],
             PF_LayerDef   *output,
             void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kEncrypt) );
}

DllExport A_Err
rikiDecrypt( PF_Cmd        cmd,
             PF_InData     *in_data,
             PF_OutData    *out_data,
             PF_ParamDef   *params[],
             PF_LayerDef   *output,
             void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kDecrypt) );
}

DllExport A_Err
rikiEncode( PF_Cmd        cmd,
            PF_InData     *in_data,
            PF_OutData    *out_data,
            PF_ParamDef   *params[],
            PF_LayerDef   *output,
            void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kEncode) );
}

DllExport A_Err
rikiDecode( PF_Cmd        cmd,
            PF_InData     *in_data,
            PF_OutData    *out_data,
            PF_ParamDef   *params[],
            PF_LayerDef   *output,
            void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kDecode) );
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
riki709( PF_Cmd        cmd,
           PF_InData     *in_data,
           PF_OutData    *out_data,
           PF_ParamDef   *params[],
           PF_LayerDef   *output,
           void          *extra )
{	
	return ( rikiGlue::rikiGlue(cmd, in_data, out_data, params, output, extra, kRec709) );
}

}