#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
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

using namespace TAGGL;
using namespace RNReplicaNet;

unsigned int TAGGL::gRISCOSToRGB[256] =
{
	0x000000ff, 0x111111ff, 0x222222ff, 0x333333ff,
	0x440000ff, 0x551111ff, 0x662222ff, 0x773333ff,
	0x000044ff, 0x111155ff, 0x222266ff, 0x333377ff,
	0x440044ff, 0x551155ff, 0x662266ff, 0x773377ff,
	0x880000ff, 0x991111ff, 0xaa2222ff, 0xbb3333ff,
	0xcc0000ff, 0xdd1111ff, 0xee2222ff, 0xff3333ff,
	0x880044ff, 0x991155ff, 0xaa2266ff, 0xbb3377ff,
	0xcc0044ff, 0xdd1155ff, 0xee2266ff, 0xff3377ff,
	0x004400ff, 0x115511ff, 0x226622ff, 0x337733ff,
	0x444400ff, 0x555511ff, 0x666622ff, 0x777733ff,
	0x004444ff, 0x115555ff, 0x226666ff, 0x337777ff,
	0x444444ff, 0x555555ff, 0x666666ff, 0x777777ff,
	0x884400ff, 0x995511ff, 0xaa6622ff, 0xbb7733ff,
	0xcc4400ff, 0xdd5511ff, 0xee6622ff, 0xff7733ff,
	0x884444ff, 0x995555ff, 0xaa6666ff, 0xbb7777ff,
	0xcc4444ff, 0xdd5555ff, 0xee6666ff, 0xff7777ff,
	0x008800ff, 0x119911ff, 0x22aa22ff, 0x33bb33ff,
	0x448800ff, 0x559911ff, 0x66aa22ff, 0x77bb33ff,
	0x008844ff, 0x119955ff, 0x22aa66ff, 0x33bb77ff,
	0x448844ff, 0x559955ff, 0x66aa66ff, 0x77bb77ff,
	0x888800ff, 0x999911ff, 0xaaaa22ff, 0xbbbb33ff,
	0xcc8800ff, 0xdd9911ff, 0xeeaa22ff, 0xffbb33ff,
	0x888844ff, 0x999955ff, 0xaaaa66ff, 0xbbbb77ff,
	0xcc8844ff, 0xdd9955ff, 0xeeaa66ff, 0xffbb77ff,
	0x00cc00ff, 0x11dd11ff, 0x22ee22ff, 0x33ff33ff,
	0x44cc00ff, 0x55dd11ff, 0x66ee22ff, 0x77ff33ff,
	0x00cc44ff, 0x11dd55ff, 0x22ee66ff, 0x33ff77ff,
	0x44cc44ff, 0x55dd55ff, 0x66ee66ff, 0x77ff77ff,
	0x88cc00ff, 0x99dd11ff, 0xaaee22ff, 0xbbff33ff,
	0xcccc00ff, 0xdddd11ff, 0xeeee22ff, 0xffff33ff,
	0x88cc44ff, 0x99dd55ff, 0xaaee66ff, 0xbbff77ff,
	0xcccc44ff, 0xdddd55ff, 0xeeee66ff, 0xffff77ff,
	0x000088ff, 0x111199ff, 0x2222aaff, 0x3333bbff,
	0x440088ff, 0x551199ff, 0x6622aaff, 0x7733bbff,
	0x0000ccff, 0x1111ddff, 0x2222eeff, 0x3333ffff,
	0x4400ccff, 0x5511ddff, 0x6622eeff, 0x7733ffff,
	0x880088ff, 0x991199ff, 0xaa22aaff, 0xbb33bbff,
	0xcc0088ff, 0xdd1199ff, 0xee22aaff, 0xff33bbff,
	0x8800ccff, 0x9911ddff, 0xaa22eeff, 0xbb33ffff,
	0xcc00ccff, 0xdd11ddff, 0xee22eeff, 0xff33ffff,
	0x004488ff, 0x115599ff, 0x2266aaff, 0x3377bbff,
	0x444488ff, 0x555599ff, 0x6666aaff, 0x7777bbff,
	0x0044ccff, 0x1155ddff, 0x2266eeff, 0x3377ffff,
	0x4444ccff, 0x5555ddff, 0x6666eeff, 0x7777ffff,
	0x884488ff, 0x995599ff, 0xaa66aaff, 0xbb77bbff,
	0xcc4488ff, 0xdd5599ff, 0xee66aaff, 0xff77bbff,
	0x8844ccff, 0x9955ddff, 0xaa66eeff, 0xbb77ffff,
	0xcc44ccff, 0xdd55ddff, 0xee66eeff, 0xff77ffff,
	0x008888ff, 0x119999ff, 0x22aaaaff, 0x33bbbbff,
	0x448888ff, 0x559999ff, 0x66aaaaff, 0x77bbbbff,
	0x0088ccff, 0x1199ddff, 0x22aaeeff, 0x33bbffff,
	0x4488ccff, 0x5599ddff, 0x66aaeeff, 0x77bbffff,
	0x888888ff, 0x999999ff, 0xaaaaaaff, 0xbbbbbbff,
	0xcc8888ff, 0xdd9999ff, 0xeeaaaaff, 0xffbbbbff,
	0x8888ccff, 0x9999ddff, 0xaaaaeeff, 0xbbbbffff,
	0xcc88ccff, 0xdd99ddff, 0xeeaaeeff, 0xffbbffff,
	0x00cc88ff, 0x11dd99ff, 0x22eeaaff, 0x33ffbbff,
	0x44cc88ff, 0x55dd99ff, 0x66eeaaff, 0x77ffbbff,
	0x00ccccff, 0x11ddddff, 0x22eeeeff, 0x33ffffff,
	0x44ccccff, 0x55ddddff, 0x66eeeeff, 0x77ffffff,
	0x88cc88ff, 0x99dd99ff, 0xaaeeaaff, 0xbbffbbff,
	0xcccc88ff, 0xdddd99ff, 0xeeeeaaff, 0xffffbbff,
	0x88ccccff, 0x99ddddff, 0xaaeeeeff, 0xbbffffff,
	0xccccccff, 0xddddddff, 0xeeeeeeff, 0xffffffff
};

