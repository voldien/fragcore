
#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include "Core/Network/TCPUDPAddress.h"
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

size_t TCPNetSocket::setupIPAddress(struct sockaddr *addr, const INetAddress &p_addr, uint16_t p_port) {

	size_t addrlen;

	const TCPUDPAddress &tcpAddress = static_cast<const TCPUDPAddress &>(p_addr);

	int domain = TCPNetSocket::getDomain(tcpAddress);
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

TCPNetSocket::TCPNetSocket() : socket(0), netStatus(NetStatus::Status_Disconnected) {}
TCPNetSocket::TCPNetSocket(int socket) : socket(socket) {}
TCPNetSocket::~TCPNetSocket() { this->close(); }

NetSocket::TransportProtocol TCPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TransportProtocolTCP;
}

int TCPNetSocket::close() {
	if (this->socket > 0) {
		int status = ::close(this->socket);
		if (status != 0)
			throw RuntimeException("{}", strerror(errno));
		this->socket = 0;
	}
	netStatus = NetStatus::Status_Disconnected;
	return 0;
}

int TCPNetSocket::bind(const INetAddress &p_addr) {
	socklen_t addrlen; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;

	int flags = SOCK_STREAM;

	/*	*/
	if (isValidNetworkAddress(p_addr))
		throw RuntimeException("Invalid Net Address Type");

	/*	*/
	const TCPUDPAddress *tcpAddress = dynamic_cast<const TCPUDPAddress *>(&p_addr);
	if (tcpAddress == nullptr)
		throw RuntimeException("Not a valid NetAddress Object");

	/*	*/
	int domain = getDomain(*tcpAddress);
	addrlen = setupIPAddress((struct sockaddr *)&addrU, p_addr, tcpAddress->getPort());

	/*	*/
	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("TCP socket - Failed to create socket {} - {}", domain, strerror(errno));
	}

	setTimeout(0);

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)&addrU, addrlen) < 0) {
		this->netStatus = NetStatus::Status_Error;
		RuntimeException ex("Failed to bind TCP socket, {}", strerror(errno));
		throw ex;
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
	return 0;
}

int TCPNetSocket::listen(unsigned int maxListen) {
	if (::listen(this->socket, maxListen) < 0) {
		SystemException(errno, std::system_category(), "TCP socket: Failed to set listen {} - error: {}", maxListen,
						strerror(errno));
	}
	return 0;
}
// int TCPNetSocket::connect(std::string &ip, unsigned int port) {
// 	return connect(IPAddress(ip, IPAddress::IPAddressType::IPAddress_Type_IPV4), port);
// }

int TCPNetSocket::connect(const INetAddress &p_addr) {
	socklen_t addrlen; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;

	int flags = SOCK_STREAM;
	// #ifdef SOCK_CLOEXEC
	// 	flags |= SOCK_CLOEXEC;
	// #endif

	// #ifdef SOCK_NONBLOCK
	// 	flags |= SOCK_NONBLOCK;
	// #endif

	const TCPUDPAddress *tcpAddress = dynamic_cast<const TCPUDPAddress *>(&p_addr);
	if (tcpAddress == nullptr)
		throw RuntimeException("Not a valid NetAddress Object");
	int domain = this->getDomain(*tcpAddress);

	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("TCP socket - Failed to create socket {} - {}", domain, strerror(errno));
	}

	int option = 1;
	int rc = setsockopt(this->socket, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(int));
	if (rc == -1) {
		throw RuntimeException();
	}

	addrlen = setupIPAddress((struct sockaddr *)&addrU, *tcpAddress, tcpAddress->getPort());

	rc = ::connect(socket, (struct sockaddr *)&addrU, addrlen);
	if (rc != 0) {
		throw RuntimeException("Failed to connect TCP socket, {}", strerror(errno));
	} else {
		this->netStatus = NetStatus::Status_Done;
	}

	return 0;
}

int TCPNetSocket::poll(int p_type, int timeout) const { /*	select or poll.	*/
}
int TCPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) {
	int flag = 0;
	int res; //= recvfrom(this->socket, p_buffer, p_len, flag, connection->intaddr, &r_read);
	return res;
}
int TCPNetSocket::recv(void *pbuffer, int p_len, int &sent, bool peek) {
	int flag = 0;
	int res = ::recv(this->socket, (void *)pbuffer, p_len, flag);
	sent = res;
	return res;
}
int TCPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {
	int flag = 0;
	int res = ::send(this->socket, p_buffer, p_len, flag);
	return res;
}
int TCPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_addr) {
	socklen_t addrlen;			 /*	*/
	const struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	const TCPUDPAddress &tcpAddress = static_cast<const TCPUDPAddress &>(p_addr);
	addrlen = setupIPAddress((struct sockaddr *)&addrU, tcpAddress.getIPAddress(), tcpAddress.getPort());
	unsigned int flag = 0;

	int res = ::sendto(this->socket, p_buffer, p_len, flag, (struct sockaddr *)&addrU, addrlen);
	return res;
}

long int TCPNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	long int res = ::send(this->socket, pbuffer, p_len, flag);
	return res;
}

Ref<NetSocket> TCPNetSocket::accept(INetAddress &r_ip) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		throw SystemException(errno, std::system_category(), "Failed to accept TCP connection");
	}

	TCPNetSocket *_newsocket = new TCPNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}

TCPNetSocket::NetStatus TCPNetSocket::accept(NetSocket &socket) {
	TCPUDPAddress addr;

	Ref<NetSocket> netSocket = this->accept(addr);
	// socket = std::move(*netSocket);
	return netSocket->getStatus();
}
int TCPNetSocket::read() {}
int TCPNetSocket::write() {}
bool TCPNetSocket::isBlocking() { /*	*/
	int flags = fcntl(this->socket, F_GETFL, 0);
	if (flags == -1) {
		//	return false;
	}
	return (flags & ~O_NONBLOCK) == 0;
}

void TCPNetSocket::setBlocking(bool blocking) { /*	*/
	int flags = fcntl(this->socket, F_GETFL, 0);
	if (flags == -1) {
		//	return false;
	}
	flags = (flags & ~O_NONBLOCK);
	int rc = fcntl(this->socket, F_SETFL, flags);
	if (rc < 0) {
		throw RuntimeException("Failed to set blocking");
	}
}
TCPNetSocket::NetStatus TCPNetSocket::getStatus() const noexcept { return this->netStatus; }

bool TCPNetSocket::isValidNetworkAddress(const INetAddress &address) {
	return address.getNetworkProtocol() == INetAddress::NetworkProtocol::NetWorkProtocol_TCP_UDP;
}

void TCPNetSocket::setTimeout(long int nanoSeconds) {
	struct timeval tv;

	/*	Set timeout for client.	*/
	tv.tv_sec = nanoSeconds / 1000000000;
	tv.tv_usec = nanoSeconds % 1000000000;
	int rc = setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	if (rc < 0) {
	}
}

int TCPNetSocket::getDomain(const INetAddress &address) {

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