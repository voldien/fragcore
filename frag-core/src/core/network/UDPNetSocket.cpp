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

UDPNetSocket::UDPNetSocket() : netStatus(NetStatus::Status_Disconnected), socket(0) {}

UDPNetSocket::UDPNetSocket(int socket) : UDPNetSocket() {
	this->socket = socket;
	// TODO get the status of the socket.
	this->netStatus = NetStatus::Status_Done;
}
UDPNetSocket::~UDPNetSocket() { this->close(); }

NetSocket::TransportProtocol UDPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TransportProtocolUDP;
}

int UDPNetSocket::close() {
	if (this->socket > 0) {
		int rc = ::close(this->socket);
		if (rc != 0)
			throw RuntimeException("{}", strerror(errno));
		this->socket = 0;
	}
	netStatus = NetStatus::Status_Disconnected;
	return 0;
}
int UDPNetSocket::bind(const INetAddress &p_addr) { return 0; }

int UDPNetSocket::listen(unsigned int maxListen) {
	if (::listen(socket, maxListen) < 0) {
	}

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
	int flags = SOCK_STREAM;
	struct timeval tv;

	//if (!isValidNetworkAddress(p_addr)) {
	//	throw RuntimeException("Invalid Net Address");
	//}

	/*	*/
	const TCPUDPAddress *tcpAddress = dynamic_cast<const TCPUDPAddress *>(&p_addr);
	if (tcpAddress == nullptr)
		throw RuntimeException("Not a valid NetAddress Object");
	domain = 0; // this->getDomain(*tcpAddress);

	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("UDP socket - Failed to create socket {} - {}", domain, strerror(errno));
	}

	int option = 1;
	int rc = setsockopt(this->socket, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(int));
	if (rc == -1) {
		throw RuntimeException("Failed to set Socket Option");
	}

	// addrlen = setupIPAddress((struct sockaddr *)&addrU, *tcpAddress, tcpAddress->getPort());

	if (::connect(socket, addr, addrlen) < 0) {
		throw RuntimeException("Failed to connect UDP socket");
	}
	return 0;
}

int UDPNetSocket::poll(int p_type, int timeout) const { return 0; }
int UDPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) { return 0; }
int UDPNetSocket::recv(void *pbuffer, int p_len, int &sent, bool peek) { return 0; }
int UDPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) { return 0; }
int UDPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) { return 0; }
long int UDPNetSocket::send(const void *pbuffer, int p_len, int &sent) { return 0; }
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
