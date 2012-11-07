#ifndef _TAGGL_TEMPLATE_H_
#define _TAGGL_TEMPLATE_H_
#pragma once

#include <vector>
#include "TAGGL/Inc/Types.h"
#include "TAGGL/Inc/BaseItem.h"

namespace TAGGL
{

class Object;
class DataChunk;

class Template : public BaseItem
{
public:
	Template();
	virtual ~Template();

	void Render(const float scaleX = 1.0f, const float scaleY = 1.0f, const float scaleZ = 1.0f);

	struct TemplateObject
	{
		TemplateObject();

		// Original Holograph data
		int mResourceIndex;
		Object *mObject;
		int mIDistance;
	};

	bool mLighting;
	std::vector<TemplateObject> mObjects;

	struct TemplateHandler
	{
		TemplateHandler();

		unsigned int mARMAddress;

		// Original Holograph data
		int mResourceIndex;
		DataChunk *mHandler;
		int mDataWords[4];
	};

	std::vector<TemplateHandler> mHandlers;
};

};

#endif
