#include "Core/Network/IPAddress.h"
#include "Core/Network/UDPSocket.h"
#include <gtest/gtest.h>

using namespace fragcore;

class UDPNetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
};

TEST_F(UDPNetworkTest, Create_Default_Constructor_No_Throw_Exception) { ASSERT_NO_THROW(UDPNetSocket netSocket); }

TEST_F(UDPNetworkTest, Create_Default_Correct_NetworkProtocol) {
	UDPNetSocket netSocket;
	ASSERT_EQ(netSocket.getTransportProtocol(), NetSocket::TransportProtocol::TransportProtocolUDP);
}

TEST_F(UDPNetworkTest, CreateLocal_Bind_No_Throw_Exception) {
	UDPNetSocket netSocket;
	const int port = 43323;
	const char *ipAddress = "127.0.0.1";

	IPAddress localHost(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);

	ASSERT_NO_THROW(netSocket.bind(localHost, port));
}
