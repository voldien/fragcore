#include <Core/DataStructure/StackAllactor.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(StackAllocator, StackAllocator_Allocate_No_Throw) { ASSERT_NO_THROW(StackAllocator allocator(32)); }