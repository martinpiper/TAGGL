#include <stdlib.h>
#include <assert.h>
#include <GL/glut.h>
#include <windows.h>
#include <algorithm>
#include "RNPlatform/Inc/MessageHelper.h"
#include "RNPlatform/Inc/SysTime.h"
#include "TAGGL/Inc/Object.h"
#include "TAGGL/Inc/ROEditor.h"
#include "TAGGL/Inc/System.h"
#include "TAGGL/Inc/World.h"
#include "TAGGL/Inc/ARMCore.h"
#include "TAGGL/Inc/DataChunk.h"

static GLfloat spin = 0.0;
static GLuint sTexture = -1;

using namespace RNReplicaNet;
using namespace TAGGL;

std::string appPath;
ROEditor gGame;

size_t toDisplay = 0;
static volatile bool sDisplayPosted = false;

void display(void)
{
	if (!sDisplayPosted)
	{
		// Waste some time, we don't want to be here
		Sleep(1);
		return;
	}
	sDisplayPosted = false;
	gGame.InitStack();
	gGame.PollEvents();
	gGame.SendEventToRunningObjects(10);		// Pre-frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
	gGame.mCurrentSystem = 0;	// Enable this line to just render models.
	glLoadIdentity();
	glTranslatef(0.0f, -10.0, -50.0f);
	glPushMatrix();
//	glRotatef(spin / 8.0f, 0.0, 0.0, 1.0);
//	glRotatef(spin / 4.0f, 1.0, 0.0, 0.0);
	glRotatef(spin, 0.0, 1.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);

//	glRectf(-25.0, -25.0, 25.0, 25.0);
#if 0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex2f(-15,15);
	glTexCoord2f(1,1);
	glVertex2f(15,15);
	glTexCoord2f(1,0);
	glVertex2f(15,-15);
	glTexCoord2f(0,0);
	glVertex2f(-15,-15);
	glEnd();
	glDisable(GL_TEXTURE_2D);
#endif
	if (!gGame.mObjectsLoaded.empty())
	{
//		sObjects[toDisplay]->Render(0.3f);
		gGame.mObjectsLoaded[toDisplay]->Render(1.0f);
	}

	glPopMatrix();
#else

	if (gGame.mCurrentSystem)
	{
		gGame.mCurrentSystem->Render();
	}
	else if (!gGame.mResources[ResourceItem::kWorld].empty())
	{
		glLoadIdentity();
//		glTranslatef(0.0f , -100.0f, -750.0f);
//		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(gOverrideRotX, 1.0f, 0.0f, 0.0f);
		glRotatef(gOverrideRotY, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f + gOverridePosX, -5.0f + gOverridePosY, 0.0f + gOverridePosZ);

		ROEditor::ResourceIndex::iterator st;
		st = gGame.mResources[ResourceItem::kWorld].begin();
		while (st != gGame.mResources[ResourceItem::kWorld].end() )
		{
			World *world = (World*) st->second->mItem;
			st++;
			if (world)
			{
				world->Render();
			}
		}
	}
#endif

	glutSwapBuffers();
}

static SysTime sTime;

void spinDisplay(void)
{
	static unsigned int numFramesElapsed = 0;
	// Clamp frame update to no more than 60 fps
	unsigned int wantFrames = (unsigned int)(sTime.FloatTime() * 60.0f);

	if (numFramesElapsed == 0)
	{
		numFramesElapsed = wantFrames - 1;
	}

	if ( wantFrames > numFramesElapsed )
	{
		while (numFramesElapsed < wantFrames)
		{
			numFramesElapsed++;
			gGame.SendEventToRunningObjects(0,false);	// IRQ: Tick

			// If there is any kind of debugging enabled then only process one frame at a time
			if (ARMCore::mDebuggingEnabled || ARMCore::mDisassemble)
			{
				numFramesElapsed = wantFrames;
			}
		}

		spin = spin + 1;

		sDisplayPosted = true;
		glutPostRedisplay();
	}
	else
	{
		// Waste some time, we don't want to be here
		Sleep(1);
	}
}

void init(void) 
{
	GLsizei sizeX = 2 , sizeY = 2;
	int data[4];

	glClearColor (0.1f, 0.1f, 0.1f, 0.0);
	glShadeModel (GL_FLAT);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glGenTextures(1, &sTexture);
	glBindTexture(GL_TEXTURE_2D, sTexture);

	// Without any mip maps, forces soft rendering
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	// Sharp
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	// With mip maps, soft
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Stop texture coords going beyond 0.0 and 1.0
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	data[0] = 0xffff0000;
	data[1] = 0xff00ff00;
	data[2] = 0xff0000ff;
	data[3] = 0xffffffff;
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sizeX, sizeY, GL_RGBA, GL_UNSIGNED_BYTE, data);


