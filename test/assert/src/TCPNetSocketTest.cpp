#include "Core/IO/SocketIO.h"
#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include <gtest/gtest.h>

using namespace fragcore;

class TCPSocketNetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
	void TearDown() override {}
};

class TCPSocketServerClientTest : public testing::Test {
  protected:
	virtual void SetUp() override {

		const char *ipAddress = "127.0.0.1";

		IPAddress localHost(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);

		tcpNetSocket = new TCPNetSocket();
		tcpNetSocket->bind(localHost, port);
		tcpNetSocket->listen(2);
	}
	virtual void TearDown() override { delete tcpNetSocket; }
	const int port = 43323;
	TCPNetSocket *tcpNetSocket;
};

TEST_F(TCPSocketNetworkTest, Create_Default_Constructor_No_Throw_Exception) { ASSERT_NO_THROW(TCPNetSocket netSocket); }

TEST_F(TCPSocketNetworkTest, Create_Default_Correct_NetworkProtocol) {
	TCPNetSocket netSocket;
	ASSERT_EQ(netSocket.getTransportProtocol(), NetSocket::TransportProtocol::TransportProtocolTCP);
}

TEST_F(TCPSocketNetworkTest, CreateLocal_Bind_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const char *ipAddress = "127.0.0.1";

	IPAddress localHost(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);
	ASSERT_NO_THROW(tpcSocket.bind(localHost, 42323));
}

TEST_F(TCPSocketNetworkTest, CreateLocal_Bind_With_Listen_No_Throw_Exception) {
	TCPNetSocket tpcSocket;
	const char *ipAddress = "127.0.0.1";
	IPAddress localHost(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);

	ASSERT_NO_THROW(tpcSocket.bind(localHost, 42323));
	ASSERT_NO_THROW(tpcSocket.listen(2));
}

TEST_F(TCPSocketNetworkTest, CreateLocal_Bind_And_Connect_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const int port = 43323;
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	tpcSocket.bind(localHost, port);
	tpcSocket.listen(2);

	TCPNetSocket serverSocket;
	ASSERT_NO_THROW(serverSocket.connect(localHost, port));
}

TEST_F(TCPSocketServerClientTest, Bind_IO_To_NetSocket_No_Throw_Exception) {

	Ref<NetSocket> netSocket = Ref<NetSocket>(this->tcpNetSocket);
	ASSERT_NO_THROW(SocketIO io(netSocket));
}

TEST_F(TCPSocketServerClientTest, Connect_And_Accept_No_Throw_Exception) {
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);

	TCPNetSocket clientSocket;
	ASSERT_NO_THROW(clientSocket.connect(localHost, port));

	IPAddress acceptAddress;
	uint16_t acceptPort;
	ASSERT_NO_THROW(tcpNetSocket->accept(acceptAddress, acceptPort));
}

TEST_F(TCPSocketServerClientTest, Connect_And_Send_No_Throw_Exception) {
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);

	TCPNetSocket clientSocket;
	ASSERT_NO_THROW(clientSocket.connect(localHost, port));

	std::vector<unsigned int> buffer(16, 0);
	int n_sent;
	ASSERT_NO_THROW(clientSocket.send(buffer.data(), buffer.size() * sizeof(unsigned int), n_sent));
}