#include<gtest/gtest.h>
#include"Utils/TextureUtil.h"
#include"Core/dataStructure/PoolAllocator.h"
#include <Core/dataStructure/ITree.h>
#include<Core/dataStructure/StackAllactor.h>
#include<Core/dataStructure/DoubleBufferedAllocator.h>
#include <Core/dataStructure/Queue.h>
using namespace fragcore;


TEST(DataStructure, StackAllocator){
	const int s_size = 128;
	StackAllocator stackAllocator(s_size);
	/*	Test initial state.	*/
	ASSERT_EQ(stackAllocator.getSize(), s_size);
	ASSERT_EQ(stackAllocator.getMarker(), 0);

	/*	*/
	void *p = stackAllocator.allocateAligned(84, 8);
	//ASSERT_NE(p, NULL);
	ASSERT_EQ(stackAllocator.getMarker(), 0);
	ASSERT_EQ(stackAllocator.getSize(), 88);

	/*	*/
	stackAllocator.freeToMarker(64);
	ASSERT_EQ(stackAllocator.getMarker(), 64);

	DoubleBufferedAllocator dobuleStackAllocator;
}



//TODO add additional more.
TEST(DataStructureQueue, Queue_Create_Default_No_Throw){
	Queue<int> queue();
}

TEST(DataStructureQueue, Queue_Add_On_Full_Queue_No_Throw)
{
}