static int sLoadedIteration = 0;

// MPi: TODO: Produce a proper heap that can free blocks. At the moment this just increments.
static unsigned int sLoadedHeap = 0x10000;

ROEditor::ROEditor() : mCurrentSystem(0) , mNeedToSendSystemInits(true)
{
	// Setup the "Global Data Ptrs" described in "Functions" document
	unsigned int address = ARMHeapAlloc(1*1024);
	WriteMemory(0x81f4,&address,sizeof(address));
	address = ARMHeapAlloc(4*1024);
	WriteMemory(0x81f8,&address,sizeof(address));

	// These 4 and 4096 magic values seem to work for BHP, but they don't work for Cobalt Seed
	mCosTable.Allocate(sizeof(int)*kAngCount*4);
	mSinTable.Allocate(sizeof(int)*kAngCount*4);
	int i;
	for (i = 0 ; i < kAngCount*4 ; i++)
	{
		((int *)mCosTable.mData)[i] = cos((double(i) * 2.0f * M_PI) / double(kAngCount)) * 4096;
		((int *)mSinTable.mData)[i] = sin((double(i) * 2.0f * M_PI) / double(kAngCount)) * 4096;
	}
	mARMCosTableAddress = ARMHeapAlloc(sizeof(int)*kAngCount*4);
	WriteMemory(mARMCosTableAddress, mCosTable.mData, sizeof(int)*kAngCount*4);
	mARMSinTableAddress = ARMHeapAlloc(sizeof(int)*kAngCount*4);
	WriteMemory(mARMSinTableAddress, mSinTable.mData, sizeof(int)*kAngCount*4);
}

ROEditor::~ROEditor()
{
}

