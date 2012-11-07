#ifndef _TAGGL_SYSTEM_H_
#define _TAGGL_SYSTEM_H_
#pragma once

#include <list>
#include "TAGGL/Inc/Types.h"
#include "TAGGL/Inc/BaseItem.h"

extern float gOverridePosX;
extern float gOverridePosY;
extern float gOverridePosZ;

extern float gOverrideRotX;
extern float gOverrideRotY;

namespace TAGGL
{

class World;

class System : public BaseItem
{
public:
	System();
	virtual ~System();

	void Render(void);

	struct SystemWorld
	{
		SystemWorld();

		int mResourceIndex;
		World *mWorld;

		unsigned int mARMAddress;

		IVector3 mIPositionOffset;
		int mGroupNumber;
		bool mMiscResource;
		bool mFilledRender;
		bool mIgnoreViewpoint;
		bool mBackdrop;
		bool mNoHandlerEvents;
	};
	std::list<SystemWorld> mWorlds;

	// Original Holograph data
	IVector3 mIViewPosition;
	IVector3 mIViewRotation;

	bool mLightSourceEnabled;
	unsigned char mLightSourceType;
	unsigned char mlightSourceShift;
	unsigned char mAmbientLevels[3];
	unsigned char mMaxLevels[3];
	IVector3 mILightPosition;
};

};

#endif
