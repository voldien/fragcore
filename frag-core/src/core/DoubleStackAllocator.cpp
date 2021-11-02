#include "Core/dataStructure/DoubleBufferedAllocator.h"
#include <utility>
using namespace fragcore;

DoubleBufferedAllocator::DoubleBufferedAllocator() {
	this->m_curStack = 0;
	this->m_stack[0] = StackAllocator();
	this->m_stack[1] = StackAllocator();
}

DoubleBufferedAllocator::DoubleBufferedAllocator(const DoubleBufferedAllocator &doublebuffer) { *this = doublebuffer; }

DoubleBufferedAllocator::DoubleBufferedAllocator(DoubleBufferedAllocator &&other) {
	this->m_stack[0] = std::move(other.m_stack[0]);
	this->m_stack[1] = std::move(other.m_stack[1]);
	this->m_curStack = std::exchange(other.m_curStack, 0);
}

DoubleBufferedAllocator::DoubleBufferedAllocator(unsigned int sizeBytes) {
	this->m_curStack = 0;
	this->m_stack[0].alloc(sizeBytes);
	this->m_stack[1].alloc(sizeBytes);
}

DoubleBufferedAllocator::~DoubleBufferedAllocator() {
	this->getStack(0)->clear();
	this->getStack(1)->clear();
}

void DoubleBufferedAllocator::alloc(unsigned int sizeBytes) {
	this->m_stack[0].alloc(sizeBytes);
	this->m_stack[1].alloc(sizeBytes);
}

void DoubleBufferedAllocator::allocateAligned(unsigned int sizeBytes, int alignment) {
	sizeBytes += (sizeBytes % alignment);
	this->alloc(sizeBytes);
}

void DoubleBufferedAllocator::clear() { this->m_stack[this->m_curStack].clear(); }

unsigned int DoubleBufferedAllocator::getMarker() const { return this->m_stack[this->m_curStack].getMarker(); }

void *DoubleBufferedAllocator::fetch(unsigned int sizeBytes) {
	return this->m_stack[this->m_curStack].fetch(sizeBytes);
}

void DoubleBufferedAllocator::freeToMarker(unsigned int marker) {
	return this->m_stack[this->m_curStack].freeToMarker(marker);
}

void DoubleBufferedAllocator::swap() { this->m_curStack = ~this->m_curStack & 0x1; }

StackAllocator *DoubleBufferedAllocator::getCurrentStack() { return &this->m_stack[this->m_curStack]; }

StackAllocator *DoubleBufferedAllocator::getStack(int index) const { return (StackAllocator *)&this->m_stack[index]; }

DoubleBufferedAllocator &DoubleBufferedAllocator::operator=(const DoubleBufferedAllocator &alloc) {
	*this->getStack(0) = *alloc.getStack(0);
	*this->getStack(1) = *alloc.getStack(1);
	this->m_curStack = alloc.m_curStack;
	return *this;
}

DoubleBufferedAllocator &DoubleBufferedAllocator::operator=(DoubleBufferedAllocator &&alloc) {
	this->m_stack[0] = std::move(alloc.m_stack[0]);
	this->m_stack[1] = std::move(alloc.m_stack[1]);
	this->m_curStack = alloc.m_curStack;
	return *this;
}