bool ROEditor::ReadOutput(RNReplicaNet::MessageHelper &data)
{
	mNeedToSendSystemInits = true;
	int thisLoadedIteration = sLoadedIteration++;

	if (ARMCore::mDebuggingEnabled)
	{
		char buffer[256];
		sprintf(buffer,"ROEditor::ReadOutput thisLoadedIteration=%d\n",thisLoadedIteration);
		OutputDebugStringA(buffer);
	}

	int temp;
	data >> temp;
	// Test for "TAGE"
	if (temp != 0x45474154)
	{
		assert(!"No TAGE header");
		return false;
	}
	int version;
	data >> version;

	int totalResources;
	data >> totalResources;

	int dataStartOffset = 96 + (totalResources*32);

	unsigned int totalResourceSize;
	data >> totalResourceSize;

	data.SkipData(16);	// Plot data buffer sizes

	int vanishingPoint;
	data >> vanishingPoint;

	data.SkipData(12);	// RFU and compressed block sizes;

	char startupSystem[17];
	startupSystem[16] = '\0';
	data.GetData(startupSystem,16);

	int numResourceItems[kMaxResourceTypes];
	int i;
	for (i = 0; i < kMaxResourceTypes ; i++)
	{
		data >> numResourceItems[i];
	}

	std::vector<unsigned int> textureHandles;
	if (numResourceItems[kSprite] > 0)
	{
		textureHandles.resize(numResourceItems[kSprite]);
		glGenTextures(textureHandles.size(), &textureHandles[0]);
	}

	std::vector<System *> loadedSystems;
	std::vector<World *> loadedWorlds;
	std::vector<Template *> loadedTemplates;
	std::vector<Object *> loadedObjects;
	std::vector<DataChunk *> loadedHandlers;

	int numResourcesParsed = 0;

	for (i = 0; i < kMaxResourceTypes ; i++)
	{
		int j;
		for (j = 0; j < numResourceItems[i] ; j++)
		{
			ResourceItem item;

			item.mType = i;
			item.mOriginalIndex = j;
			item.mLoadedByIteration = thisLoadedIteration;

			data >> item.mFileOffset;

			data.SkipData(12);

			if (numResourcesParsed < (totalResources-1))
			{
				// Skip forward to the next resource file offset
				MessageHelper next;
				next.SetBuffer(data.GetCurrentPosition());
				next.SkipData(16);
				unsigned int nextOffset;
				next >> nextOffset;
				item.mChunkSize = (int) nextOffset - item.mFileOffset;
			}
			else
			{
				item.mChunkSize = totalResourceSize - item.mFileOffset;
			}

			char name[17];
			name[16] = '\0';
			data.GetData(name,16);

			MessageHelper part;
			part.SetBuffer( ((char*)data.GetBuffer()) + item.mFileOffset + dataStartOffset );
			part.SetGuardSize(item.mChunkSize);

			// If either of these are set then they get added to the ARM resource header
			unsigned int compiledData = 0;
			unsigned int liveData = 0;
			switch(i)
			{
				case kSystem:
				{
					System *system = new System;
					item.mItem = system;
					loadedSystems.push_back(system);

					if (_stricmp(startupSystem,name) == 0)
					{
						mCurrentSystem = system;
					}

					compiledData = ARMHeapAlloc(64);
					liveData = ARMHeapAlloc(64);
					WriteMemory(compiledData,part.GetCurrentPosition(), 6 * 4);
					// MPi: TODO: When the system is re-run then this needs to be copied from the loaded data
					WriteMemory(liveData + 8,part.GetCurrentPosition(), 6 * 4);


					part >> system->mIViewPosition;
					part >> system->mIViewRotation;

					int index;
					part >> index;
					if (index == -3)
					{
						system->mLightSourceEnabled = true;
						part >> system->mLightSourceType;
						part >> system->mlightSourceShift;
						part >> system->mAmbientLevels[0];
						part >> system->mAmbientLevels[1];
						part >> system->mAmbientLevels[2];
						part >> system->mMaxLevels[0];
						part >> system->mMaxLevels[1];
						part >> system->mMaxLevels[2];
						part >> system->mILightPosition;

						part >> index;
					}
					while (index != -1)
					{
						assert(index < numResourceItems[kWorld]);
						System::SystemWorld world;
						world.mResourceIndex = index;
						world.mARMAddress = ARMHeapAlloc(32);
						WriteMemory(world.mARMAddress + 12, part.GetCurrentPosition(), 20);
						unsigned char flag;
						part >> flag;
						world.mMiscResource = flag?true:false;
						part >> flag;
						world.mFilledRender = flag?true:false;
						unsigned char otherFlags;
						part >> otherFlags;
						if (otherFlags & 1)
						{
							world.mIgnoreViewpoint = true;
						}
						if (otherFlags & 2)
						{
							world.mBackdrop = true;
						}
						if (otherFlags & 4)
						{
							world.mNoHandlerEvents = true;
						}
						part.SkipData(1);	// RFU
						part >> world.mIPositionOffset;
						part >> world.mGroupNumber;

						system->mWorlds.push_back(world);

						part >> index;
					}
					break;
				}

				case kWorld:
				{
					World *world = new World;
					item.mItem = world;
					loadedWorlds.push_back(world);

					compiledData = ARMHeapAlloc(item.mChunkSize);
					WriteMemory(compiledData,part.GetCurrentPosition(), item.mChunkSize);

					// MPi: TODO: The live world needs a proper link list
					liveData = ARMHeapAlloc(2 * 4);

					// MPi: TODO: When the world is re-run then this needs to be copied from the loaded data
					WriteMemory(liveData + 8,part.GetCurrentPosition(), 2 * 4);


					int index;
					part >> index;
					while (index != -1)
					{
						assert(index < numResourceItems[kTemplate]);
						World::WorldTemplate *temp = new World::WorldTemplate();
						temp->mResourceIndex = index;

						// MPi: TODO: Implement proper TAG ARM internal data structures
						temp->mARMAddress = ARMHeapAlloc(128);
						WriteMemory(temp->mARMAddress + 12,part.GetCurrentPosition(), 9 * 4);

						part >> temp->mIPosition;
						part >> temp->mIRotation;
						part >> temp->mIScale;

						part >> temp->mICollisionDistance;
						part >> temp->mDataWords[0];
						part >> temp->mDataWords[1];

						WriteMemory(temp->mARMAddress + 52,&temp->mDataWords[0], 4);
						WriteMemory(temp->mARMAddress + 56,&temp->mDataWords[1], 4);

						world->mTemplates.push_back(temp);
						mARMAddressToBaseItem[temp->mARMAddress] = temp;

						part >> index;
					}
					break;
				}

				case kTemplate:
				{
					Template *temp = new Template;
					item.mItem = temp;
					loadedTemplates.push_back(temp);

					int num;

					part >> num;
					if (num & 0x80000000)
					{
						temp->mLighting = true;
					}
					num = num & 0x7fffffff;

					while (num-- > 0)
					{
						Template::TemplateObject obj;
						part >> obj.mResourceIndex;
						assert(obj.mResourceIndex < numResourceItems[kObject]);
						part >> obj.mIDistance;

						temp->mObjects.push_back(obj);
					}
					part >> num;	// Always -1, apparently.

					part >> num;
					while (num-- > 0)
					{
						Template::TemplateHandler hand;
						part >> hand.mResourceIndex;
						assert(hand.mResourceIndex < numResourceItems[kHandler]);

						// MPi: TODO: Implement proper TAG ARM internal data structures
						hand.mARMAddress = ARMHeapAlloc(4*4);
						WriteMemory(hand.mARMAddress,part.GetCurrentPosition(), 4 * 4);

						part >> hand.mDataWords[0];
						part >> hand.mDataWords[1];
						part >> hand.mDataWords[2];
						part >> hand.mDataWords[3];

						temp->mHandlers.push_back(hand);
					}

					// Using compiledData since the templates don't have live TAG data.
					compiledData = ARMHeapAlloc(4 + (temp->mObjects.size() * 8) + 4 + (temp->mHandlers.size() * 20));

					break;
				}

				case kObject:
				{
					Object *object = ReadOutputObject(part);
					item.mItem = object;
					loadedObjects.push_back(object);

					mObjectsLoaded.push_back(object);
					break;
				}

				case kSprite:
				{
					int colourDepth;

					DataChunk *chunk = new DataChunk();
					item.mItem = chunk;
					compiledData = ARMHeapAlloc(item.mChunkSize);
					WriteMemory(compiledData,part.GetBuffer(), item.mChunkSize);

					part >> colourDepth;
					assert((colourDepth == 3) && "Unsupported colour depth");
					int width,height;
					part >> width;
					part >> height;
					unsigned int offsetData , offsetPal , offsetMask;
					part >> offsetData;
					part >> offsetPal;
					part >> offsetMask;
					part.SkipData(3*4);	// Size data
					int RISCOSScreenmode;
					part >> RISCOSScreenmode;

					// Data
					unsigned char *pixels = ((unsigned char*)data.GetBuffer()) + item.mFileOffset + dataStartOffset + offsetData;
					unsigned int *pixelData = (unsigned int*)calloc(width * height,sizeof(unsigned int));

					unsigned char *mask = 0;
					if (offsetMask)
					{
						mask = ((unsigned char*)data.GetBuffer()) + item.mFileOffset + dataStartOffset + offsetMask;
					}

					int x , y;
					for (y = 0 ; y < height ; y++)
					{
						for (x = 0 ; x < width ; x++)
						{
							unsigned int RGBA = gRISCOSToRGB[pixels[(y*width)+x]];
							pixelData[(y*width)+x] = ((RGBA >> 24) & 0xff) | (((RGBA >> 16) & 0xff) << 8) | (((RGBA >> 8) & 0xff) << 16) | 0xff000000;
						}
					}

					glBindTexture(GL_TEXTURE_2D, textureHandles[j]);
//					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
//					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
					gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
					free(pixelData);

					break;
				}

				case kHandler:
				case kMiscData:
				{
					DataChunk *chunk = new DataChunk();
					item.mItem = chunk;
					loadedHandlers.push_back(chunk);

					chunk->Allocate(item.mChunkSize);
					memcpy(chunk->mData, part.GetBuffer(), item.mChunkSize);

					compiledData = ARMHeapAlloc(item.mChunkSize);
					WriteMemory(compiledData,chunk->mData, item.mChunkSize);


					break;
				}

				default:
				{
					DataChunk *chunk = new DataChunk();
					item.mItem = chunk;

					chunk->Allocate(item.mChunkSize);
					memcpy(chunk->mData, part.GetBuffer(), item.mChunkSize);
					break;
				}
			}

			if (item.mItem)
			{
				item.mItem->mOriginalResourceTypeIndex = i;
				item.mItem->mOriginalResourceName = name;

				CreateARMResource(item.mItem,name);

				if (compiledData)
				{
					WriteMemory(item.mItem->mARMAddress,&compiledData, 4);
					item.mItem->mARMCompiledAddress = compiledData;
					mARMAddressToBaseItem.insert(std::pair<unsigned int,BaseItem *>(compiledData,item.mItem));
				}

				if (liveData)
				{
					WriteMemory(item.mItem->mARMAddress + 4,&liveData, 4);
					item.mItem->mARMLiveAddress = liveData;
					mARMAddressToBaseItem.insert(std::pair<unsigned int,BaseItem *>(liveData,item.mItem));
				}

				if (ARMCore::mDebuggingEnabled)
				{
					char buffer[256];
					sprintf(buffer,"Loaded resource '%s':%d resource $%x compiled $%x live $%x\n",name,i,item.mItem->mARMAddress,item.mItem->mARMCompiledAddress,item.mItem->mARMLiveAddress);
					OutputDebugStringA(buffer);
				}
			}

			std::pair<ResourceIndex::iterator,bool> ret = mResources[i].insert(std::pair<std::string,ResourceItem>(std::string(name),item));
			if (!ret.second)
			{
				// A resource exists with the same name, so unlink it and apply the new one
				ResourceItem &toReplace = ret.first->second;
				if (toReplace.mItem && toReplace.mItem->mARMCompiledAddress)
				{
					mARMAddressToBaseItem.erase(toReplace.mItem->mARMCompiledAddress);
				}
				if (toReplace.mItem && toReplace.mItem->mARMLiveAddress)
				{
					mARMAddressToBaseItem.erase(toReplace.mItem->mARMLiveAddress);
				}
				toReplace = item;
			}
			numResourcesParsed++;
		}
	}

	// Update loaded objects with the loaded texture index
	size_t ind;
	for (ind = 0; ind < loadedObjects.size() ; ind++ )
	{
		Object *object = loadedObjects[ind];
		object->ROOutputToLive();
		size_t i;
		for (i = 0 ; i < object->mFaces.size() ; i++)
		{
			if (object->mFaces[i].mIsTextured)
			{
				FFace &face = object->mFaces[i];
				unsigned int handle = face.mTextureHandle;
				if (handle >= textureHandles.size())
				{
					// Something went wrong with the handle value, so mark the face with purple
					face.mIsTextured = false;
					face.mColourR = 1.0f;
					face.mColourG = 0.0f;
					face.mColourB = 1.0f;
				}
				else
				{
					face.mTextureHandle = textureHandles[handle];
				}
			}
		}
	}

	for (ind = 0; ind < loadedTemplates.size() ; ind++ )
	{
		Template *temp = loadedTemplates[ind];
		size_t i;
		for (i = 0; i < temp->mObjects.size() ; i++)
		{
			if (temp->mObjects[i].mResourceIndex >= 0)
			{
				temp->mObjects[i].mObject = loadedObjects[temp->mObjects[i].mResourceIndex];
			}
		}

		for (i = 0; i < temp->mHandlers.size() ; i++)
		{
			if (temp->mHandlers[i].mResourceIndex >= 0)
			{
				temp->mHandlers[i].mHandler = loadedHandlers[temp->mHandlers[i].mResourceIndex];
			}
		}
	}

	for (ind = 0; ind < loadedWorlds.size() ; ind++ )
	{
		World *world = loadedWorlds[ind];
		std::list<World::WorldTemplate*>::iterator st;
		st = world->mTemplates.begin();
		while (st != world->mTemplates.end())
		{
			World::WorldTemplate *temp = *st++;
			if (temp->mResourceIndex >= 0)
			{
				temp->mTemplate = loadedTemplates[temp->mResourceIndex];
				if (temp->mARMAddress && temp->mTemplate && temp->mTemplate->mARMCompiledAddress)
				{
					// Point the world template to the template data
					WriteMemory(temp->mARMAddress+8,&temp->mTemplate->mARMCompiledAddress,4);
				}
			}
		}
	}

	for (ind = 0; ind < loadedTemplates.size() ; ind++ )
	{
		if (loadedTemplates[ind] && loadedTemplates[ind]->mARMAddress)
		{
			// Now create template memory, not done during the load since there are too many unknowns ARM pointers
			unsigned int compiledData = loadedTemplates[ind]->mARMCompiledAddress;
			if (compiledData)
			{
				unsigned int temp = loadedTemplates[ind]->mObjects.size();
				WriteMemory(compiledData,&temp, sizeof(temp));
				temp = loadedTemplates[ind]->mHandlers.size();
				WriteMemory(compiledData + 4 + (loadedTemplates[ind]->mObjects.size() * 8),&temp, sizeof(temp));
				// MPi: TODO: Fill in proper template live data like object and handler data
			}
		}
	}

	for (ind = 0; ind < loadedSystems.size() ; ind++ )
	{
		System *system = loadedSystems[ind];
		std::list<System::SystemWorld>::iterator st;
		st = system->mWorlds.begin();
		while (st != system->mWorlds.end())
		{
			System::SystemWorld &temp = *st++;
			if (temp.mResourceIndex >= 0)
			{
				if (temp.mMiscResource)
				{
					assert(false && "Misc resource not implemented");
				}
				else
				{
					temp.mWorld = loadedWorlds[temp.mResourceIndex];
				}
			}
		}
	}

	return true;
}

