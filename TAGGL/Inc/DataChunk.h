#ifndef _TAGGL_DATACHUNK_H_
#define _TAGGL_DATACHUNK_H_
#pragma once

#include "TAGGL/Inc/BaseItem.h"

namespace TAGGL
{

class DataChunk : public BaseItem
{
public:
	DataChunk();
	virtual ~DataChunk();

	void Allocate(const int size);

	int mSize;
	void *mData;
};

};

#endif
