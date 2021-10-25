
#include "Core/Network/IPAddress.h"
#include "Core/Network/TCPSocket.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace fragcore;

// static void setupAddress(){
// 	/*	*/
// 	if (domain == AF_INET) {
// 		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);

// 		bzero(&addrU.addr4, sizeof(addrU.addr4));
// 		addrU.addr4.sin_port = htons(p_port);
// 		addrU.addr4.sin_family = (sa_family_t)domain;
// 		if (inet_pton(domain, ipAddress.getIP().c_str(), &addrU.addr4.sin_addr) < 0) {
// 			throw RuntimeException("");
// 		}
// 		addrlen = sizeof(addrU.addr4);
// 		addr = (struct sockaddr *)&addrU.addr4;
// 	} else if (domain == AF_INET6) {
// 		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);
// 		bzero(&addrU.addr6, sizeof(addrU.addr6));
// 		addrU.addr6.sin6_port = htons(p_port);
// 		addrU.addr6.sin6_family = (sa_family_t)domain;
// 		if (inet_pton(domain, ipAddress.getIP().c_str(), &addrU.addr6.sin6_addr) < 0) {
// 			throw RuntimeException("");
// 		}
// 		addrlen = sizeof(addrU.addr6);
// 		addr = (struct sockaddr *)&addrU.addr6;
// 	} else {

// 		this->close();
// 	}
// }

TCPNetSocket::TCPNetSocket() : netStatus(NetStatus::Status_Disconnected), socket(0) {}
TCPNetSocket::TCPNetSocket(int socket) : socket(socket) {}
TCPNetSocket::~TCPNetSocket() { this->close(); }

NetSocket::TransportProtocol TCPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TCP;
}

int TCPNetSocket::close() {
	int status = ::close(this->socket);
	if (status != 0)
		throw RuntimeException("{}", strerror(errno));
	this->socket = 0;
	netStatus = NetStatus::Status_Disconnected;
}

int TCPNetSocket::bind(std::string &IPaddr, unsigned int port) {
	socklen_t addrlen;	   /*	*/
	struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	int domain = AF_INET; // TODO be override by the NetAddress!

	/*	*/
	if (domain == AF_INET) {
		bzero(&addrU.addr4, sizeof(addrU.addr4));
		addrU.addr4.sin_port = htons(port);
		addrU.addr4.sin_family = (sa_family_t)domain;
		if (inet_pton(domain, IPaddr.c_str(), &addrU.addr4.sin_addr) < 0) {
			this->close();
		}
		addrlen = sizeof(addrU.addr4);
		addr = (struct sockaddr *)&addrU.addr4;
	} else if (domain == AF_INET6) {
		bzero(&addrU.addr6, sizeof(addrU.addr6));
		addrU.addr6.sin6_port = htons(port);
		addrU.addr6.sin6_family = (sa_family_t)domain;
		if (inet_pton(domain, IPaddr.c_str(), &addrU.addr6.sin6_addr) < 0) {
			this->close();
		}
		addrlen = sizeof(addrU.addr6);
		addr = (struct sockaddr *)&addrU.addr6;
	} else {
		throw RuntimeException();
		this->close();
	}

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)addr, addrlen) < 0) {
		RuntimeException ex("Failed to bind TCP socket, {}", strerror(errno));
		this->netStatus = NetStatus::Status_Error;
		close();
		throw ex;
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
}

int TCPNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {
	socklen_t addrlen;	   /*	*/
	struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;

	int domain = getDomain(p_addr);

	/*	*/
	if (domain == AF_INET) {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);

		bzero(&addrU.addr4, sizeof(addrU.addr4));
		addrU.addr4.sin_port = htons(p_port);
		addrU.addr4.sin_family = (sa_family_t)domain;
		/*	*/
		if (inet_pton(domain, ipAddress.getIP().c_str(), &addrU.addr4.sin_addr) < 0) {
			this->close();
		}
		addrlen = sizeof(addrU.addr4);
		addr = (struct sockaddr *)&addrU.addr4;
	} else if (domain == AF_INET6) {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);
		bzero(&addrU.addr6, sizeof(addrU.addr6));
		addrU.addr6.sin6_port = htons(p_port);
		addrU.addr6.sin6_family = (sa_family_t)domain;
		if (inet_pton(domain, ipAddress.getIP().c_str(), &addrU.addr6.sin6_addr) < 0) {
			this->close();
		}
		addrlen = sizeof(addrU.addr6);
		addr = (struct sockaddr *)&addrU.addr6;
	} else {
		throw RuntimeException("None Supported domain {}", domain);
		this->close();
	}

	this->socket = ::socket(domain, SOCK_STREAM, 0);
	if (this->socket < 0) {
		throw RuntimeException("Failed to create TCP socket, {}", strerror(errno));
		// sntLogErrorPrintf("Failed to create socket, %s.\n", strerror(errno));
	}

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)addr, addrlen) < 0) {
		RuntimeException ex("Failed to bind TCP socket, {}", strerror(errno));
		this->netStatus = NetStatus::Status_Error;
		close();
		throw ex;
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
	return 0;
}

