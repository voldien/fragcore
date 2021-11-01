#include <Core/Network/IPInterface.h>
#include <Core/Network/TCPUDPAddress.h>
#include <Core/RefPtr.h>
#include <gtest/gtest.h>

using namespace fragcore;

class TCPUDPAddressTest : public testing::Test {
  protected:
};

TEST_F(TCPUDPAddressTest, Create_Local_Address_IPV4_Throw_Exception) {
	ASSERT_ANY_THROW(IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV6));
}

TEST_F(TCPUDPAddressTest, Create_Local_Address_IPV4_No_Exception) {
	ASSERT_NO_THROW(IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4));
}

TEST_F(TCPUDPAddressTest, Create_Local_Address_IPV6_With_IPv6_Throw_Exception) {
	ASSERT_NO_THROW(IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV6));
}

TEST_F(TCPUDPAddressTest, Create_Local_Adddress_FDQN_No_Exception) {
	ASSERT_NO_THROW(IPAddress localHost("localhost", IPAddress::IPAddressType::IPAddress_Type_IPV4));
}

TEST_F(TCPUDPAddressTest, Create_Local_Adddress_FDQN_InCorrect_Throw_Exception) {
	ASSERT_ANY_THROW(IPAddress localHost("incorrect_fqdn"));
}