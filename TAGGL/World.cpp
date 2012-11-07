#include <GL/GLUT.h>
#include "TAGGL/Inc/World.h"
#include "TAGGL/Inc/Template.h"
#include "TAGGL/Inc/Object.h"

using namespace TAGGL;

World::World()
{
}

World::~World()
{
}

void World::Render(void)
{
	std::list<WorldTemplate>::iterator st;
	st = mTemplates.begin();
	while (st != mTemplates.end() )
	{
		WorldTemplate &temp = *st++;
		if (temp.mTemplate && !temp.mTemplate->mObjects.empty() && temp.mTemplate->mObjects[0].mObject)
		{
			glPushMatrix();
			glTranslatef(float(temp.mIPosition.mX) / 256.0f , float(temp.mIPosition.mY) / 256.0f, -float(temp.mIPosition.mZ) / 256.0f);
			glRotatef(-(360.0f * float(temp.mIRotation.mY)) / float(kAngCount), 0.0, 1.0, 0.0);
			glRotatef(-(360.0f * float(temp.mIRotation.mX)) / float(kAngCount), 1.0, 0.0, 0.0);
			glRotatef(-(360.0f * float(temp.mIRotation.mZ)) / float(kAngCount), 0.0, 0.0, 1.0);
			temp.mTemplate->Render(float(temp.mIScale.mX) / 256.0f , float(temp.mIScale.mY) / 256.0f, float(temp.mIScale.mZ) / 256.0f);
			glPopMatrix();
		}
	}
}

World::WorldTemplate::WorldTemplate() : mARMAddress(0) , mSysInitEventSent(false) , mInitEventSent(false) ,
										mResourceIndex(0) , mTemplate(0) , mIPosition(0,0,0) , mIRotation(0,0,0) ,
										mIScale(0,0,0) , mICollisionDistance(0)
{
	mDataWords[0] = 0;
	mDataWords[1] = 0;
}
