#include <stdlib.h>
#include "TAGGL/Inc/DataChunk.h"

using namespace TAGGL;

DataChunk::DataChunk() : mSize(0) , mData(0)
{
}

DataChunk::~DataChunk()
{
	free(mData);
}

void DataChunk::Allocate(const int size)
{
	free(mData);	// Free anything previously there
	mData = malloc(size);
	mSize = size;
}
