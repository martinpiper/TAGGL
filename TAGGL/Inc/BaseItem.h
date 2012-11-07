#ifndef _TAGGL_BASEITEM_H_
#define _TAGGL_BASEITEM_H_
#pragma once

#include <string>

namespace TAGGL
{

class BaseItem
{
public:
	BaseItem() : mARMAddress(0) , mARMCompiledAddress(0) , mARMLiveAddress(0) , mOriginalResourceTypeIndex(-1) {}
	virtual ~BaseItem() {}

	/// The ARM Address of the "Resource Header".
	// 00/03 = ptr to compiled resource data
	// 04/07 = ptr to live resource data       
	// 08/11 = internal use  (bounding box size if template resource)
	// 12/15 = internal use  
	// 16/31 = resource name (padded with spaces)
	unsigned int mARMAddress;
	unsigned int mARMCompiledAddress;
	unsigned int mARMLiveAddress;

	int mOriginalResourceTypeIndex;
	std::string mOriginalResourceName;
};

}

#endif
