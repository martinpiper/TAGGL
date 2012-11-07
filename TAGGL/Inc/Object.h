#ifndef _TAGGL_OBJECT_H_
#define _TAGGL_OBJECT_H_
#pragma once

#include "TAGGL/Inc/Types.h"
#include "TAGGL/Inc/BaseItem.h"
#include <string>
#include <vector>
#include <list>
#include <map>

namespace TAGGL
{

struct ICollision
{
	int mVertexIndex;
	int mSize;
};

struct ITextureCoords
{
	unsigned short mX[4] , mY[4];
};

struct IFace
{
	IFace();
	int mType;
	int mDesc1 , mDesc2;

	std::vector<int> mVertList;
	ITextureCoords mTexture;
	bool mIsTextured;
	bool mIsWireframe;
};

struct FFace
{
	FFace();
	// Optimised for triangles
	// MPi: TODO: Instead of verts per triangle use glGenLists, glNewList and GL_COMPILE for the base object
	FVector3 mVertList[3];
	float mTexture[3][2];
	float mColourR , mColourG , mColourB;
	unsigned int mTextureHandle;
	bool mIsTextured;
	bool mIsScaledSprite;
	bool mIsLine;
	float mScaledSpriteSize;
};

class Object : public BaseItem
{
public:
	Object();
	virtual ~Object();

	void ROOutputToLive(void);

	void Render(const float scaleX = 1.0f, const float scaleY = 1.0f, const float scaleZ = 1.0f);

	// Original Holograph data
	std::vector<IVector3> mIVectors;
	std::list<ICollision> mICollision;
	std::list<IFace> mIFaces;

	// Live data
	std::vector<FFace> mFaces;
};

};

#endif
