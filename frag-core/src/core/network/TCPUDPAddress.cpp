#include "Core/Network/TCPUDPAddress.h"
#include "Core/Network/IPAddress.h"

using namespace fragcore;

TCPUDPAddress::TCPUDPAddress(const IPAddress &ipaddr, unsigned int port) {

	this->ipAddress = ipaddr;
	this->port = port;
}

bool TCPUDPAddress::isValid() const noexcept {
	return this->getIPAddress().isValid() && this->port > 0 && this->port < 65536;
}