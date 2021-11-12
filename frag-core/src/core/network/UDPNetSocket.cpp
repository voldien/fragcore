#include "Core/Network/INetAddress.h"
#include "Core/Network/UDPSocket.h"
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace fragcore;

UDPNetSocket::UDPNetSocket() : netStatus(NetStatus::Status_Disconnected), socket(0) {}
UDPNetSocket::~UDPNetSocket() {}

NetSocket::TransportProtocol UDPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TransportProtocolUDP;
}

int UDPNetSocket::close() { int status = ::close(socket); }

// int UDPNetSocket::bind(std::string &IPaddr, unsigned int port) {
// 	socklen_t addrlen;	   /*	*/
// 	struct sockaddr *addr; /*	*/
// 	union {
// 		struct sockaddr_in addr4;  /*	*/
// 		struct sockaddr_in6 addr6; /*	*/
// 	} addrU;
// 	// int domain = option->affamily;

// 	/*	Bind process to socket.	*/
// 	if (::bind(socket, (struct sockaddr *)addr, addrlen) < 0) {
// 		throw RuntimeException("Failed to bind UDP socket");
// 		// sntLogErrorPrintf("Failed to bind TCP socket, %s.\n", strerror(errno));
// 		// sntDisconnectSocket(connection);
// 	}
// }
int UDPNetSocket::listen(unsigned int maxListen) {
	if (::listen(socket, maxListen) < 0) {
		// sntLogErrorPrintf("listen failed, %s.\n", strerror(errno));
		// sntDisconnectSocket(connection);
	}
}

// int UDPNetSocket::connect(std::string &ip, unsigned int port) {
// 	// IPAddress ipNet(ip);
// 	// return this->connect(static_cast<const INetAddress &>(ipNet), port);
// }

int UDPNetSocket::connect(const INetAddress &p_addr) {
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
		throw RuntimeException("Failed to connect UDP socket");
		// sntLogErrorPrintf("Failed to connect TCP, %s.\n", strerror(errno));
		// sntDisconnectSocket(connection);
		// return NULL;
	}
}

int UDPNetSocket::bind(const INetAddress &p_addr) { return 0; }
int UDPNetSocket::poll(int p_type, int timeout) const { return 0; }
int UDPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) { return 0; }
int UDPNetSocket::recv( void *pbuffer, int p_len, int &sent, bool peek) { return 0; }
int UDPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) { return 0; }
int UDPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) { return 0; }
long int UDPNetSocket::send(const void *pbuffer, int p_len, int &sent) { return 0; }
Ref<NetSocket> UDPNetSocket::accept(INetAddress &r_ip) {}
UDPNetSocket::NetStatus UDPNetSocket::accept(NetSocket &socket) {}
int UDPNetSocket::read() { return 0; }
int UDPNetSocket::write() { return 0; }
bool UDPNetSocket::isBlocking() { return 0; }
void UDPNetSocket::setBlocking(bool blocking) {}
UDPNetSocket::NetStatus UDPNetSocket::getStatus() const noexcept { return this->netStatus; }