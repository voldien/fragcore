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

TCPNetSocket::TCPNetSocket(const IPInterface &ip) {
	// /*	Create socket if not already created.	*/
	// if ((protocol & SNT_TRANSPORT_TCP) && connection->tcpsock == 0) {
	// 	sntDebugPrintf("Create stream socket.\n");
	// 	connection->tcpsock = socket(affamily, SOCK_STREAM, 0);
	// 	if (connection->tcpsock < 0) {
	// 		sntLogErrorPrintf("Failed to create socket, %s.\n", strerror(errno));
	// 		return 0;
	// 	}
	// }
	// if ((protocol & SNT_TRANSPORT_UDP) && connection->udpsock == 0) {
	// 	sntDebugPrintf("Create datagram socket.\n");
	// 	connection->udpsock = socket(affamily, SOCK_DGRAM, IPPROTO_UDP);
	// 	if (connection->udpsock < 0) {
	// 		sntLogErrorPrintf("Failed to create socket, %s.\n", strerror(errno));
	// 		return 0;
	// 	}
	// }
}
TCPNetSocket::~TCPNetSocket() {}

NetSocket::TransportProtocol TCPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TCP;
}

int TCPNetSocket::close() { int status = ::close(socket); }

int TCPNetSocket::bind(std::string &IPaddr, unsigned int port) {
	socklen_t addrlen;	   /*	*/
	struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	// int domain = option->affamily;

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)addr, addrlen) < 0) {
		close();
		throw RuntimeException("Failed to bind TCP socket, {}.\n", strerror(errno));
	} else {
	}
}
int TCPNetSocket::listen(unsigned int maxListen) {
	if (::listen(socket, maxListen) < 0) {
		close();
		// sntLogErrorPrintf("listen failed, %s.\n", strerror(errno));
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
}

int TCPNetSocket::open(int p_type, int ip_type) {}
int TCPNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {}
int TCPNetSocket::poll(int p_type, int timeout) const {}
int TCPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, uint16_t &r_port,
						   bool p_peek) {}
int TCPNetSocket::recv(const void *pbuffer, int p_len, int &sent) {}
int TCPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {}
int TCPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip, uint16_t p_port) {}
long int TCPNetSocket::send(const void *pbuffer, int p_len, int &sent) {}
Ref<NetSocket> TCPNetSocket::accept(INetAddress &r_ip, uint16_t &r_port) {}
Ref<NetSocket> TCPNetSocket::accept(std::string &ip, unsigned int port) {}
TCPNetSocket::NetStatus TCPNetSocket::accept(NetSocket &socket) {}
int TCPNetSocket::read() {}
int TCPNetSocket::write() {}
bool TCPNetSocket::isBlocking() {}
void TCPNetSocket::setBlocking(bool blocking) {}
TCPNetSocket::NetStatus TCPNetSocket::getStatus() const noexcept { return netStatus; }