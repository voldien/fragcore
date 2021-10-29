
#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include "Core/Network/TCPUDPAddress.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace fragcore;

size_t TCPNetSocket::setupIPAddress(struct sockaddr *addr, const INetAddress &p_addr, uint16_t p_port) {
	int domain = TCPNetSocket::getDomain(p_addr);
	size_t addrlen;
	/*	*/
	if (domain == AF_INET) {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);
		const uint8_t *_addr = ipAddress.getAddress(IPAddress::IPAddressType::IPAddress_Type_IPV4);
		sockaddr_in *addr4 = (sockaddr_in *)(addr);
		addrlen = sizeof(*addr4);

		size_t addr_len = sizeof(addr4->sin_addr);
		memcpy(&addr4->sin_addr, _addr, addr_len);

		addr4->sin_port = htons(p_port);
		addr4->sin_family = (sa_family_t)domain;

	} else if (domain == AF_INET6) {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);
		const uint8_t *_addr = ipAddress.getAddress(IPAddress::IPAddressType::IPAddress_Type_IPV4);
		sockaddr_in6 *addr6 = (sockaddr_in6 *)(addr);
		addrlen = sizeof(*addr6);

		size_t addr_len = sizeof(addr6->sin6_addr);
		memcpy(&addr6->sin6_addr, _addr, addr_len);

		addr6->sin6_port = htons(p_port);
		addr6->sin6_family = (sa_family_t)domain;

	} else {
		throw RuntimeException("");
	}
	return addrlen;
}

TCPNetSocket::TCPNetSocket() : netStatus(NetStatus::Status_Disconnected), socket(0) {}
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
}

// int TCPNetSocket::bind(std::string &IPaddr, unsigned int port) {
// 	socklen_t addrlen;	   /*	*/
// 	struct sockaddr *addr; /*	*/
// 	union {
// 		struct sockaddr_in addr4;  /*	*/
// 		struct sockaddr_in6 addr6; /*	*/
// 	} addrU;
// 	int domain = AF_INET; // TODO be override by the NetAddress!

// 	/*	*/
// 	if (domain == AF_INET) {
// 		bzero(&addrU.addr4, sizeof(addrU.addr4));
// 		addrU.addr4.sin_port = htons(port);
// 		addrU.addr4.sin_family = (sa_family_t)domain;
// 		if (inet_pton(domain, IPaddr.c_str(), &addrU.addr4.sin_addr) < 0) {
// 			this->close();
// 		}
// 		addrlen = sizeof(addrU.addr4);
// 		addr = (struct sockaddr *)&addrU.addr4;
// 	} else if (domain == AF_INET6) {
// 		bzero(&addrU.addr6, sizeof(addrU.addr6));
// 		addrU.addr6.sin6_port = htons(port);
// 		addrU.addr6.sin6_family = (sa_family_t)domain;
// 		if (inet_pton(domain, IPaddr.c_str(), &addrU.addr6.sin6_addr) < 0) {
// 			this->close();
// 		}
// 		addrlen = sizeof(addrU.addr6);
// 		addr = (struct sockaddr *)&addrU.addr6;
// 	} else {
// 		throw RuntimeException();
// 		this->close();
// 	}

// 	/*	Bind process to socket.	*/
// 	if (::bind(socket, (struct sockaddr *)addr, addrlen) < 0) {
// 		RuntimeException ex("Failed to bind TCP socket, {}", strerror(errno));
// 		this->netStatus = NetStatus::Status_Error;
// 		close();
// 		throw ex;
// 	} else {
// 		this->netStatus = NetStatus::Status_Done;
// 	}
// }

int TCPNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {
	socklen_t addrlen; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;

	int domain = getDomain(p_addr);

	addrlen = setupIPAddress((struct sockaddr *)&addrU, p_addr, p_port);

	this->socket = ::socket(domain, SOCK_STREAM, 0);
	if (this->socket < 0) {
		throw RuntimeException("Failed to create TCP socket, {}", strerror(errno));
	}

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
		SystemException(errno, "TCP socket: Failed to set listen {} - error: {}", maxListen, strerror(errno));
	}
	return 0;
}
// int TCPNetSocket::connect(std::string &ip, unsigned int port) {
// 	return connect(IPAddress(ip, IPAddress::IPAddressType::IPAddress_Type_IPV4), port);
// }

int TCPNetSocket::connect(const INetAddress &p_addr, uint16_t p_port) {
	socklen_t addrlen;			 /*	*/
	const struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	struct timeval tv;
	int flags = SOCK_STREAM;
	// #ifdef SOCK_CLOEXEC
	// 	flags |= SOCK_CLOEXEC;
	// #endif

	// #ifdef SOCK_NONBLOCK
	// 	flags |= SOCK_NONBLOCK;
	// #endif
	int domain = getDomain(p_addr);

	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("TCP socket - Failed to create socket {}", strerror(errno));
	}

	int option = 1;
	int rc = setsockopt(this->socket, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(int));
	if (rc == -1) {
		throw RuntimeException();
	}

	addrlen = setupIPAddress((struct sockaddr *)&addrU, p_addr, p_port);

	rc = ::connect(socket, (struct sockaddr *)&addrU, addrlen);
	if (rc != 0) {
		throw RuntimeException("Failed to connect TCP socket, {}", strerror(errno));
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
}

int TCPNetSocket::open(int p_type, int ip_type) {}

int TCPNetSocket::poll(int p_type, int timeout) const { /*	select or poll.	*/
}
int TCPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, uint16_t &r_port,
						   bool p_peek) {
	int flag = 0;
	int res; //= recvfrom(this->socket, p_buffer, p_len, flag, connection->intaddr, &r_read);
	return res;
}
int TCPNetSocket::recv(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	int res = ::recv(this->socket, (void *)pbuffer, p_len, flag);
	return res;
}
int TCPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {
	int flag = 0;
	int res = ::send(this->socket, p_buffer, p_len, flag);
	return res;
}
int TCPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_addr, uint16_t p_port) {
	socklen_t addrlen;			 /*	*/
	const struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	addrlen = setupIPAddress((struct sockaddr *)&addrU, p_addr, p_port);
	unsigned int flag = 0;

	int res = ::sendto(this->socket, p_buffer, p_len, flag, (struct sockaddr *)&addrU, addrlen);
	return res;
}

long int TCPNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	long int res = ::send(this->socket, pbuffer, p_len, flag);
	return res;
}

Ref<NetSocket> TCPNetSocket::accept(INetAddress &r_ip, uint16_t &r_port) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		throw SystemException(errno, "Failed to accept TCP connection");
		// close();
	}

	TCPNetSocket *_newsocket = new TCPNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}

Ref<NetSocket> TCPNetSocket::accept(std::string &ip, unsigned int port) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		// close();
	}
	TCPNetSocket *_newsocket = new TCPNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}
TCPNetSocket::NetStatus TCPNetSocket::accept(NetSocket &socket) {
	IPAddress ipAddress("");
	uint16_t port;
	Ref<NetSocket> netSocket = this->accept(ipAddress, port);
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
}
TCPNetSocket::NetStatus TCPNetSocket::getStatus() const noexcept { return this->netStatus; }

int TCPNetSocket::getDomain(const INetAddress &address) {

	int domain = 0; // TODO be override by the NetAddress!
	switch (address.getNetworkProtocol()) {
	case INetAddress::NetworkProtocol::NetWorkProtocol_IP: {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(address);
		if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV4)
			return AF_INET;
		else if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV6)
			return AF_INET6;
	} break;
	case INetAddress::NetworkProtocol::NetWorkProtocol_CAN:
		return AF_CAN;
		break;
	default:
		throw RuntimeException("Non Supported Network Protocol: {}", address.getNetworkProtocol());
	}
}