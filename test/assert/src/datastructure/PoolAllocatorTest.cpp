#include "Core/DataStructure/PoolAllocator.h"
#include <gtest/gtest.h>
using namespace fragcore;


TEST(PoolAllocator, PoolAllocator_Allocate_No_Throw) { ASSERT_NO_THROW(PoolAllocator<int> allocator(32)); }

TEST(PoolAllocator, PoolAllocator_obtain) {
	PoolAllocator<int> allocator(32);
	allocator.obtain();
}

TEST(PoolAllocator, PoolAllocator) {
	PoolAllocator<int> allocator(32);
	std::vector<int *> pointers;

	const int s = allocator.reserved();
	for (int i = 0; i < s; i++) {
		int *p = allocator.obtain();
		pointers.push_back(p);
	}

	for (int i = 0; i < pointers.size(); i++) {
		allocator.Return(pointers[i]);
	}

	allocator.resize(allocator.reserved() * 2);
}
