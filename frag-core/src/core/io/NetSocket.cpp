#include "Core/Network/NetSocket.h"
using namespace fragcore;

NetSocket::NetSocket(const IPInterface &ip) {}

NetSocket::~NetSocket() { /*Make sure it gets disconnected.   */
}

constexpr const char *NetSocket::getTransportProtocolSymbol(TransportProtocol transportProtocol) noexcept {
	switch (transportProtocol) {
	case TransportProtocol::TransportProtocolTCP:
		return "TCP";

	case TransportProtocol::TransportProtocolUDP:
		return "TCP";
	default:
		return "";
	}
}
constexpr const char *NetSocket::getNetStatusSymbol(NetStatus status) noexcept {}