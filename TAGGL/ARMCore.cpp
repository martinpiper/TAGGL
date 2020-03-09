#include <string.h>
#include <assert.h>
#include <windows.h>
#include "TAGGL/Inc/ARMCore.h"
#include "ARCEmu/armemul.h"
#include "ARCEmu/decode.h"

using namespace TAGGL;
using namespace simit;

static arm_emulator sTheARM(false,true);

// Depending on the build we can enable or disable some ARM debugging options.
#ifdef _DEBUG
// This helps to narrow down unimplemented handlers
//bool ARMCore::mDebuggingEnabled = false;
bool ARMCore::mDebuggingEnabled = true;

// Disassemble executed ARM instructions
//bool ARMCore::mDisassemble = false;
bool ARMCore::mDisassemble = true;

#else
bool ARMCore::mDebuggingEnabled = false;
bool ARMCore::mDisassemble = false;
#endif

//static bool sEnableARMCore = false;		// Enable this to stop all ARM code from running
static bool sEnableARMCore = true;


ARMCore::ARMCore()
{
	sTheARM.reset();
	sTheARM.mem->mark_region_available(0x30000000,0x10000,MEMORY_PAGE_READABLE | MEMORY_PAGE_WRITABLE);

	sTheARM.mem->mark_region_available(0x0,0x8000,MEMORY_PAGE_READABLE);
	sTheARM.mem->mark_region_available(0x8000,0x8000,MEMORY_PAGE_READABLE | MEMORY_PAGE_WRITABLE | MEMORY_PAGE_EXECUTABLE);
	sTheARM.mem->mark_region_available(0x10000,0x8000000,MEMORY_PAGE_READABLE | MEMORY_PAGE_WRITABLE | MEMORY_PAGE_EXECUTABLE);	// The main TAG handler and data heap
//	sTheARM.mem->mark_region_available(0x10000000,320*256,MEMORY_PAGE_READABLE | MEMORY_PAGE_WRITABLE);	// The screen

	sTheARM.mem->set_block(0x30000000, 0, 0x10000);
	sTheARM.cpu_val = SA1100;

	InitStack();
}

void TAGGL::ARMCore::InitStack()
{
	sTheARM.write_gpr(SPIND, 0x30000000 + 0x10000 - 4);
}

ARMCore::~ARMCore()
{
}

bool ARMCore::WriteMemory(const unsigned int targetAddr, const void *source, const unsigned int size)
{
	return (sTheARM.mem->write_block(source, targetAddr, size) == MEM_NO_FAULT);
}

bool ARMCore::ReadMemory(const unsigned int sourceAddr, void *dest, const unsigned int size)
{
	return (sTheARM.mem->read_block(dest, sourceAddr, size) == MEM_NO_FAULT);
}

static const unsigned int kHandlerRet = 0x100;

void ARMCore::SetPC(const unsigned int addr)
{
	int i;
	for (i=0;i<SPIND;i++)
	{
		// Setup all the registers to point to unused memory so unimplemented handler code will be more likely generate an exception
		sTheARM.write_gpr(i,0x20000000);
	}

	sTheARM.write_gpr(LRIND, kHandlerRet);	// When the PC reaches this it indicates a return from a handler

	sTheARM.write_gpr(PC_AHEAD_IND, addr);
	sTheARM.pc_set = true;
}

void ARMCore::SetRegister(const int reg, const unsigned int value,const bool PSR)
{
	sTheARM.write_gpr(reg,value);
	if ( PSR && (reg >= 15) )
	{
		sTheARM.write_cc( (value >> 28) & 0xf );
		sTheARM.cc_to_cfs();
	}
}

unsigned int ARMCore::GetRegister(const int reg)
{
	return sTheARM.read_gpr(reg);
}

bool ARMCore::Execute(const int cycles)
{
	if (!sEnableARMCore)
	{
		return true;
	}

	int toRun = cycles;

	while ((cycles == -1) || (toRun-- > 0))
	{
		arm_inst_t inst;
		word_t addr = sTheARM.get_pc();

		inst = sTheARM.fetch_inst_user(sTheARM.get_pc());

		if (ARMCore::mDisassemble)
		{
			char buffer[256] , buffer2[256];
			arm_disassemble(inst, sTheARM.get_pc(), buffer);
			sprintf(buffer2,"%08x : %08x %s\n",addr & 0x3ffffffc, inst, buffer);
			OutputDebugStringA(buffer2);
			buffer[0] = 0;
			for (int i = 0; i < 15; i++)
			{
				sprintf(buffer2, "%02d: %08x ", i , sTheARM.read_gpr(i));
				strcat(buffer, buffer2);
				if (i == 7)
				{
					strcat(buffer, "\n");
				}
			}
			strcat(buffer, "\n");
			OutputDebugStringA(buffer);
		}

		sTheARM.execute_user(inst, sTheARM.get_pc());

		if (sTheARM.mem->get_last_fault() != MEM_NO_FAULT)
		{
			if (ARMCore::mDebuggingEnabled)
			{
				if ( (sTheARM.mem->get_last_faddr() & 0xf0000000) == 0x10000000 )
				{
						OutputDebugStringA("Handler tried to access the screen\n");
				}
				else
				{
					assert(!"A handler produced an exception");
				}
			}
			sTheARM.mem->ClearFault();
			return false;
		}


		// Check to see if the handler is jumping to anywhere inside TAG
		unsigned int execNextAddr = sTheARM.get_pc() & 0x3ffffffc;	// Removes the status and mode
		// Did the handler return?
		if (execNextAddr == kHandlerRet)
		{
			return false;
		}
		if ((execNextAddr >= 0x8100) && (execNextAddr <= 0x8200))
		{
			if (!CallbackTAGFunction(execNextAddr))
			{
				return false;
			}
		}
	}

	return true;
}

bool ARMCore::CallbackTAGFunction(const unsigned int address)
{
	// For debugging just return
	SetRegister(15,GetRegister(14));
	return true;
}
