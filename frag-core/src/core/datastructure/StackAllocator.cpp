#include "Core/Math.h"
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

StackAllocator::StackAllocator(const size_t stackSizeBytes) : StackAllocator() { this->alloc(stackSizeBytes); }

StackAllocator::~StackAllocator() { free(this->mData); }

void *StackAllocator::alloc(size_t sizeBytes) {

	this->mSize = sizeBytes;
	this->mData = realloc(mData, sizeBytes);
	if (!this->mData) {
		throw RuntimeException("Failed to allocate {} kb.", (sizeBytes / 1024));
	}
	return mData;
}

size_t StackAllocator::getSize() const noexcept { return this->mSize; }

void *StackAllocator::allocateAligned(size_t sizeBytes, unsigned int alignment) {
	size_t allocateSize = Math::align<size_t>(sizeBytes, alignment);
	return this->alloc(allocateSize);
}

void StackAllocator::clear() noexcept { this->mMarker = 0; }

size_t StackAllocator::getMarker() const noexcept { return this->mMarker; }

void *StackAllocator::fetch(const size_t sizeBytes) {
	/*	TODO: validate. request.	*/
	uint8_t *pdata = static_cast<uint8_t *>(this->mData) + getMarker();
	this->mMarker += sizeBytes;
	return pdata;
}

void StackAllocator::freeToMarker(const size_t marker) { this->mMarker = marker; }

StackAllocator &StackAllocator::operator=(const StackAllocator &alloc) {
	this->alloc(alloc.getSize());
	memcpy(this->mData, alloc.mData, alloc.getMarker());
	this->mMarker = alloc.mMarker;
	return *this;
}