int TCPNetSocket::listen(unsigned int maxListen) {
	if (::listen(this->socket, maxListen) < 0) {
		RuntimeException ex("TCP socket: Failed to set listen {} - error: {}", maxListen, strerror(errno));
	}
	return 0;
}
int TCPNetSocket::connect(std::string &ip, unsigned int port) {
	return connect(IPAddress(ip, IPAddress::IPAddressType::IPAddress_Type_IPV4), port);
}

int TCPNetSocket::connect(const INetAddress &p_addr, uint16_t p_port) {
	socklen_t addrlen;			 /*	*/
	const struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	struct timeval tv;

	int domain = getDomain(p_addr);

	this->socket = ::socket(domain, SOCK_STREAM, 0);
	if (this->socket < 0) {
		throw RuntimeException("TCP socket - Failed to create socket {}", strerror(errno));
	}

	/*	*/
	if (domain == AF_INET) {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);

		bzero(&addrU.addr4, sizeof(addrU.addr4));
		addrU.addr4.sin_port = htons(p_port);
		addrU.addr4.sin_family = (sa_family_t)domain;
		if (inet_pton(domain, ipAddress.getIP().c_str(), &addrU.addr4.sin_addr) < 0) {
			throw RuntimeException("");
		}
		addrlen = sizeof(addrU.addr4);
		addr = (struct sockaddr *)&addrU.addr4;
	} else if (domain == AF_INET6) {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(p_addr);
		bzero(&addrU.addr6, sizeof(addrU.addr6));
		addrU.addr6.sin6_port = htons(p_port);
		addrU.addr6.sin6_family = (sa_family_t)domain;
		if (inet_pton(domain, ipAddress.getIP().c_str(), &addrU.addr6.sin6_addr) < 0) {
			throw RuntimeException("");
		}
		addrlen = sizeof(addrU.addr6);
		addr = (struct sockaddr *)&addrU.addr6;
	} else {

		this->close();
	}

	if (::connect(socket, addr, addrlen) != 0) {
		throw RuntimeException("Failed to create TCP socket, {}", strerror(errno));
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
}

int TCPNetSocket::open(int p_type, int ip_type) {}

int TCPNetSocket::poll(int p_type, int timeout) const {}
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
int TCPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip, uint16_t p_port) {
	int res; //=  sendto(this->socket, p_buffer, p_len, flag, connection->extaddr, connection->sclen);
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
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
	}
}
int TCPNetSocket::read() {}
int TCPNetSocket::write() {}
bool TCPNetSocket::isBlocking() { /*	*/
}
void TCPNetSocket::setBlocking(bool blocking) { /*	*/
}
TCPNetSocket::NetStatus TCPNetSocket::getStatus() const noexcept { return netStatus; }

int TCPNetSocket::getDomain(const INetAddress &address) {

	int domain = 0; // TODO be override by the NetAddress!
	switch (address.getNetworkProtocol()) {
	case INetAddress::NetworkProtocol::NetWorkProtocol_IP: {
		const IPAddress &ipAddress = static_cast<const IPAddress &>(address);
		domain = AF_INET;
		if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV4)
			return AF_INET;
		else if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV6)
			return AF_INET6;
	} break;
	case INetAddress::NetworkProtocol::NetWorkProtocol_CAN:
		domain = AF_CAN;
		break;
	default:
		throw RuntimeException("Non Supported Network Protocol: {}", address.getNetworkProtocol());
	}
}