#include "Core/Network/UDPSocket.h"

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

UDPNetSocket::UDPNetSocket(const IPInterface &ip) : netStatus(NetStatus::Status_Disconnected), socket(0) {}
UDPNetSocket::~UDPNetSocket() {}

NetSocket::TransportProtocol UDPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::UDP;
}

int UDPNetSocket::close() { int status = ::close(socket); }

int UDPNetSocket::bind(std::string &IPaddr, unsigned int port) {
	socklen_t addrlen;	   /*	*/
	struct sockaddr *addr; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	// int domain = option->affamily;

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)addr, addrlen) < 0) {
		throw RuntimeException();
		// sntLogErrorPrintf("Failed to bind TCP socket, %s.\n", strerror(errno));
		// sntDisconnectSocket(connection);
	}
}
int UDPNetSocket::listen(unsigned int maxListen) {
	if (::listen(socket, maxListen) < 0) {
		// sntLogErrorPrintf("listen failed, %s.\n", strerror(errno));
		// sntDisconnectSocket(connection);
		return NULL;
	}
}

int UDPNetSocket::connect(std::string &ip, unsigned int port) {
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

int UDPNetSocket::connect(const INetAddress &p_addr, uint16_t p_port) {
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

int UDPNetSocket::open(int p_type, int ip_type) {}
int UDPNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {}
int UDPNetSocket::poll(int p_type, int timeout) const {}
int UDPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, uint16_t &r_port,
						   bool p_peek) {}
int UDPNetSocket::recv(const void *pbuffer, int p_len, int &sent) {}
int UDPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {}
int UDPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip, uint16_t p_port) {}
long int UDPNetSocket::send(const void *pbuffer, int p_len, int &sent) {}
Ref<NetSocket> UDPNetSocket::accept(INetAddress &r_ip, uint16_t &r_port) {}
Ref<NetSocket> UDPNetSocket::accept(std::string &ip, unsigned int port) {}
UDPNetSocket::NetStatus UDPNetSocket::accept(NetSocket &socket) {}
int UDPNetSocket::read() {}
int UDPNetSocket::write() {}
bool UDPNetSocket::isBlocking() {}
void UDPNetSocket::setBlocking(bool blocking) {}
UDPNetSocket::NetStatus UDPNetSocket::getStatus() const noexcept { return this->netStatus; }