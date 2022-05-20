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

StackAllocator::StackAllocator(size_t stackSizeBytes) : StackAllocator() { this->alloc(stackSizeBytes); }

StackAllocator::~StackAllocator() { free(this->mData); }

void *StackAllocator::alloc(size_t sizeBytes) {

	this->mSize = sizeBytes;
	this->mData = realloc(mData, sizeBytes);
	if (!this->mData) {
		throw RuntimeException("Failed to allocate {} kb.", (sizeBytes / 1024));
	}
	return mData;
}

size_t StackAllocator::getSize() const { return this->mSize; }

void *StackAllocator::allocateAligned(size_t sizeBytes, int alignment) {
	sizeBytes += (alignment - (sizeBytes % alignment));
	return this->alloc(sizeBytes);
}

void StackAllocator::clear() { this->mMarker = 0; }

size_t StackAllocator::getMarker() const { return this->mMarker; }

void *StackAllocator::fetch(size_t sizeBytes) {
	uint8_t *p = static_cast<uint8_t *>(this->mData) + getMarker();
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