static void operator >>(RNReplicaNet::MessageHelper &lhs , IVector3 &rhs)
{
	lhs >> rhs.mX;
	lhs >> rhs.mY;
	lhs >> rhs.mZ;
}

Object *ROEditor::ReadOutputObject(RNReplicaNet::MessageHelper &data)
{
	Object *object = new Object();

	int numVerts;
	data >> numVerts;

	object->mIVectors.reserve(numVerts);
	while(numVerts-- > 0)
	{
		IVector3 v;
		data >> v;
		object->mIVectors.push_back(v);
	}

	int numCollision;
	data >> numCollision;
	while (numCollision-- > 0)
	{
		ICollision col;
		data >> col.mVertexIndex;
		data >> col.mSize;

		object->mICollision.push_back(col);
	}

	int faceType;
	data >> faceType;
	while (faceType != -1)
	{
		IFace face;
		face.mType = faceType;
		data >> face.mDesc1;
		data >> face.mDesc2;

		// Texture mapping coordinates are always set to be this
		face.mTexture.mX[0] = 0;
		face.mTexture.mY[0] = 0;
		face.mTexture.mX[1] = 0x4000;
		face.mTexture.mY[1] = 0;
		face.mTexture.mX[2] = 0x4000;
		face.mTexture.mY[2] = 0x4000;
		face.mTexture.mX[3] = 0;
		face.mTexture.mY[3] = 0x4000;

		if ((face.mType & 0x80000000) && (face.mType & 0x07) >= 2 && (face.mType & 0x07) <= 7)
		{
			face.mIsTextured = true;

			int i;
			for (i=0;i<4;i++)
			{
				data >> face.mTexture.mX[i];
				data >> face.mTexture.mY[i];
			}
		}
		else if ((face.mType & 0x07) >= 2 && (face.mType & 0x07) <= 7)
		{
			face.mIsTextured = true;
		}
		else
		{
			if ( (face.mType &0x07) == 1 )
			{
				face.mIsWireframe = true;
			}
		}

		// MPi: TODO: Add scaled sprites

		int vertexIndex;
		data >> vertexIndex;

		while (vertexIndex != -1)
		{
			face.mVertList.push_back(vertexIndex);
			data >> vertexIndex;
		}

		object->mIFaces.push_back(face);
		data >> faceType;
	}

	return object;
}

