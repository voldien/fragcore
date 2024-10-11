#include <Core/DataStructure/Stack.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(Stack, Create_Default_Primitive_Type_No_Throw) { ASSERT_NO_THROW(Stack<int> allocator(32)); }

// TEST(Stack, Create_Default_Primitive_Type_No_Throw) { ASSERT_NO_THROW(Stack<int> allocator(32)); }

TEST(Stack, Stack_Allocate_No_Throw__) {
	Stack<int> allocator(32);
	ASSERT_EQ(allocator.getReserved(), 32);

	ASSERT_EQ(allocator.getSize(), 0);

	const int pushV0 = 2;
	const int pushV1 = 0;

	allocator.push(2);
	int a = allocator.pop();
	ASSERT_EQ(pushV0, a);
	int b = allocator.push(pushV1);
	ASSERT_EQ(pushV1, b);
	allocator.clear();

	ASSERT_EQ(allocator.getSize(), 0);
}