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

UDPNetSocket::UDPNetSocket(const IPInterface &ip) {
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
UDPNetSocket::~UDPNetSocket() {}

void UDPNetSocket::close() {}

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