unsigned int ROEditor::ARMHeapAlloc(unsigned int size)
{
	unsigned int ret = sLoadedHeap;
	// Align
	sLoadedHeap += (size + 15 + 16) & 0xfffffff0;		// Deliberate round up from 0 and to the nearest 32 bytes

	return ret;
}

bool ROEditor::SendEventToAllHandlers(const unsigned int event)
{
	// Send to all loaded handlers
	ResourceIndex::iterator st = mResources[kHandler].begin();
	while (st != mResources[kHandler].end())
	{
		DataChunk *handler = (DataChunk*) st->second.mItem;
		unsigned int offset = 0;
		if (handler && handler->mARMCompiledAddress && (handler->mSize >= 8) && ReadMemory(handler->mARMCompiledAddress,&offset,sizeof(offset)) && offset)
		{
			if (ARMCore::mDisassemble)
			{
				char buffer[256];
				sprintf(buffer,"Running Event %d for handler '%s'\n",event, handler->mOriginalResourceName.c_str());
				OutputDebugStringA(buffer);
			}
			SetPC(handler->mARMCompiledAddress + offset);
			SetRegister(0,event);
			SetRegister(4,mARMCosTableAddress);
			SetRegister(5,mARMSinTableAddress);
			// MPi: TODO: Need temp work area in R10
			Execute();
		}

		st++;
	}

	return true;
}

