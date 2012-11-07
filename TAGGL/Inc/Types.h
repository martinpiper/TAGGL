#ifndef _TAGGL_TYPES_H_
#define _TAGGL_TYPES_H_
#pragma once


namespace TAGGL
{

enum
{
	kAngCount = 360
};

struct FVector3
{
	FVector3() {}
	FVector3(const float x, const float y, const float z) : mX(x) , mY(y) , mZ(z) { }

	float mX, mY, mZ;
};

struct IVector3
{
	IVector3() {}
	IVector3(const int x, const int y, const int z) : mX(x) , mY(y) , mZ(z) { }

	int mX, mY, mZ;
};

}

#endif
