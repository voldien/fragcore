#include "Core/Network/INetAddress.h"
#include "Core/Network/TCPUDPAddress.h"
#include "Core/Network/UDPSocket.h"
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace fragcore;

UDPNetSocket::UDPNetSocket() : netStatus(NetStatus::Status_Disconnected), socket(0) { this->UDPaddr = nullptr; }

UDPNetSocket::UDPNetSocket(int socket) : UDPNetSocket() {
	this->socket = socket;
	this->UDPaddr = nullptr;
	// TODO get the status of the socket.
	this->netStatus = NetStatus::Status_Done;
}
UDPNetSocket::~UDPNetSocket() { this->close(); }

NetSocket::TransportProtocol UDPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TransportProtocolUDP;
}

int UDPNetSocket::close() {
	free(this->UDPaddr);
	this->UDPaddr = nullptr;
	if (this->socket > 0) {
		int rc = ::close(this->socket);
		if (rc != 0)
			throw RuntimeException("{}", strerror(errno));
		this->socket = 0;
	}
	this->netStatus = NetStatus::Status_Disconnected;
	return 0;
}
int UDPNetSocket::bind(const INetAddress &p_addr) {
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;

	int flags = SOCK_DGRAM;

	/*	*/
	// if (!isValidNetworkAddress(p_addr)) {
	//	throw RuntimeException("Invalid Net Address");
	//}

	/*	*/
	const TCPUDPAddress *udpAddress = dynamic_cast<const TCPUDPAddress *>(&p_addr);
	if (udpAddress == nullptr) {
		throw RuntimeException("Not a valid NetAddress Object");
	}

	int domain = getDomain(*udpAddress);

	/*	*/
	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("TCP socket - Failed to create socket {} - {}", domain, strerror(errno));
	}

	/*	*/

	this->UDPaddr = malloc(sizeof(addrU));
	memset(this->UDPaddr, 0, sizeof(addrU));
	this->addrSize = setupIPAddress((struct sockaddr *)UDPaddr, p_addr, udpAddress->getPort());

	// this->setTimeout(0);

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)UDPaddr, this->addrSize) < 0) {
		this->netStatus = NetStatus::Status_Error;
		throw RuntimeException("Failed to bind UDP socket - {}:{}, {}", udpAddress->getIPAddress().getIP(),
							   udpAddress->getPort(), strerror(errno));
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
	return 0;
}

int UDPNetSocket::listen(unsigned int maxListen) {
	//	if (::listen(socket, maxListen) < 0) {
	//	}

	return 0;
}

int UDPNetSocket::connect(const INetAddress &p_addr) {
	socklen_t addrlen = 0;		 /*	*/
	const struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	struct hostent *hosten = nullptr; /*	*/
	int domain;
	int flags = SOCK_DGRAM;
	struct timeval tv;

	// if (!isValidNetworkAddress(p_addr)) {
	//	throw RuntimeException("Invalid Net Address");
	//}

	/*	*/
	const TCPUDPAddress *udpAddress = dynamic_cast<const TCPUDPAddress *>(&p_addr);
	if (udpAddress == nullptr)
		throw RuntimeException("Not a valid NetAddress Object");
	domain = this->getDomain(*udpAddress);

	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("UDP socket - Failed to create socket {} - {}", domain, strerror(errno));
	}

	//	int option = 1;
	//	int rc = setsockopt(this->socket, IPPROTO_UDP, NODELA, (const void *)&option, sizeof(int));
	//	if (rc == -1) {
	//		throw RuntimeException("Failed to set Socket Option");
	//	}
	this->UDPaddr = malloc(sizeof(addrU));
	memset(this->UDPaddr, 0, sizeof(addrU));
	this->addrSize = setupIPAddress((struct sockaddr *)UDPaddr, *udpAddress, udpAddress->getPort());

	// if (::connect(socket, addr, addrlen) < 0) {
	//	throw RuntimeException("Failed to connect UDP socket - {}", strerror(errno));
	//}
	return 0;
}

