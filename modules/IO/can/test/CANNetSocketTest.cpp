#include "NetCANSocket.h"
#include <gtest/gtest.h>

using namespace fragcore;

TEST(CANSocketTest, Create_Default_Constructor_No_Throw_Exception) { ASSERT_NO_THROW(CANNetSocket netSocket); }