#if 1
	// Read data files from the command line
	DynamicMessageHelper file;

	size_t pos;
	while ( (pos = appPath.find_first_of('\"')) == 0)
	{
		appPath.erase(pos , 1);
		pos = appPath.find_first_of('\"');
	}
	if ( !appPath.empty() && (appPath.c_str()[appPath.length()-1] == '\"') )
	{
		appPath.erase(appPath.length()-1);
	}

	std::string rootDirectory;
	if ((pos = appPath.find_last_of('\\')) != std::string::npos)
	{
		rootDirectory = appPath.substr(0, pos+1);
		gGame.SetRootDirectory(rootDirectory);
	}

	// First try to read the file as Holograph output data
	bool ret = file.Read(appPath.c_str(),true);
	assert(ret && "No file loaded");
	if (ret)
	{
		// It loaded so try to parse it
		gGame.ReadOutput(file);
	}

	// If no startup system was found then try to fudge one instead
#if 1
	if (!gGame.mCurrentSystem && !gGame.mResources[ResourceItem::kWorld].empty())
	{
		gGame.mCurrentSystem = new System;
		ROEditor::ResourceIndex::iterator st = gGame.mResources[ResourceItem::kWorld].begin();
		while (st != gGame.mResources[ResourceItem::kWorld].end())
		{
			System::SystemWorld systemWorld;
			systemWorld.mFilledRender = true;
			systemWorld.mWorld = (World*)st->second->mItem;
			gGame.mCurrentSystem->mWorlds.push_back(systemWorld);
			st++;
		}
	}
#endif
	if (!gGame.mCurrentSystem && !gGame.mResources[ResourceItem::kSystem].empty())
	{
		gGame.mCurrentSystem = (System *) gGame.mResources[ResourceItem::kSystem].begin()->second->mItem;
	}
#else
	// Force what files to load from BHP to try to get this running. The BHP loader code seems to want to merge these three system files and run the track, so this is what we shall attempt to do.
	DynamicMessageHelper file;
	file.Read("Data\\BHPOutput\\Control",true);
	gGame.ReadOutput(file);
	file.Read("Data\\BHPOutput\\Titles", true);
	gGame.ReadOutput(file);
	file.Read("Data\\BHPOutput\\Game",true);
	gGame.ReadOutput(file);
	file.Read("Data\\BHPOutput\\Track2",true);
	gGame.ReadOutput(file);
#endif

	gGame.SendEventToAllHandlers(0);
	gGame.PollEvents();
}

void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(-50.0, 50.0, -50.0, 50.0, 0.0, 100.0);
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,1.0f,1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


static bool gotLast = false;
static bool leftDown = false;
static bool middleDown = false;
static bool rightDown = false;

void mouse(int button, int state, int x, int y) 
{
#if 0
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			glutIdleFunc(spinDisplay);
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
		{
			glutIdleFunc(NULL);
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			toDisplay++;
			if (toDisplay >= gGame.mObjectsLoaded.size())
			{
				toDisplay = 0;
			}
		}
		break;

	default:
		break;
	}
#else

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		leftDown = false;
		if (state == GLUT_DOWN)
		{
			gotLast = false;
			leftDown = true;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		middleDown = false;
		if (state == GLUT_DOWN)
		{
			gotLast = false;
			middleDown = true;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		rightDown = false;
		if (state == GLUT_DOWN)
		{
			gotLast = false;
			rightDown = true;
		}
		break;

	default:
		break;
	}
#endif
}

void mouseMove(int x, int y) 
{
	static int lastX, lastY;
	if (!gotLast)
	{
		gotLast = true;
		lastX = x;
		lastY = y;
		return;
	}

	if (leftDown)
	{
		gOverridePosX += x - lastX;
		gOverridePosZ += y - lastY;
	}

	if (middleDown)
	{
		gOverridePosX += x - lastX;
		gOverridePosY += y - lastY;
	}

	if (rightDown)
	{
		gOverrideRotY += x - lastX;
		gOverrideRotX += y - lastY;
	}

	lastX = x;
	lastY = y;
}

int WINAPI WinMain (
	__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine,
	__in int nShowCmd
	)
{
	appPath = lpCmdLine;

#if 0
	DynamicMessageHelper file;
	if (file.Read("data\\TestAnimHandlers\\BinHandlers\\SpinXYZ,ffd",true))
	{
		ARMCore armTest;
		DataChunk memory;
		memory.Allocate(file.GetBufferSize());
		memcpy(memory.mData,file.GetBuffer(),file.GetBufferSize());
		armTest.WriteMemory(0x10000,file.GetBuffer(),file.GetBufferSize());

		// Get the frame code offset from the loaded handler
		unsigned int offset;
		file >> offset;
		armTest.SetPC(0x80000000 | 0x10000 + offset);
		armTest.SetRegister(0,6);
		armTest.Execute();
	}
#endif


	int argc = 1;
	char **argv = (char **)calloc(argc,sizeof(char*));
	argv[0] = "Moo";
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (640, 480); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutIdleFunc(spinDisplay);
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutMainLoop();
	return 0;
}
