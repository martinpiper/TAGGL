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
				if (resourceIndex < kMaxResourceTypes)
				{
					ResourceIndex::iterator found = mResources[resourceIndex].find(std::string(name));
					if (found != mResources[resourceIndex].end())
					{
						ResourceItem &item = found->second;
						if (item.mItem && item.mItem->mARMAddress)
						{
							error = false;
							SetRegister(0,item.mItem->mARMAddress);
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

				if (fromItem && (fromItem->mOriginalResourceTypeIndex == ROEditor::kWorld) && toItem && (toItem->mOriginalResourceTypeIndex == ROEditor::kWorld))
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
					if ((worldItem->mOriginalResourceTypeIndex == kWorld) && (item->mOriginalResourceTypeIndex == kTemplate))
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
			mNeedToSendSystemInits = true;

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
					if ((worldItem->mOriginalResourceTypeIndex == kWorld) && (item->mOriginalResourceTypeIndex == kWorldTemplate))
					{
						World *world = (World*)worldItem;
						World::WorldTemplate *worldTemplate = (World::WorldTemplate*)item;
						Template *theTemplate = worldTemplate->mTemplate;

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

		case kTAGFunctionMiscOp:
		case 0x814c:
		case 0x8140:
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