int UDPNetSocket::poll(int p_type, int timeout) const { return 0; }
int UDPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) { return 0; }
int UDPNetSocket::recv(void *pbuffer, int p_len, int &sent, bool peek) {
	struct sockaddr_in servaddr;
	socklen_t len;
	int flag = 0;
	return ::recvfrom(this->socket, pbuffer, p_len, flag, (struct sockaddr *)&servaddr, &len);
}
int UDPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {
	int flag = MSG_CONFIRM;

	return ::sendto(this->socket, p_buffer, p_len, flag, (const struct sockaddr *)this->UDPaddr, this->addrSize);
}
int UDPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) { return 0; }
long int UDPNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = MSG_CONFIRM;
	return ::sendto(this->socket, pbuffer, p_len, flag, (const struct sockaddr *)this->UDPaddr, this->addrSize);
}
Ref<NetSocket> UDPNetSocket::accept(INetAddress &r_ip) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		throw SystemException(errno, std::system_category(), "Failed to accept TCP connection");
	}

	UDPNetSocket *_newsocket = new UDPNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}
UDPNetSocket::NetStatus UDPNetSocket::accept(NetSocket &socket) { return UDPNetSocket::NetStatus::Status_Disconnected; }
int UDPNetSocket::read() { return 0; }
int UDPNetSocket::write() { return 0; }
bool UDPNetSocket::isBlocking() { return 0; }
void UDPNetSocket::setBlocking(bool blocking) {}
UDPNetSocket::NetStatus UDPNetSocket::getStatus() const noexcept { return this->netStatus; }

size_t UDPNetSocket::setupIPAddress(struct sockaddr *addr, const INetAddress &p_addr, uint16_t p_port) {

	size_t addrlen;

	const TCPUDPAddress &tcpAddress = static_cast<const TCPUDPAddress &>(p_addr);

	int domain = UDPNetSocket::getDomain(tcpAddress);

	/*	*/
	if (domain == AF_INET) {
		const IPAddress &ipAddress = tcpAddress.getIPAddress();

		const uint8_t *_addr = ipAddress.getAddress(IPAddress::IPAddressType::IPAddress_Type_IPV4);
		sockaddr_in *addr4 = (sockaddr_in *)(addr);
		addrlen = sizeof(*addr4);

		size_t addr_len = sizeof(addr4->sin_addr);
		memcpy(&addr4->sin_addr, _addr, addr_len);

		addr4->sin_port = htons(p_port);
		addr4->sin_family = (sa_family_t)domain;

	} else if (domain == AF_INET6) {
		const IPAddress &ipAddress = tcpAddress.getIPAddress();
		const uint8_t *_addr = ipAddress.getAddress(IPAddress::IPAddressType::IPAddress_Type_IPV4);
		sockaddr_in6 *addr6 = (sockaddr_in6 *)(addr);
		addrlen = sizeof(*addr6);

		size_t addr_len = sizeof(addr6->sin6_addr);
		memcpy(&addr6->sin6_addr, _addr, addr_len);

		addr6->sin6_port = htons(p_port);
		addr6->sin6_family = (sa_family_t)domain;

	} else {
		throw RuntimeException("Not a valid Net Address");
	}
	return addrlen;
}

int UDPNetSocket::getDomain(const INetAddress &address) {

	const TCPUDPAddress &tcpAddress = dynamic_cast<const TCPUDPAddress &>(address);

	int domain = 0; // TODO be override by the NetAddress!
	switch (address.getNetworkProtocol()) {
	case INetAddress::NetworkProtocol::NetWorkProtocol_TCP_UDP:
	case INetAddress::NetworkProtocol::NetWorkProtocol_IP: {
		const IPAddress &ipAddress = tcpAddress.getIPAddress();
		if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV4)
			return AF_INET;
		else if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV6)
			return AF_INET6;
		else
			throw RuntimeException("No valid IP address");
	} break;
	case INetAddress::NetworkProtocol::NetWorkProtocol_CAN:
		return AF_CAN;
		break;
	default:
		throw RuntimeException("Non Supported Network Protocol: {}", (unsigned int)address.getNetworkProtocol());
	}
}