#include<Core/dataStructure/Stack.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(DataStructure, Stack_Allocate_No_Throw) { ASSERT_NO_THROW(Stack<int> allocator(32)); }

TEST(DataStructure, Stack_Allocate_No_Throw__) {
	Stack<int> allocator(32);
	ASSERT_EQ(allocator.getReserved(), 32);

	ASSERT_EQ(allocator.getSize(), 0);

	allocator.push(2);
	int a = allocator.pop();
	int b = allocator.push(0);
	allocator.clear();

	ASSERT_EQ(allocator.getSize(), 0);
}