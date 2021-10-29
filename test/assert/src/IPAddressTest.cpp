#include <Core/Network/IPAddress.h>
#include <Core/Network/IPInterface.h>
#include <Core/RefPtr.h>
#include <gtest/gtest.h>
#include <sched.h>
// #include <Core/IO/IP.h>
// #include <Core/IO/NetSocket.h>

using namespace fragcore;

class IPAddressTest : public testing::Test {
  protected:
};

TEST_F(IPAddressTest, Create_Local_Address_IPV4_Throw_Exception) {
	// IP
	ASSERT_ANY_THROW(IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV6));
}

TEST_F(IPAddressTest, Create_Local_Address_IPV4_No_Exception) {
	// IP
	ASSERT_NO_THROW(IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4));
}

TEST_F(IPAddressTest, Create_Local_Address_IPV6_With_IPv6_Throw_Exception) {
	// IP
	ASSERT_NO_THROW(IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV6));
}

TEST_F(IPAddressTest, Create_Local_Adddress_FDQN_No_Exception) {
	ASSERT_NO_THROW(IPAddress localHost("localhost", IPAddress::IPAddressType::IPAddress_Type_IPV4));
	// IP ip;
}

TEST_F(IPAddressTest, Socket) {

	// IP ip;

	// NetSocket netSocket;
}
