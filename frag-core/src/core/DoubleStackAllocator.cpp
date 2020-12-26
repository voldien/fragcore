#include"Core/dataStructure/DoubleBufferedAllocator.h"
using namespace fragcore;

DoubleBufferedAllocator::DoubleBufferedAllocator(void)
{
	this->m_curStack = 0;
	this->m_stack[0] = StackAllocator();
	this->m_stack[1] = StackAllocator();
}

DoubleBufferedAllocator::DoubleBufferedAllocator(const DoubleBufferedAllocator &doublebuffer)
{
	*this = doublebuffer;
}

DoubleBufferedAllocator::DoubleBufferedAllocator(unsigned int sizeBytes)
{
	this->m_curStack = 0;
	this->m_stack[0].alloc(sizeBytes);
	this->m_stack[1].alloc(sizeBytes);
}

DoubleBufferedAllocator::~DoubleBufferedAllocator(void)
{
	this->getStack(0)->clear();
	this->getStack(1)->clear();
}

void DoubleBufferedAllocator::alloc(unsigned int sizeBytes)
{
	this->m_stack[0].alloc(sizeBytes);
	this->m_stack[1].alloc(sizeBytes);
}

void DoubleBufferedAllocator::allocateAligned(unsigned int sizeBytes, int alignment)
{
	sizeBytes += (sizeBytes % alignment);
	this->alloc(sizeBytes);
}

void DoubleBufferedAllocator::clear(void)
{
	this->m_stack[this->m_curStack].clear();
}

unsigned int DoubleBufferedAllocator::getMarker(void) const
{
	return this->m_stack[this->m_curStack].getMarker();
}

void *DoubleBufferedAllocator::fetch(unsigned int sizeBytes)
{
	return this->m_stack[this->m_curStack].fetch(sizeBytes);
}

void DoubleBufferedAllocator::freeToMarker(unsigned int marker)
{
	return this->m_stack[this->m_curStack].freeToMarker(marker);
}

void DoubleBufferedAllocator::swap(void)
{
	this->m_curStack = ~this->m_curStack & 0x1;
}

StackAllocator *DoubleBufferedAllocator::getCurrentStack(void)
{
	return &this->m_stack[this->m_curStack];
}

StackAllocator *DoubleBufferedAllocator::getStack(int index) const
{
	return (StackAllocator *)&this->m_stack[index];
}

DoubleBufferedAllocator &DoubleBufferedAllocator::operator=(const DoubleBufferedAllocator &alloc)
{
	*this->getStack(0) = *alloc.getStack(0);
	*this->getStack(1) = *alloc.getStack(1);
	this->m_curStack = alloc.m_curStack;
	return *this;
}
