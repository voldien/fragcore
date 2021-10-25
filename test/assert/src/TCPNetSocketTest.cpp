#include "Core/IO/SocketIO.h"
#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include <gtest/gtest.h>

using namespace fragcore;

class TCPSocketNetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
};

TEST_F(TCPSocketNetworkTest, NetworkSocket_Create_Default_No_Throw_Exception) {
	ASSERT_NO_THROW(TCPNetSocket netSocket);
}

TEST_F(TCPSocketNetworkTest, NetworkSocket_CreateLocal_Bind_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const char *ipAddress = "127.0.0.1";

	IPAddress localHost(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);
	ASSERT_NO_THROW(tpcSocket.bind(localHost, 42323));
}

TEST_F(TCPSocketNetworkTest, NetworkSocket_CreateLocal_Bind_With_Listen_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const char *ipAddress = "127.0.0.1";

	IPAddress localHost(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);
	ASSERT_NO_THROW(tpcSocket.bind(localHost, 42323));
	ASSERT_NO_THROW(tpcSocket.listen(2));
}

TEST_F(TCPSocketNetworkTest, NetworkSocket_CreateLocal_Bind_And_Connect_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const int port = 43323;
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	tpcSocket.bind(localHost, port);
	tpcSocket.listen(2);

	TCPNetSocket serverSocket;
	ASSERT_NO_THROW(serverSocket.connect(localHost, port));
}

TEST_F(TCPSocketNetworkTest, NetworkSocket_CreateLocal_Bind_And_Connect_And_Accept_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const int port = 43323;
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	tpcSocket.bind(localHost, port);
	tpcSocket.listen(2);

	TCPNetSocket serverSocket;
	ASSERT_NO_THROW(serverSocket.connect(localHost, port));

	// ASSERT_NO_THROW(tpcSocket.accept(localHost, port));
}

TEST_F(TCPSocketNetworkTest, NetworkSocket_CreateLocal_Bind_IO_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const int port = 43323;
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	tpcSocket.bind(localHost, port);
	tpcSocket.listen(2);

	Ref<NetSocket> netSocket = Ref<NetSocket>(&tpcSocket);
	ASSERT_NO_THROW(SocketIO io(netSocket));
}