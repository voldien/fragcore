#include <Core/dataStructure/StackAllactor.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(DataStructure, StackAllocator_Allocate_No_Throw) { ASSERT_NO_THROW(StackAllocator allocator(32)); }