bool ROEditor::SendEventToWorldTemplate(System::SystemWorld *systemWorld, World *world, World::WorldTemplate * temp, const unsigned int event, const bool frame)
{
	if ((event == 4) && temp->mSysInitEventSent)
	{
		return false;
	}
	if ((event == 6) && temp->mInitEventSent)
	{
		return false;
	}
	if (event == 4)
	{
		temp->mSysInitEventSent = true;
	}
	else if (event == 6)
	{
		temp->mInitEventSent = true;
	}

	if (temp->mTemplate)
	{
		bool executed = false;
		size_t i;
		for( i = 0 ; i < temp->mTemplate->mHandlers.size() ; i++)
		{
			if (temp->mTemplate->mHandlers[i].mHandler && (temp->mTemplate->mHandlers[i].mHandler->mSize >= 8))
			{
				unsigned int offset;
				unsigned int handOffset = frame?0:4;
				if (ReadMemory(temp->mTemplate->mHandlers[i].mHandler->mARMCompiledAddress + handOffset,&offset,sizeof(offset)))
				{
					if (offset)
					{
						if (temp->mARMPrivateWords.size() != temp->mTemplate->mHandlers.size())
						{
							temp->mARMPrivateWords.resize(temp->mTemplate->mHandlers.size(),0);
						}
						if (ARMCore::mDisassemble)
						{
							char buffer[256];
							sprintf(buffer,"Template $%x Running Event %s:%d for '%s' handler %d='%s'\n",temp->mARMAddress,frame?"Frame":"IRQ",event,temp->mTemplate->mOriginalResourceName.c_str(), i, temp->mTemplate->mHandlers[i].mHandler->mOriginalResourceName.c_str());
							OutputDebugStringA(buffer);
						}

						if (temp->mTemplate->mHandlers[i].mHandler->mOriginalResourceName == "CarPos          ")
						{
							int a = 0;
						}

						SetPC(temp->mTemplate->mHandlers[i].mHandler->mARMCompiledAddress + offset);
						SetRegister(0,event);
						SetRegister(1,temp->mTemplate->mHandlers[i].mARMAddress);
						SetRegister(4,mARMCosTableAddress);
						SetRegister(5,mARMSinTableAddress);
						SetRegister(6,temp->mARMAddress);
						SetRegister(7,world->mARMAddress);		// World resource header
						if (systemWorld)
						{
							SetRegister(8,systemWorld->mARMAddress);
						}
						if (mCurrentSystem)
						{
							SetRegister(9,mCurrentSystem->mARMAddress);		// System resource header
						}
						SetRegister(11,0x10000000);	// The screen
						SetRegister(12,temp->mARMPrivateWords[i]);
						Execute();
						temp->mARMPrivateWords[i] = GetRegister(12);
						executed = true;
					}
				}
				else
				{
					assert(!"Handler data seems to be corrupt");
				}
			}
		}

		if (executed && frame && (event == 10))
		{
			// Copy positions back again
			// MPi: TODO: Do a proper transfer of this data
			ReadMemory(temp->mARMAddress + 12,&temp->mIPosition,3*4);
			ReadMemory(temp->mARMAddress + 12 + (3*4),&temp->mIRotation,3*4);
		}

	}
	return true;
}

