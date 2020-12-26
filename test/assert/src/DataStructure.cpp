#include<gtest/gtest.h>
#include"Utils/TextureUtil.h"
#include"Core/dataStructure/PoolAllocator.h"
#include <Core/dataStructure/ITree.h>
#include<Core/dataStructure/StackAllactor.h>
#include<Core/dataStructure/DoubleBufferedAllocator.h>
#include <Core/dataStructure/Queue.h>
using namespace fragcore;

TEST(Tree, Tree_Default_Empty){
//	ITree<int> tree;
//	ASSERT_TRUE(tree.isEmpty());
}

TEST(Tree, Tree_Add_Child){
//	ITree<int> tree;
	//int tree_node;
	//ASSERT_NO_THROW(tree.addChild(&tree_node));
	///ASSERT_EQ(tree.getChild(0), &tree_node);
}

TEST(Tree, Tree_Child_Parent_Correct){
//	ITree<int> tree;
	//ITree<int> tree_node;
	//ASSERT_NO_THROW(tree.addChild(&tree_node));
	//ASSERT_EQ(tree.getChild(0).getPare, &tree_node);
}

TEST(DataStructure, PoolAllocator_Allocate_No_Throw){
	ASSERT_NO_THROW(PoolAllocator<int> allocator(32));
}

TEST(DataStructure, PoolAllocator_obtain){
	PoolAllocator<int> allocator(32);
	allocator.obtain();
}

TEST(DataStructure, PoolAllocator)
{
	PoolAllocator<int> allocator(32);
	std::vector<int*> pointers;

	const int s = allocator.reserved();
	for(int i = 0; i < s; i++){
		int* p = allocator.obtain();
		pointers.push_back(p);
	}

	for(int i = 0; i < pointers.size(); i++){
		allocator.Return(pointers[i]);
	}

	allocator.resize(allocator.reserved() * 2 );
}

TEST(DataStructure, StackAllocator){
	const int s_size = 128;
	StackAllocator stackAllocator(s_size);
	/*	Test initial state.	*/
	ASSERT_EQ(stackAllocator.getSize(), s_size);
	ASSERT_EQ(stackAllocator.getMarker(), 0);

	/*	*/
	void *p = stackAllocator.allocateAligned(84, 8);
	ASSERT_NE(p, NULL);
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