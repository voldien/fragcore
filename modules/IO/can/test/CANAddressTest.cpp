#include <CANAddress.h>
#include <gtest/gtest.h>

using namespace fragcore;

class CANAddressTest : public testing::Test {
  protected:
};

TEST_F(CANAddressTest, Create_Default_Throw_Exception) { ASSERT_ANY_THROW(CANAddress address(0)); }

TEST_F(CANAddressTest, Create_Default_Correct_ID_No_Throw_Exception) {
	const int IDAddress = 120;
	CANAddress address(IDAddress);
	ASSERT_EQ(address.getID(), IDAddress);
}

TEST_F(CANAddressTest, Create_Default_Invalid_Address_Throw_Exception) {
	const int IDAddress = 120;
	CANAddress address(IDAddress);
	// ASSERT_EQ(address.getID(), address);
}