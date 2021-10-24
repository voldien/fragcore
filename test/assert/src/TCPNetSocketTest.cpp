#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include <gtest/gtest.h>

using namespace fragcore;

class NetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
};

TEST_F(NetworkTest, NetworkSocket_Create_Default_No_Throw_Exception) { ASSERT_NO_THROW(TCPNetSocket netSocket); }

TEST_F(NetworkTest, NetworkSocket_CreateLocal_Bind_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	ASSERT_NO_THROW(tpcSocket.bind(localHost, 42323));
}

TEST_F(NetworkTest, NetworkSocket_CreateLocal_Bind_And_Connect_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const int port = 43323;
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	tpcSocket.bind(localHost, port);

	TCPNetSocket serverSocket;
	ASSERT_NO_THROW(serverSocket.connect(localHost, port));
}
