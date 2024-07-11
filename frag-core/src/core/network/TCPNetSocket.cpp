
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
TCPNetSocket::TCPNetSocket(int socket) : socket(socket) {
	// TODO set status.
}
TCPNetSocket::~TCPNetSocket() { this->close(); }

NetSocket::TransportProtocol TCPNetSocket::getTransportProtocol() const noexcept {
	return NetSocket::TransportProtocol::TransportProtocolTCP;
}

int TCPNetSocket::close() {
	/*	*/
	if (this->socket > 0) {
		int status = ::close(this->socket);
		if (status != 0) {
			throw RuntimeException("{}", strerror(errno));
		}
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
	if (!isValidNetworkAddress(p_addr)) {
		throw RuntimeException("Invalid Net Address");
	}

	/*	*/
	const TCPUDPAddress *tcpAddress = dynamic_cast<const TCPUDPAddress *>(&p_addr);
	if (tcpAddress == nullptr) {
		throw RuntimeException("Not a valid NetAddress Object");
	}

	/*	*/
	int domain = getDomain(*tcpAddress);
	addrlen = setupIPAddress((struct sockaddr *)&addrU, p_addr, tcpAddress->getPort());

	/*	*/
	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("TCP socket - Failed to create socket {} - {}", domain, strerror(errno));
	}

	this->setTimeout(0);

	/*	Bind process to socket.	*/
	if (::bind(socket, (struct sockaddr *)&addrU, addrlen) < 0) {
		this->netStatus = NetStatus::Status_Error;
		throw RuntimeException("Failed to bind TCP socket - {}:{}, {}", tcpAddress->getIPAddress().getIP(),
							   tcpAddress->getPort(), strerror(errno));
	}
	this->netStatus = NetStatus::Status_Done;

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

	/*	*/
	if (!isValidNetworkAddress(p_addr)) {
		throw RuntimeException("Invalid Net Address");
	}

	/*	*/
	const TCPUDPAddress *tcpAddress = dynamic_cast<const TCPUDPAddress *>(&p_addr);
	if (tcpAddress == nullptr) {
		throw RuntimeException("Not a valid NetAddress Object");
	}
	int domain = TCPNetSocket::getDomain(*tcpAddress);

	this->socket = ::socket(domain, flags, 0);
	if (this->socket < 0) {
		throw RuntimeException("TCP socket - Failed to create socket {} - {}", domain, strerror(errno));
	}

	int option = 1;
	int rcode = setsockopt(this->socket, IPPROTO_TCP, TCP_NODELAY, (const void *)&option, sizeof(int));
	if (rcode == -1) {
		throw RuntimeException("Failed to set Socket Option");
	}

	addrlen = setupIPAddress((struct sockaddr *)&addrU, *tcpAddress, tcpAddress->getPort());

	rcode = ::connect(socket, (struct sockaddr *)&addrU, addrlen);
	if (rcode != 0) {
		throw RuntimeException("Failed to connect TCP {}:{}, {}", tcpAddress->getIPAddress().getIP(),
							   tcpAddress->getPort(), strerror(errno));
	}
	this->netStatus = NetStatus::Status_Done;

	return 0;
}

int TCPNetSocket::poll(int p_type, int timeout) const { /*	select or poll.	*/
	return 0;
}

int TCPNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) {
	int flag = 0;
	socklen_t addrlen; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	if (p_peek) {
		flag |= MSG_PEEK;
	}

	int res = ::recvfrom(this->socket, p_buffer, p_len, flag, reinterpret_cast<sockaddr *>(&addrU), &addrlen);

	// TODO Add extract ip address.
	// r_ip = IPAddress(addrU.addr4, IPAddress::IPAddressType::IPAddress_Type_IPV4);

	r_read = res;

	return res;
}

int TCPNetSocket::recv(void *pbuffer, int p_len, int &sent, bool peek) {
	int flag = 0;

	if (peek) {
		flag |= MSG_PEEK;
	}
	int res = ::recv(this->socket, (void *)pbuffer, p_len, flag);
	sent = res;
	return res;
}

int TCPNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {
	int flag = 0;

	int res = ::send(this->socket, p_buffer, p_len, flag);
	// Check status of the send.
	r_sent = res;
	if (res < 0) {
	}
	return res;
}

int TCPNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_addr) {
	socklen_t addrlen; /*	*/
	union {
		struct sockaddr_in addr4;  /*	*/
		struct sockaddr_in6 addr6; /*	*/
	} addrU;
	const TCPUDPAddress &tcpAddress = static_cast<const TCPUDPAddress &>(p_addr);
	addrlen = setupIPAddress(reinterpret_cast<sockaddr *>(&addrU), tcpAddress.getIPAddress(), tcpAddress.getPort());
	unsigned int flag = 0;

	int res = ::sendto(this->socket, p_buffer, p_len, flag, reinterpret_cast<sockaddr *>(&addrU), addrlen);

	r_sent = res;

	return res;
}

