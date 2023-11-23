#include "Core/dataStructure/StackBufferedAllocator.h"
#include "Core/Math.h"
#include <utility>
using namespace fragcore;

// StackBufferedAllocator::StackBufferedAllocator(size_t nrStack) {
// 	this->m_curStack = 0;
// 	this->m_stack[0] = StackAllocator();
// 	this->m_stack[1] = StackAllocator();
// }

StackBufferedAllocator::StackBufferedAllocator(const StackBufferedAllocator &doublebuffer) { *this = doublebuffer; }

StackBufferedAllocator::StackBufferedAllocator(StackBufferedAllocator &&other) {
	this->m_stack[0] = std::move(other.m_stack[0]);
	this->m_stack[1] = std::move(other.m_stack[1]);
	this->m_curStack = std::exchange(other.m_curStack, 0);
}

StackBufferedAllocator::StackBufferedAllocator(size_t sizeBytes) {
	this->m_curStack = 0;
	this->m_stack[0].alloc(sizeBytes);
	this->m_stack[1].alloc(sizeBytes);
}

StackBufferedAllocator::~StackBufferedAllocator() {
	this->getStack(0)->clear();
	this->getStack(1)->clear();
}

void StackBufferedAllocator::alloc(size_t sizeBytes) {
	this->m_stack[0].alloc(sizeBytes);
	this->m_stack[1].alloc(sizeBytes);
}

void StackBufferedAllocator::allocateAligned(size_t sizeBytes, int alignment) {
	size_t allocateSize = Math::align<size_t>(sizeBytes, alignment);
	this->alloc(allocateSize);
}

void StackBufferedAllocator::clear() noexcept { this->m_stack[this->m_curStack].clear(); }

size_t StackBufferedAllocator::getMarker() const noexcept { return this->m_stack[this->m_curStack].getMarker(); }

void *StackBufferedAllocator::fetch(size_t sizeBytes) { return this->m_stack[this->m_curStack].fetch(sizeBytes); }

void StackBufferedAllocator::freeToMarker(unsigned int marker) {
	return this->m_stack[this->m_curStack].freeToMarker(marker);
}

void StackBufferedAllocator::swap() { this->m_curStack = ~this->m_curStack & 0x1; }

StackAllocator *StackBufferedAllocator::getCurrentStack() { return &this->m_stack[this->m_curStack]; }

const StackAllocator *StackBufferedAllocator::getStack(int index) const {
	return static_cast<const StackAllocator *>(&this->m_stack[index]);
}

StackAllocator *StackBufferedAllocator::getStack(int index) {
	return static_cast<StackAllocator *>(&this->m_stack[index]);
}

StackBufferedAllocator &StackBufferedAllocator::operator=(const StackBufferedAllocator &alloc) {
	*this->getStack(0) = *alloc.getStack(0);
	*this->getStack(1) = *alloc.getStack(1);
	this->m_curStack = alloc.m_curStack;
	return *this;
}

StackBufferedAllocator &StackBufferedAllocator::operator=(StackBufferedAllocator &&alloc) {
	this->m_stack[0] = std::move(alloc.m_stack[0]);
	this->m_stack[1] = std::move(alloc.m_stack[1]);
	this->m_curStack = alloc.m_curStack;
	return *this;
}
