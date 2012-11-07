#ifndef _TAGGL_WORLD_H_
#define _TAGGL_WORLD_H_
#pragma once

#include <list>
#include <vector>
#include "TAGGL/Inc/Types.h"
#include "TAGGL/Inc/BaseItem.h"

namespace TAGGL
{

class Template;

class World : public BaseItem
{
public:
	World();
	virtual ~World();

	void Render(void);

	struct WorldTemplate
	{
		WorldTemplate();

		unsigned int mARMAddress;
		std::vector<unsigned int> mARMPrivateWords;
		bool mSysInitEventSent;
		bool mInitEventSent;

		// Original Holograph data
		int mResourceIndex;
		Template *mTemplate;
		IVector3 mIPosition;
		IVector3 mIRotation;
		IVector3 mIScale;
		int mICollisionDistance;
		int mDataWords[2];
	};

	std::list<WorldTemplate> mTemplates;
};

};

#endif