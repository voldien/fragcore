#include <Core/Library.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(Library, Set_NULL_Path_No_Throw_Exception) { ASSERT_NO_THROW(Library(nullptr)); }