long int TCPNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	ssize_t res = ::send(this->socket, pbuffer, p_len, flag);

	sent = res;

	return res;
}

Ref<NetSocket> TCPNetSocket::accept(INetAddress &r_ip) {
	struct sockaddr_storage addr;
	socklen_t addrlen;

	addrlen = sizeof(addr);
	int aaccept_socket = ::accept(this->socket, (struct sockaddr *)&addr, &addrlen);
	if (aaccept_socket < 0) {
		throw SystemException(errno, std::system_category(), "Failed to accept TCP connection");
	}

	// TODO assign r_ip
	IPAddress *ipAddress = reinterpret_cast<IPAddress *>(&r_ip);
	if (addr.ss_family == AF_INET) {
		struct sockaddr_in *address = reinterpret_cast<struct sockaddr_in *>(&addr);
		*ipAddress = IPAddress(&address->sin_addr, IPAddress::IPAddressType::IPAddress_Type_IPV4);
	}

	TCPNetSocket *_newsocket = new TCPNetSocket(aaccept_socket);
	return {_newsocket};
}

TCPNetSocket::NetStatus TCPNetSocket::accept(NetSocket &socket) {
	TCPUDPAddress addr;

	Ref<NetSocket> netSocket = this->accept(addr);
	// socket = std::move(netSocket);
	return netSocket->getStatus();
}

int TCPNetSocket::read() { return 0; }
int TCPNetSocket::write() { return 0; }
bool TCPNetSocket::isBlocking() { /*	*/
	int flags = fcntl(this->socket, F_GETFL, 0);
	if (flags == -1) {
		throw SystemException(errno, std::system_category(), "Failed to get flag");
	}
	return (flags & ~O_NONBLOCK) == 0;
}

void TCPNetSocket::setBlocking(bool blocking) { /*	*/

	/*	*/
	int flags = fcntl(this->socket, F_GETFL, 0);
	if (flags == -1) {
		throw SystemException(errno, std::system_category(), "Failed to get Socket Flag");
	}

	if (blocking) {
		flags = (flags & ~O_NONBLOCK);
	} else {
		flags = (flags & O_NONBLOCK);
	}

	int rcode = fcntl(this->socket, F_SETFL, flags);

	if (rcode < 0) {
		throw SystemException(errno, std::system_category(), "Failed to set Blocking State {}", blocking);
	}
}
TCPNetSocket::NetStatus TCPNetSocket::getStatus() const noexcept { return this->netStatus; }

bool TCPNetSocket::isValidNetworkAddress(const INetAddress &address) noexcept {
	return address.getNetworkProtocol() == INetAddress::NetworkProtocol::NetWorkProtocol_TCP_UDP && address.isValid();
}

void TCPNetSocket::setTimeout(long int microsec) {
	struct timeval timeout;

	/*	Set timeout for client.	*/
	timeout.tv_sec = microsec / 1000000;
	timeout.tv_usec = microsec % 1000000;

	int rcode = setsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	if (rcode != 0) {
		throw SystemException(errno, std::system_category(), "Failed to set recv timeout");
	}
	rcode = setsockopt(this->socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

	if (rcode != 0) {
		throw SystemException(errno, std::system_category(), "Failed to set send timeout");
	}
}

long int TCPNetSocket::getTimeout() {
	struct timeval timeout;
	socklen_t len;

	int rcode = getsockopt(this->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, &len);

	if (rcode != 0) {
		throw SystemException(errno, std::system_category(), "Failed to set recv timeout");
	}

	return timeout.tv_sec * 1E6L + timeout.tv_usec;
}

int TCPNetSocket::getSocket() const noexcept { return this->socket; }

int TCPNetSocket::getPort() const noexcept {
	struct sockaddr_in sin;
	socklen_t addrlen = sizeof(sin);
	getsockname(this->getSocket(), (struct sockaddr *)&sin, &addrlen); // read binding

	return ntohs(sin.sin_port); // get the port number
}

int TCPNetSocket::getDomain(const INetAddress &address) {

	const TCPUDPAddress &tcpAddress = dynamic_cast<const TCPUDPAddress &>(address);

	switch (address.getNetworkProtocol()) {
	case INetAddress::NetworkProtocol::NetWorkProtocol_TCP_UDP:
	case INetAddress::NetworkProtocol::NetWorkProtocol_IP: {
		const IPAddress &ipAddress = tcpAddress.getIPAddress();
		if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV4) {
			return AF_INET;
		}
		if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV6) {
			return AF_INET6;
		}

		throw RuntimeException("No valid IP address");

	} break;
	case INetAddress::NetworkProtocol::NetWorkProtocol_CAN:
		return AF_CAN;
		break;
	default:
		throw RuntimeException("Non Supported Network Protocol: {}", (unsigned int)address.getNetworkProtocol());
	}
}