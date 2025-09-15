#include "DataStructure/MemoryAddress.h"
#include "Math/Math.h"

using namespace fragcore;

MemoryAddress::MemoryAddress(const size_t size, const size_t offset) {
	this->mSize = size;
	this->baseOffset = offset;
	this->clear();
}

size_t MemoryAddress::alloc(const size_t sizeBytes) noexcept {
	size_t current_marker = this->mMarker;
	this->mMarker += sizeBytes;
	return current_marker;
}

size_t MemoryAddress::allocateAligned(const size_t sizeBytes, const unsigned int alignment) noexcept {

	const size_t requested_byte_size = fragcore::Math::align<size_t>(sizeBytes, alignment);
	size_t current_marker = this->mMarker;
	this->mMarker += requested_byte_size;

	return current_marker;
}

void MemoryAddress::clear() noexcept { this->mMarker = 0; }
void MemoryAddress::freeToMarker(const size_t marker) { this->mMarker = marker; }