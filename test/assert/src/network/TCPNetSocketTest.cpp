#include "Core/IO/SocketIO.h"
#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include "Core/Network/TCPUDPAddress.h"
#include <gtest/gtest.h>

using namespace fragcore;

class TCPSocketNetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
	void TearDown() override {}
};

class TCPSocketServerClientTest : public testing::Test {
  protected:
	void SetUp() override {

		const char *ipAddress = "127.0.0.1";

		/*	*/
		IP4Address = IPAddress(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4);
		TCPUDPAddress tcpAddress(IP4Address, port);

		/*	*/
		tcpNetServerSocket = new TCPNetSocket();
		tcpNetServerSocket->bind(tcpAddress);
		tcpNetServerSocket->listen(2);
	}
	void TearDown() override {
		tcpNetServerSocket->close();
		delete tcpNetServerSocket;
	}
	const int port = 43323;
	TCPNetSocket *tcpNetServerSocket;
	IPAddress IP4Address;
};

TEST_F(TCPSocketNetworkTest, Create_Default_Constructor_No_Throw_Exception) { ASSERT_NO_THROW(TCPNetSocket netSocket); }

TEST_F(TCPSocketNetworkTest, Create_Default_Correct_NetworkProtocol) {
	TCPNetSocket netSocket;
	ASSERT_EQ(netSocket.getTransportProtocol(), NetSocket::TransportProtocol::TransportProtocolTCP);
}

TEST_F(TCPSocketNetworkTest, CreateLocal_Bind_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const char *ipAddress = "127.0.0.1";
	TCPUDPAddress tcpAddress(IPAddress(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4), 42323);

	ASSERT_NO_THROW(tpcSocket.bind(tcpAddress));
}

TEST_F(TCPSocketNetworkTest, CreateLocal_Bind_With_Listen_No_Throw_Exception) {
	TCPNetSocket tpcSocket;
	const char *ipAddress = "127.0.0.1";

	TCPUDPAddress tcpAddress(IPAddress(ipAddress, IPAddress::IPAddressType::IPAddress_Type_IPV4), 42323);

	ASSERT_NO_THROW(tpcSocket.bind(tcpAddress));
	ASSERT_NO_THROW(tpcSocket.listen(2));
}

TEST_F(TCPSocketNetworkTest, CreateLocal_Bind_And_Connect_No_Throw_Exception) {
	TCPNetSocket tpcSocket;

	const int port = 43323;
	IPAddress localHost("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
	TCPUDPAddress tcpAddress(localHost, 42323);
	tpcSocket.bind(tcpAddress);
	tpcSocket.listen(2);

	TCPNetSocket serverSocket;
	ASSERT_NO_THROW(serverSocket.connect(tcpAddress));
}

TEST_F(TCPSocketServerClientTest, Bind_IO_To_NetSocket_No_Throw_Exception) {

	/*	*/
	Ref<NetSocket> netSocket = Ref<NetSocket>(this->tcpNetServerSocket);

	//	ASSERT_NO_THROW(SocketIO io(netSocket));
}

TEST_F(TCPSocketServerClientTest, Connect_And_Accept_No_Throw_Exception) {

	TCPUDPAddress localHost(this->IP4Address, this->port);

	/*	*/
	TCPNetSocket clientSocket;
	ASSERT_NO_THROW(clientSocket.connect(localHost));

	/*	*/
	TCPUDPAddress acceptAddress;
	ASSERT_NO_THROW(tcpNetServerSocket->accept(acceptAddress));
}

TEST_F(TCPSocketServerClientTest, Connect_And_Send_No_Throw_Exception) {
	TCPUDPAddress localHost(this->IP4Address, this->port);

	/*	*/
	TCPNetSocket clientSocket;
	ASSERT_NO_THROW(clientSocket.connect(localHost));

	std::vector<unsigned int> buffer(16, 0);
	int n_sent;
	ASSERT_NO_THROW(clientSocket.send(buffer.data(), buffer.size() * sizeof(unsigned int), n_sent));
}