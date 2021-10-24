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

TCPNetSocket::TCPNetSocket(const IPInterface &ip) : netStatus(NetStatus::Status_Disconnected), socket(0) {}
TCPNetSocket::~TCPNetSocket() {}

NetSocket::TransportProtocol TCPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TCP;
}

int TCPNetSocket::close() {
	int status = ::close(socket);
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

		this->close();
	}

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)addr, addrlen) < 0) {
		close();
		throw RuntimeException("Failed to bind TCP socket, {}.\n", strerror(errno));
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
}
int TCPNetSocket::listen(unsigned int maxListen) {
	if (::listen(socket, maxListen) < 0) {
		close();
		// sntLogErrorPrintf("listen failed, {}.\n", strerror(errno));
		// sntDisconnectSocket(connection);
		return NULL;
	}
}
int TCPNetSocket::connect(std::string &ip, unsigned int port) {
	socklen_t addrlen;			 /*	*/
	const struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	struct hostent *hosten = NULL; /*	*/
	int domain;
	struct timeval tv;

	if (::connect(socket, addr, addrlen) < 0) {
		// sntLogErrorPrintf("Failed to connect TCP, %s.\n", strerror(errno));
		// sntDisconnectSocket(connection);
		// return NULL;
	}

	/*	Set timeout for client.	*/
	// tv.tv_sec = 10;
	// tv.tv_usec = 0;
	// if (setsockopt(connection->udpsock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
	// }
}

int TCPNetSocket::open(int p_type, int ip_type) {}
int TCPNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {}
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
long int TCPNetSocket::send(const void *pbuffer, int p_len, int &sent) {}
Ref<NetSocket> TCPNetSocket::accept(INetAddress &r_ip, uint16_t &r_port) {}
Ref<NetSocket> TCPNetSocket::accept(std::string &ip, unsigned int port) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		// sntLogErrorPrintf("Failed to accept, %s.\n", strerror(errno));
		// sntDisconnectSocket(connection);
		close();
	}
}
TCPNetSocket::NetStatus TCPNetSocket::accept(NetSocket &socket) {}
int TCPNetSocket::read() {}
int TCPNetSocket::write() {}
bool TCPNetSocket::isBlocking() { /*	*/
}
void TCPNetSocket::setBlocking(bool blocking) { /*	*/
}
TCPNetSocket::NetStatus TCPNetSocket::getStatus() const noexcept { return netStatus; }