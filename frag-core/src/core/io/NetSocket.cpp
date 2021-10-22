#include "Core/Network/NetSocket.h"
using namespace fragcore;

NetSocket::NetSocket(const IPInterface &ip) {}

NetSocket::~NetSocket() {
	/*Make sure it gets disconnected.   */
	close();
}

constexpr const char *NetSocket::getTransportProtocolSymbol(TransportProtocol transportProtocol) noexcept { return ""; }
constexpr const char *NetSocket::getNetStatusSymbol(NetStatus status) noexcept {}