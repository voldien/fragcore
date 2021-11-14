#include "Core/dataStructure/StackAllactor.h"
#include <cstring>
#include <malloc.h>
#include <utility>
using namespace fragcore;

StackAllocator::StackAllocator() {
	this->mData = nullptr;
	this->mMarker = 0;
	this->mSize = 0;
}

StackAllocator::StackAllocator(const StackAllocator &stack) {
	this->mSize = 0;
	*this = stack;
}
StackAllocator::StackAllocator(StackAllocator &&other) {
	this->mData = std::exchange(other.mData, nullptr);
	this->mMarker = std::exchange(other.mMarker, 0);

	this->mSize = std::exchange(other.mSize, 0);
}

StackAllocator::StackAllocator(size_t stackSizeBytes) {
	this->mMarker = 0;
	this->mData = nullptr;
	this->mSize = 0;
	this->alloc(stackSizeBytes);
}

StackAllocator::~StackAllocator() { free(this->mData); }

void *StackAllocator::alloc(size_t sizeBytes) {
	// TODO determine if allocaing smaller memory.
	if (sizeBytes < getSize()) {
	}
	mSize = sizeBytes;
	mData = realloc(mData, sizeBytes);
	if (!mData) {
		// VDDebug::errorLog("Failed to allocate {} kb.\n", ( sizeBytes / 1024 ) );
	}
	return mData;
}

size_t StackAllocator::getSize() const { return this->mSize; }

void *StackAllocator::allocateAligned(size_t sizeBytes, int alignment) {
	sizeBytes += (sizeBytes % alignment);
	return this->alloc(sizeBytes);
}

void StackAllocator::clear() { this->mMarker = 0; }

size_t StackAllocator::getMarker() const { return this->mMarker; }

void *StackAllocator::fetch(size_t sizeBytes) {
	void *p = ((uint8_t *)this->mData) + getMarker();
	this->mMarker += sizeBytes;
	return p;
}

void StackAllocator::freeToMarker(size_t marker) { this->mMarker = marker; }

StackAllocator &StackAllocator::operator=(const StackAllocator &alloc) {
	this->alloc(alloc.getSize());
	memcpy(this->mData, alloc.mData, alloc.getMarker());
	this->mMarker = alloc.mMarker;
	return *this;
}
