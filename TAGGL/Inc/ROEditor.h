#ifndef _TAGGL_ROEDITOR_H_
#define _TAGGL_ROEDITOR_H_
#pragma once

#include <vector>
#include <map>

#include "RNPlatform/Inc/MessageHelper.h"
#include "TAGGL/Inc/DataChunk.h"
#include "TAGGL/Inc/ARMCore.h"
#include "TAGGL/Inc/World.h"
#include "TAGGL/Inc/System.h"

namespace TAGGL
{

extern unsigned int gRISCOSToRGB[256];

class Object;
class BaseItem;

struct ResourceItem
{
	enum
	{
		kSystem = 0,
		kWorld,
		kTemplate,
		kObject,
		kSprite,
		kFaceList,
		kHandler,
		kMiscData,
		kMaxResourceTypes,		// Must be the last one in the list for loaded resource types
		// Code defined types
		kWorldTemplate
	};

	ResourceItem() : mType(kMaxResourceTypes), mFileOffset(0), mItem(0), mLoadedByIteration(0), mOriginalIndex(0), mChunkSize(0) {}
	int mType;
	unsigned int mFileOffset;
	BaseItem *mItem;
	int mLoadedByIteration;
	int mOriginalIndex;
	int mChunkSize;
};

class ROEditor : public ARMCore
{
public:
	ROEditor();
	virtual ~ROEditor();

	bool ReadOutput(RNReplicaNet::MessageHelper &data);
	Object *ReadOutputObject(RNReplicaNet::MessageHelper &data);

	unsigned int ARMHeapAlloc(unsigned int size);

	void PollEvents(void);

	bool SendEventToAllHandlers(const unsigned int event , const unsigned int applicationHandle = 0);

	bool SendEventToWorldTemplate(System::SystemWorld *systemWorld, World *world, World::WorldTemplate * temp, const unsigned int event, const bool frame = true);

	bool SendEventToRunningObjects(const unsigned int event, const bool frame = true);

	struct ResourceLess
	{
		bool operator()(const std::string &lhs, const std::string &rhs) const;
	};

	typedef std::map<std::string,ResourceItem*,ResourceLess> ResourceIndex;
	ResourceIndex mResources[ResourceItem::kMaxResourceTypes];

	std::map<unsigned int,BaseItem *> mARMAddressToBaseItem;

	std::vector<Object *> mObjectsLoaded;

	System *mCurrentSystem;

	void SetRootDirectory(const std::string &path)
	{
		mRootDirectory = path;
	}

	unsigned int GetLoadedIteration(void) const;

protected:
	virtual bool CallbackTAGFunction(const unsigned int address);

	void CreateARMResource(BaseItem *item, const char *name);

	DataChunk mCosTable;
	DataChunk mSinTable;
	unsigned int mARMCosTableAddress;
	unsigned int mARMSinTableAddress;

	bool mNeedToSendSystemInits;

	std::string mRootDirectory;
};

};

#endif
