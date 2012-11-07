#ifndef _TAGGL_TAGFUNCTIONS_H_
#define _TAGGL_TAGFUNCTIONS_H_
#pragma once

namespace TAGGL
{

enum
{
	kTAGFunctionHeapAlloc			= 0x8100,
	kTAGFunctionFindResource		= 0x8114,
	kTAGFunctionAddTemplate			= 0x8118,
	kTAGFunctionMiscOp				= 0x8154,
	kTAGFunctionDivide01			= 0x81b0,
	kTAGFunctionDivide02			= 0x81b4,
	kTAGFunctionDivide12			= 0x81b8,
	kTAGFunctionDivide32			= 0x81bc
};

}

#endif
