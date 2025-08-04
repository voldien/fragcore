#include "Network/NetSocket.h"
using namespace fragcore;

NetSocket::~NetSocket() { /*Make sure it gets disconnected.   */ }

constexpr const char *NetSocket::getTransportProtocolSymbol(const TransportProtocol transportProtocol) noexcept {
	switch (transportProtocol) {
	case TransportProtocol::TransportProtocolTCP:
		return "TCP";
	case TransportProtocol::TransportProtocolUDP:
		return "UDP";
	default:
		return "Unknown";
	}
}
constexpr const char *NetSocket::getNetStatusSymbol(const NetStatus status) noexcept {
	switch (status) {
	case NetStatus::Status_Done:
		return "";

	default:
		return "";
	}
}