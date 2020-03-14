#include <stdlib.h>
#include <GL/glut.h>
#include <algorithm>
#include <assert.h>
#include <windows.h>
#include "TAGGL/Inc/ROEditor.h"
#include "TAGGL/Inc/System.h"
#include "TAGGL/Inc/World.h"
#include "TAGGL/Inc/Template.h"
#include "TAGGL/Inc/Object.h"
#include "TAGGL/Inc/DataChunk.h"
#include "TAGGL/Inc/TAGFunctions.h"
#include "RNLobby/Inc/ScanPath.h"

using namespace TAGGL;
using namespace RNReplicaNet;

bool ROEditor::CallbackTAGFunction(const unsigned int address)
{
	if (ARMCore::mDebuggingEnabled)
	{
		char buffer[256];
		sprintf(buffer,"CallbackTAGFunction $%x\n",address);
		OutputDebugStringA(buffer);
	}

	// Assume the call always fails for safety
	bool error = true;

	switch (address)
	{
		default:
		{
			if (ARMCore::mDebuggingEnabled)
			{
				assert(!"CallbackTAGFunction not implemented");
			}
			break;
		}

		case kTAGFunctionHeapAlloc:
		{
			unsigned int ret = ARMHeapAlloc(GetRegister(0));
			SetRegister(0,ret);
			error = false;
			break;
		}

		case kTAGFunctionFindResource:
		{
			char name[17];
			name[16] = '\0';
			unsigned int nameParam = GetRegister(1);
			if (ReadMemory(nameParam,name,16))
			{
				unsigned int resourceIndex = GetRegister(0) & 0xf;
				bool reportError = (GetRegister(0) & 0x80000000)?true:false;
				if (resourceIndex < ResourceItem::kMaxResourceTypes)
				{
					ResourceIndex::iterator found = mResources[resourceIndex].find(std::string(name));
					if (found != mResources[resourceIndex].end())
					{
						ResourceItem *item = found->second;
						if (item->mItem && item->mItem->mARMAddress)
						{
							error = false;
							SetRegister(0,item->mItem->mARMAddress);
						}
					}
					if (error && reportError && ARMCore::mDebuggingEnabled)
					{
						char buffer[1024];
						sprintf(buffer,"Resource '%s':%d not found\n",name,resourceIndex);
						OutputDebugStringA(buffer);

						assert(!"Resource not found");
					}
				}
				else if (reportError && ARMCore::mDebuggingEnabled)
				{
					assert(!"Resource index out of range");
				}
			}
			break;
		}

		case kTAGFunctionAddTemplate:
		{
			// SendEventToWorldTemplate() cannot easily be used during ROEditor::CallbackTAGFunction
			mNeedToSendSystemInits = true;

			unsigned int op = GetRegister(0);
			if (op == 0)
			{
				// Add world
				// MPi: TODO: 04/15 = misc parameters (if added using add_world)
				unsigned int fromWorld;
				unsigned int offX,offY,offZ;
				unsigned int handlerParams[3];
				ReadMemory(GetRegister(1),&fromWorld,sizeof(unsigned int));
				ReadMemory(GetRegister(1)+4,&offX,sizeof(unsigned int));
				ReadMemory(GetRegister(1)+8,&offY,sizeof(unsigned int));
				ReadMemory(GetRegister(1)+12,&offZ,sizeof(unsigned int));
				ReadMemory(GetRegister(1)+16,handlerParams,sizeof(handlerParams));

				BaseItem *fromItem = 0;
				fromItem = mARMAddressToBaseItem[fromWorld];

				BaseItem *toItem = 0;
				toItem = mARMAddressToBaseItem[GetRegister(2)];

				if (fromItem && (fromItem->mOriginalResourceTypeIndex == ResourceItem::kWorld) && toItem && (toItem->mOriginalResourceTypeIndex == ResourceItem::kWorld))
				{
					World *from = (World *) fromItem;
					World *to = (World *) toItem;

					std::list<World::WorldTemplate*>::iterator st = from->mTemplates.begin();
					while (st != from->mTemplates.end())
					{
						World::WorldTemplate *temp = *st++;
						to->mTemplates.push_back(temp);
					}
				}
			}
			else if (op == 1)
			{
				// Add template
				DynamicMessageHelper part;
				part.SetBufferSize(128);
				part.EnsureBufferAllocated();
				ReadMemory(GetRegister(1),part.GetBuffer(),part.GetBufferSize());

				unsigned int taddr;
				part >> taddr;
				BaseItem *item = 0;
				item = mARMAddressToBaseItem[taddr];
				BaseItem *worldItem = 0;
				taddr = GetRegister(2);
				worldItem = mARMAddressToBaseItem[taddr];

				if (!item || !worldItem)
				{
					if (ARMCore::mDebuggingEnabled)
					{
						assert(!"Could not resolve the template or world resource address");
					}
				}
				else
				{
					if ((worldItem->mOriginalResourceTypeIndex == ResourceItem::kWorld) && (item->mOriginalResourceTypeIndex == ResourceItem::kTemplate))
					{
						World *world = (World*) worldItem;

						World::WorldTemplate *temp = new World::WorldTemplate();
						temp->mARMAddress = ARMHeapAlloc(128);
						WriteMemory(temp->mARMAddress + 12,part.GetCurrentPosition(), 9 * 4);
						mARMAddressToBaseItem[temp->mARMAddress] = temp;

						// MPi: TODO: I think this needs some template data words ARM memory setup

						part >> temp->mIPosition;
						part >> temp->mIRotation;
						part >> temp->mIScale;

						part >> temp->mICollisionDistance;
						part >> temp->mDataWords[0];
						part >> temp->mDataWords[1];

						temp->mTemplate = (Template*) item;

						world->mTemplates.push_back(temp);
						error = false;

						if (temp->mTemplate->mARMCompiledAddress)
						{
							// Point the world template to the template data
							WriteMemory(temp->mARMAddress+8,&temp->mTemplate->mARMCompiledAddress,4);
						}
					}
					else
					{
						if (ARMCore::mDebuggingEnabled)
						{
							assert(!"kTAGFunctionAddTemplate bad resource types being added");
						}
					}
				}
			}
			break;
		}

		case kTAGFunctionDeleteTemplate:
		{
			unsigned int op = GetRegister(0);
			if (op == 1)
			{
				unsigned int taddr;
				taddr = GetRegister(1);
				BaseItem *item = 0;
				item = mARMAddressToBaseItem[taddr];
				BaseItem *worldItem = 0;
				taddr = GetRegister(2);
				worldItem = mARMAddressToBaseItem[taddr];

				if (!item || !worldItem)
				{
					if (ARMCore::mDebuggingEnabled)
					{
						assert(!"Could not resolve the template or world resource address");
					}
				}
				else
				{
					if ((worldItem->mOriginalResourceTypeIndex == ResourceItem::kWorld) && (item->mOriginalResourceTypeIndex == ResourceItem::kWorldTemplate))
					{
						World *world = (World*)worldItem;
						World::WorldTemplate *worldTemplate = (World::WorldTemplate*)item;

						SendEventToWorldTemplate(worldTemplate->mSystemWorld, world, worldTemplate, 7);
						world->mTemplates.remove(worldTemplate);

						error = false;
					}
					else
					{
						if (ARMCore::mDebuggingEnabled)
						{
							assert(!"kTAGFunctionDeleteTemplate bad resource types being added");
						}
					}
				}
			}
			break;
		}

		case kTAGFunctionExecNew:
		{
			char name[257];
			name[256] = '\0';
			unsigned int r0 = GetRegister(0);
			unsigned int r1 = GetRegister(1);
			if (r0 == 0 && r1 != 0)
			{
				if (ReadMemory(r1, name, 256))
				{
					error = false;
					if (ARMCore::mDebuggingEnabled)
					{
						assert(!"TODO");
					}
				}
				else if (ARMCore::mDebuggingEnabled)
				{
					assert(!"kTAGFunctionExecNew could not read name when r0=0");
				}
			}
			else if (r0 == 0xffffffff && r1 != 0)
			{
				// : -1 : > 0 : load app data(R1 = filename) (R2 = crc word) : R0 = app handle : N:
				if (ReadMemory(r1, name, 256))
				{
					std::string riscosPath = name;
					TidyRISCOSPath(riscosPath);
					riscosPath = mRootDirectory + riscosPath;

					if (ARMCore::mDebuggingEnabled)
					{
						char buffer[1024];
						sprintf(buffer, "Loading app data '%s'\n", riscosPath.c_str());
						OutputDebugStringA(buffer);
					}

					DynamicMessageHelper file;
					bool ret = file.Read(riscosPath.c_str(), true);
					assert(ret && "No file loaded");
					ReadOutput(file);

					unsigned int loadedIteration = GetLoadedIteration();

					if (ARMCore::mDebuggingEnabled)
					{
						char buffer[1024];
						sprintf(buffer, "Loaded app data '%s' app handle $%08x\n", riscosPath.c_str() , loadedIteration);
						OutputDebugStringA(buffer);
					}

					SendEventToAllHandlers(0, loadedIteration);

					SetRegister(0, loadedIteration);

					error = false;
				}
				else if (ARMCore::mDebuggingEnabled)
				{
					assert(!"kTAGFunctionExecNew could not read name when r0=0");
				}
			}
			else if (ARMCore::mDebuggingEnabled)
			{
				assert(!"kTAGFunctionExecNew unhandled entry state");
			}
			break;
		}

		case kTAGFunctionMiscOp:
		case 0x8124:
		case 0x814c:
		case 0x8140:
		case 0x8184:
		case 0x81e0:
		case 0x81e4:
		case 0x81d0:
		case 0x81c8:
		{
			// MPi: TODO: Implement properly
			SetRegister(0,0);
			error = false;
			break;
		}

		case kTAGFunctionDivide01:
		{
			if (GetRegister(1) == 0)
			{
				SetRegister(0,0);
			}
			else
			{
				SetRegister(0,GetRegister(0)/GetRegister(1));
			}
			error = false;
			break;
		}

		case kTAGFunctionDivide02:
		{
			if (GetRegister(2) == 0)
			{
				SetRegister(0,0);
			}
			else
			{
				SetRegister(0,GetRegister(0)/GetRegister(2));
			}
			error = false;
			break;
		}

		case kTAGFunctionDivide12:
		{
			if (GetRegister(2) == 0)
			{
				SetRegister(1,0);
			}
			else
			{
				SetRegister(1,GetRegister(1)/GetRegister(2));
			}
			error = false;
			break;
		}

		case kTAGFunctionDivide32:
		{
			if (GetRegister(2) == 0)
			{
				SetRegister(3,0);
			}
			else
			{
				SetRegister(3,GetRegister(3)/GetRegister(2));
			}
			error = false;
			break;
		}

		case kTAGFunctionReportError:
		{
			char message[256];
			message[255] = '\0';
			unsigned int r0 = GetRegister(0);
			if (ReadMemory(r0+4, message, 255))
			{
				char buffer[256];
				sprintf(buffer, "kTAGFunctionReportError $%08x %s\n", r0 , message);
				OutputDebugStringA(buffer);
			}
			assert(!"kTAGFunctionReportError");
			break;
		}


	}

	if (error)
	{
		SetRegister(15,GetRegister(14) | kPSROverflow);
	}
	else
	{
		SetRegister(15,GetRegister(14) & ~kPSROverflow);
	}
	return true;
}

