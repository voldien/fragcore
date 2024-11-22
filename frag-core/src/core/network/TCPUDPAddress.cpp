#include "Network/TCPUDPAddress.h"
#include "Network/IPAddress.h"
#include <limits>

using namespace fragcore;

TCPUDPAddress::TCPUDPAddress(const IPAddress &ipaddr, unsigned int port) : IPAddress(ipaddr), port(port) {  }

bool TCPUDPAddress::isValid() const noexcept {
	return IPAddress::isValid() && this->port > 0 && this->port <= std::numeric_limits<unsigned short>::max();
}