#include "Core/DataStructure/Queue.h"
#include <gtest/gtest.h>
using namespace fragcore;

TEST(Queue, Allocate_No_Throw) { ASSERT_NO_THROW(Queue<int> allocator(32)); }
