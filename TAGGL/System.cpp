#include <GL/GLUT.h>
#include "TAGGL/Inc/System.h"
#include "TAGGL/Inc/World.h"

float gOverridePosX = 0.0f;
float gOverridePosY = 0.0f;
float gOverridePosZ = 0.0f;

float gOverrideRotX = 0.0f;
float gOverrideRotY = 0.0f;

using namespace TAGGL;
System::System() : mIViewPosition(0,0,0) , mIViewRotation(0,0,0) , mLightSourceEnabled(0) , mLightSourceType(0) , mlightSourceShift(0) ,
					mILightPosition(0,0,0)
{
}

System::~System()
{
}

void System::Render(void)
{

	std::list<SystemWorld>::iterator st;
	st = mWorlds.begin();
	while (st != mWorlds.end())
	{
		SystemWorld &world = *st++;

		glLoadIdentity();

		if (!world.mIgnoreViewpoint)
		{
			glRotatef(float(mIViewRotation.mZ) / kAngCount, 0.0, 0.0, 1.0);
			if (!world.mBackdrop)
			{
				glRotatef(gOverrideRotX - (float(mIViewRotation.mX) / kAngCount), 1.0, 0.0, 0.0);
				glRotatef(gOverrideRotY - (float(mIViewRotation.mY) / kAngCount), 0.0, 1.0, 0.0);
				glTranslatef(gOverridePosX - (float(mIViewPosition.mX) / 256.0f) , gOverridePosY - (float(mIViewPosition.mY) / 256.0f) , gOverridePosZ + (float(mIViewPosition.mZ) / 256.0f));
			}
		}

		if (world.mFilledRender)
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}
		else
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		}
		glPushMatrix();
		glTranslatef(float(world.mIPositionOffset.mX) / 256.0f,float(world.mIPositionOffset.mY) / 256.0f,-float(world.mIPositionOffset.mZ) / 256.0f);
		if (world.mWorld)
		{
			world.mWorld->Render();
		}
		glPopMatrix();
	}
}

System::SystemWorld::SystemWorld() : mResourceIndex(0) , mWorld(0) , mARMAddress(0) , mIPositionOffset(0,0,0) , mGroupNumber(0) , mMiscResource(false) ,
										mFilledRender(false) , mIgnoreViewpoint(false) , mBackdrop(false) , mNoHandlerEvents(false)
{
}
