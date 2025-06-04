#include "DataStructure/StackBufferedAllocator.h"
#include "Math/Math.h"
using namespace fragcore;

StackBufferedAllocator::StackBufferedAllocator(const StackBufferedAllocator &doublebuffer) : UIDObject(doublebuffer) {
	*this = doublebuffer;
}

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
	 this->m_stack[this->m_curStack].freeToMarker(marker);
}

void StackBufferedAllocator::next() { this->m_curStack = ~this->m_curStack & 0x1; }

StackAllocator *StackBufferedAllocator::getCurrentStack() { return &this->m_stack[this->m_curStack]; }

const StackAllocator *StackBufferedAllocator::getStack(int index) const {
	return (&this->m_stack[index]);
}

StackAllocator *StackBufferedAllocator::getStack(int index) {
	return (&this->m_stack[index]);
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