bool ROEditor::SendEventToRunningObjects(const unsigned int event, const bool frame)
{
	if (mCurrentSystem)
	{
		std::list<System::SystemWorld>::iterator st = mCurrentSystem->mWorlds.begin();
		while (st != mCurrentSystem->mWorlds.end())
		{
			System::SystemWorld *systemWorld = &(*st);
			if (!systemWorld->mNoHandlerEvents)
			{
				World *world = systemWorld->mWorld;
				st++;
				if (world)
				{
					std::list<World::WorldTemplate*>::iterator st;
					st = world->mTemplates.begin();
					while (st != world->mTemplates.end())
					{
						World::WorldTemplate *temp = *st++;

						// If the event number is explicitly sent then assume we know what we are doing and send it without checks
						if ( (event == 4) || (event == 6) )
						{
							SendEventToWorldTemplate(systemWorld, world, temp, event, frame);
						}
						else
						{
							if (!temp->mSysInitEventSent)
							{
								SendEventToWorldTemplate(systemWorld, world, temp, 4);	// Send init
							}
							if (!temp->mInitEventSent)
							{
								SendEventToWorldTemplate(systemWorld, world, temp, 6);	// Send init
							}
							SendEventToWorldTemplate(systemWorld, world, temp, event, frame);
						}
					}
				}
			}
		}
	}
	return true;
}

void ROEditor::CreateARMResource(BaseItem *item, const char *name)
{
	item->mARMAddress = ARMHeapAlloc(32);
	WriteMemory(item->mARMAddress + 16,name, 16);

	mARMAddressToBaseItem.insert(std::pair<unsigned int,BaseItem *>(item->mARMAddress,item));
}

bool ROEditor::ResourceLess::operator()(const std::string &lhs, const std::string &rhs) const
{
	std::string rlhs(lhs);
	std::string rrhs(rhs);
	while (rlhs.size() < 16)
	{
		rlhs += ' ';
	}
	while (rrhs.size() < 16)
	{
		rrhs += ' ';
	}
	return _stricmp(rlhs.c_str(),rrhs.c_str()) < 0;
}

void ROEditor::PollEvents(void)
{
	if (!mNeedToSendSystemInits)
	{
		return;
	}

	mNeedToSendSystemInits = false;

	SendEventToRunningObjects(4);
	SendEventToRunningObjects(6);
}
