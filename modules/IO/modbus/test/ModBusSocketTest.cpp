

#include "NetModbusSocket.h"
#include <Core/Network/IPAddress.h>
#include <Core/Network/TCPSocket.h>
#include <Core/Network/TCPUDPAddress.h>
#include <gtest/gtest.h>

using namespace fragcore;

class ModBusNetworkTest : public testing::Test {
  protected:
	void SetUp() override {}
	void TearDown() override {}
};

class ModBusServerClientTest : public testing::Test {
  protected:
	void SetUp() override {

		const char *ipAddress = "localhost";

		/*	*/
		TCPUDPAddress tcpAddress(IPAddress(ipAddress), this->port);

		/*	*/
		modbusNetSocket = new ModbusNetSocket();
		modbusNetSocket->bind(tcpAddress);
		modbusNetSocket->listen(2);
	}
	void TearDown() override { delete modbusNetSocket; }
	const int port = 43323;
	ModbusNetSocket *modbusNetSocket;
};

TEST_F(ModBusServerClientTest, Connect_And_Accept_No_Throw_Exception) {
	IPAddress ipAddress("localhost");
	TCPUDPAddress localHost(ipAddress, this->port);

	ModbusNetSocket clientSocket;
	ASSERT_NO_THROW(clientSocket.connect(localHost));

	ModbusNetSocket acceptAddress;
	ASSERT_NO_THROW(modbusNetSocket->accept(acceptAddress));
}

TEST_F(ModBusServerClientTest, Bind_IO_To_NetSocket_No_Throw_Exception) {

	Ref<NetSocket> netSocket = Ref<NetSocket>(this->modbusNetSocket);
	// ASSERT_NO_THROW(SocketIO io(netSocket));
}