bool ROEditor::CallbackSWI(const unsigned int swi)
{
	if (ARMCore::mDebuggingEnabled)
	{
		OutputDebugStringA("gLastSysCall\n");
	}
	switch (swi)
	{
		default:
			break;
		case 0x6:
		{
			if ((rand() & 3) == 0)
			{
				SetRegister(1, 0xff);
			}
			break;
		}
		case 0x2000c:
		{
			int os_gbpb_type = GetRegister(0);
			if (os_gbpb_type == 12)
			{
				char directoryName[256];
				ReadMemory(GetRegister(1), directoryName, sizeof(directoryName));
				int numObjectsToRead = GetRegister(3);
				int whereToStart = GetRegister(4);
				int lengthOfBuffer = GetRegister(5);
				char wildcardNameToMatch[256];
				ReadMemory(GetRegister(6), wildcardNameToMatch, sizeof(wildcardNameToMatch));

				assert(numObjectsToRead == 1);

				std::string riscosPath = directoryName;
				TidyRISCOSPath(riscosPath);
				if (riscosPath.empty())
				{
					riscosPath = mRootDirectory;
				}
				else
				{
					riscosPath = mRootDirectory + riscosPath;
					riscosPath += "\\";
				}
				std::string toSearch = riscosPath + wildcardNameToMatch;

				std::list<RNLobby::ScanPath::Entry> result;
				RNLobby::ScanPath scanPath;
				scanPath.Start(toSearch.c_str(), result);

				std::list<RNLobby::ScanPath::Entry>::iterator st = result.begin();
				std::advance(st, whereToStart);

				if (st == result.end())
				{
					SetRegister(3, 0);
					SetRegister(4, -1);
					// MPi: TODO: Set carry?
				}
				else
				{
					// The mmeory buffer format https://www.riscosopen.org/wiki/documentation/show/OS_GBPB%2012%20Buffer
					DynamicMessageHelper os_gbpbBuffer;
					os_gbpbBuffer.AddInteger(0);
					os_gbpbBuffer.AddInteger(0);
					os_gbpbBuffer.AddInteger((int)st->mSize);
					os_gbpbBuffer.AddInteger(0); // Read access for user
					os_gbpbBuffer.AddInteger(1); // File
					os_gbpbBuffer.AddInteger(0xffd); // File type arbitrary data
					std::string name = st->mName.substr(riscosPath.length());
					os_gbpbBuffer.AddData(name.c_str(), name.length()+1);
					WriteMemory(GetRegister(2), os_gbpbBuffer.GetBuffer(), os_gbpbBuffer.GetSize());
					st++;
					if (st == result.end())
					{
						SetRegister(4, -1);
					}
					else
					{
						SetRegister(4, whereToStart + 1);
					}
				}
			}
			break;
		}
	}
	if (ARMCore::mDebuggingEnabled)
	{
		OutputDebugStringA("gLastSysCall end\n");
	}

	return true;
}

void TAGGL::ROEditor::TidyRISCOSPath(std::string &riscosPath)
{
	size_t pos;
	if ((pos = riscosPath.find_last_of(':')) != std::string::npos)
	{
		riscosPath = riscosPath.substr(pos + 1);
	}
	std::replace(riscosPath.begin(), riscosPath.end(), '.', '\\');
}
