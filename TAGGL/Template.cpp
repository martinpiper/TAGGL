#include "TAGGL/Inc/Template.h"
#include "TAGGL/Inc/Object.h"

using namespace TAGGL;

Template::Template() : mLighting(false)
{
}

Template::~Template()
{
}

void Template::Render(const float scaleX, const float scaleY, const float scaleZ)
{
	// MPi: TODO: Find the distance of the current matrix from the viewpoint and choose the object from the list
	mObjects[0].mObject->Render(scaleX, scaleY, scaleZ);
}

Template::TemplateObject::TemplateObject() : mResourceIndex(0) , mObject(0) , mIDistance(0)
{
}

Template::TemplateHandler::TemplateHandler() : mARMAddress(0) , mResourceIndex(0) , mHandler(0)
{
	mDataWords[0] = 0;
	mDataWords[1] = 0;
	mDataWords[2] = 0;
	mDataWords[3] = 0;
}
