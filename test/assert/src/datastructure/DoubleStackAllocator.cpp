#include <Core/dataStructure/DoubleBufferedAllocator.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(DoubleBufferedAllocator, DoubleStackAllocator_Allocate_No_Throw) {
	ASSERT_NO_THROW(DoubleBufferedAllocator allocator(32));
}

TEST(DoubleBufferedAllocator, Stack_Allocate_No_Throw__) {
	DoubleBufferedAllocator allocator(32);
	// ASSERT_EQ(allocator.getReserved(), 32);

	// ASSERT_EQ(allocator.getSize(), 0);

	// allocator.push(2);
	// int a = allocator.pop();
	// int b = allocator.push(0);
	// allocator.clear();

	// ASSERT_EQ(allocator.getSize(), 0);
}