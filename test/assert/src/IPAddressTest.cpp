#include <Core/Network/IP.h>
#include <Core/Network/IPAddress.h>
#include <Core/RefPtr.h>
#include <gtest/gtest.h>
#include <sched.h>
// #include <Core/IO/IP.h>
// #include <Core/IO/NetSocket.h>

using namespace fragcore;

class IPTest : public testing::Test {
  protected:
};

TEST_F(IPTest, IP_Create_Local_Connection_No_Exception) {
	// IP
	ASSERT_NO_THROW(IPAddress localHost("localhost", IPAddress::IPAddressType::IPAddress_Type_IPV4));
}

TEST_F(IPTest, IP) {

	// IP ip;
}

TEST_F(IPTest, Socket) {

	// IP ip;

	// NetSocket netSocket;
}

TEST(Network, NetworkSocket_CreateLocal_Connection_No_Throw_Exception) {}
