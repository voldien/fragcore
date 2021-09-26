#include "Core/dataStructure/Queue.h"
#include <gtest/gtest.h>
using namespace fragcore;

TEST(DataStructure, Queue_Allocate_No_Throw) { ASSERT_NO_THROW(PoolAllocator<int> allocator(32)); }
