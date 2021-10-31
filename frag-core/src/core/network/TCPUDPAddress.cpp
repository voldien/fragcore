#include "Core/Network/TCPUDPAddress.h"
#include "Core/Network/IPAddress.h"

using namespace fragcore;

TCPUDPAddress::TCPUDPAddress(const IPAddress &ipaddr, unsigned int port)
	: INetAddress(INetAddress::NetworkProtocol::NetWorkProtocol_TCP_UDP) {
	this->ipAddress = ipaddr;
	this->port = port;
}