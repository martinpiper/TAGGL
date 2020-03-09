#ifndef _TAGGL_ARMCORE_H_
#define _TAGGL_ARMCORE_H_
#pragma once

namespace TAGGL
{

/// Emulates some of a 26 bit StrongARM core. Enough to get simple TAG handlers running.
class ARMCore
{
public:
	ARMCore();
	virtual ~ARMCore();

	static const unsigned int kPSRNegative	= 0x80000000;
	static const unsigned int kPSRZero		= 0x80000000;
	static const unsigned int kPSRCarry	= 0x80000000;
	static const unsigned int kPSROverflow	= 0x80000000;

	/// Any new memory is cleared to zero before being used.
	bool WriteMemory(const unsigned int targetAddr, const void *source, const unsigned int size);
	bool ReadMemory(const unsigned int sourceAddr, void *dest, const unsigned int size);
	void SetPC(const unsigned int addr);
	void SetRegister(const int reg, const unsigned int value,const bool PSR = true);
	unsigned int GetRegister(const int reg);
	bool Execute(const int cycles = -1);
	void InitStack();

	virtual bool CallbackTAGFunction(const unsigned int address);

	static bool mDisassemble;
	static bool mDebuggingEnabled;
};

}

#endif
