#include "Core/dataStructure/PoolAllocator.h"
#include "Utils/TextureUtil.h"
#include <Core/dataStructure/DoubleBufferedAllocator.h>
#include <Core/dataStructure/ITree.h>
#include <Core/dataStructure/Queue.h>
#include <Core/dataStructure/StackAllactor.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(Tree, Tree_Default_Is_Empty) {
	ITree<int> tree;
	ASSERT_TRUE(tree.isEmpty());
}

TEST(Tree, Tree_Default_Empty_Is_Root) {
	ITree<int> tree;
	ASSERT_EQ(tree.root(), &tree);
}

TEST(Tree, Tree_Add_Child_Has_Bindeded_Child) {
	ITree<int> tree;
	ITree<int> tree_node;
	ASSERT_NO_THROW(tree.addChild(&tree_node));
	ASSERT_EQ(tree.getChild(0), &tree_node);
}

TEST(Tree, Tree_Child_Parent_Correct) {
	ITree<int> tree;
	ITree<int> tree_node;
	ASSERT_NO_THROW(tree.addChild(&tree_node));
	ASSERT_EQ(tree.getChild(0)->getParent(), &tree_node);
}

TEST(Tree, Tree_Child_Remove_Child) {
	ITree<int> tree;
	ITree<int> tree_node;
	EXPECT_NO_THROW(tree.addChild(&tree_node));
	tree.removeChild(0);
	ASSERT_TRUE(tree.isEmpty());
}