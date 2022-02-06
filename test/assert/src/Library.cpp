#include <Core/Library.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(Library, Set_Invalid_NULL_Path_Throw_Exception) { ASSERT_THROW(Library(nullptr), InvalidArgumentException); }

TEST(Library, Set_Invalid_Path_Throw_Exception) { ASSERT_THROW(Library(""), InvalidArgumentException); }