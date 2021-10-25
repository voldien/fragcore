#include "Core/Network/IPAddress.h"
#include "Core/Network/UDPSocket.h"
#include <gtest/gtest.h>

using namespace fragcore;

class UDPNetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
};

TEST_F(UDPNetworkTest, NetworkSocket_Create_Default_No_Throw_Exception) { ASSERT_NO_THROW(UDPNetSocket netSocket); }

TEST_F(UDPNetworkTest, NetworkSocket_Create_Binded_LocalHost_No_Throw_Exception) {
	UDPNetSocket netSocket;
	const int port = 43323;
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);

	ASSERT_NO_THROW(netSocket.bind(localHost, port));
}
