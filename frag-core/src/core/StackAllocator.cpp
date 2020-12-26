#include "Core/dataStructure/StackAllactor.h"
#include <malloc.h>
#include <cstring>
using namespace fragcore;

StackAllocator::StackAllocator(void)
{
	this->mData = NULL;
	this->mMarker = NULL;
	this->mSize = 0;
}

StackAllocator::StackAllocator(const StackAllocator &stack)
{
	this->mSize = 0;
	*this = stack;
}

StackAllocator::StackAllocator(unsigned int stackSizeBytes)
{
	this->mMarker = 0;
	this->mData = NULL;
	this->mSize = 0;
	this->alloc(stackSizeBytes);
}

StackAllocator::~StackAllocator(void)
{
	free(this->mData);
}

void *StackAllocator::alloc(unsigned int sizeBytes)
{
	//TODO determine if allocaing smaller memory.
	if(sizeBytes < getSize()){

	}
	mSize = sizeBytes;
	mData = realloc(mData, sizeBytes);
	if(!mData){
		//VDDebug::errorLog("Failed to allocate %d kb.\n", ( sizeBytes / 1024 ) );
	}
	return mData;
}

unsigned int StackAllocator::getSize(void) const
{
	return this->mSize;
}

void *StackAllocator::allocateAligned(unsigned int sizeBytes, int alignment)
{
	sizeBytes += (sizeBytes % alignment);
	return this->alloc(sizeBytes);
}

void StackAllocator::clear(void)
{
	this->mMarker = 0;
}

unsigned int StackAllocator::getMarker(void) const
{
	return this->mMarker;
}

void *StackAllocator::fetch(unsigned int sizeBytes)
{
	void* p = this->mData + getMarker();
	this->mMarker += sizeBytes;
	return p;
}

void StackAllocator::freeToMarker(unsigned int marker)
{
	this->mMarker = marker;
}

StackAllocator &StackAllocator::operator=(const StackAllocator &alloc)
{
	this->alloc(alloc.getSize());
	memcpy(this->mData, alloc.mData, alloc.getMarker());
	return *this;
}
