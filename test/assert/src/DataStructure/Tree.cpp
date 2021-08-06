#include "Core/dataStructure/PoolAllocator.h"
#include "Utils/TextureUtil.h"
#include <Core/dataStructure/DoubleBufferedAllocator.h>
#include <Core/dataStructure/ITree.h>
#include <Core/dataStructure/Queue.h>
#include <Core/dataStructure/StackAllactor.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(Tree, Tree_Default_Empty) {
	ITree<int> tree;
	ASSERT_TRUE(tree.isEmpty());
}

TEST(Tree, Tree_Add_Child) {
	//	ITree<int> tree;
	// int tree_node;
	// ASSERT_NO_THROW(tree.addChild(&tree_node));
	/// ASSERT_EQ(tree.getChild(0), &tree_node);
}

TEST(Tree, Tree_Child_Parent_Correct) {
	//	ITree<int> tree;
	// ITree<int> tree_node;
	// ASSERT_NO_THROW(tree.addChild(&tree_node));
	// ASSERT_EQ(tree.getChild(0).getPare, &tree_node);
}