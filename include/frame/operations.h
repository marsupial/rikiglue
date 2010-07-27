/*
	operations.h
*/

#pragma once

#ifndef _operations_h__
#define _operations_h__

#include "frame/block.h"

namespace rikiGlue
{

register_t
argbToRGB( const Block    &block );

register_t
bgraToRGB( const Block    &block );

register_t
rgbToARGB( const Block    &block );

register_t
rgbToBGRA( const Block    &block );

register_t 
rsaEncrypt( const Block    &block );

register_t 
rsaDecrypt( const Block    &block );

register_t
lutEncrypt( const Block    &block );

register_t
lutDecrypt( const Block    &block );

register_t
rChannel( const Block    &block );

register_t
gChannel( const Block    &block );

register_t
bChannel( const Block    &block );

register_t
splitter( const Block    &block );

register_t
css( const Block    &block );

register_t
aesEncrypt( const Block    &block );

register_t
aesDecrypt( const Block    &block );

} /* namespace rikiGlue */

#endif /* _operations_h__ */