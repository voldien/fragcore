#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include <gtest/gtest.h>

using namespace fragcore;

class NetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
};

TEST_F(NetworkTest, NetworkSocket_Create_Default_No_Throw_Exception) { ASSERT_NO_THROW(TCPNetSocket netSocket()); }

TEST_F(NetworkTest, NetworkSocket_CreateLocal_Connection_No_Throw_Exception) {
	// IPAddress::getLocalAddresses();
	TCPNetSocket netSocket();

	IPAddress localHost("localhost", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	netSocket.bind(localHost, 25120);

}
