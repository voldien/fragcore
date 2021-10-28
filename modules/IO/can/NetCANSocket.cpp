
#include "NetCANSocket.h"
#include <linux/can.h>
#include <linux/can/raw.h>

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
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

CANNetSocket::CANNetSocket() : netStatus(NetStatus::Status_Disconnected), socket(0) {}
CANNetSocket::CANNetSocket(int socket) : socket(socket) {}
CANNetSocket::~CANNetSocket() { this->close(); }

CANNetSocket::TransportProtocol CANNetSocket::getTransportProtocol() const noexcept {
	return CANNetSocket::TransportProtocol::TransportProtocolCAN;
}

int CANNetSocket::close() {
	int status = ::close(this->socket);
	if (status != 0)
		throw RuntimeException("Failed to close socket: {}", strerror(errno));
	this->socket = 0;
	netStatus = NetStatus::Status_Disconnected;
}

int CANNetSocket::bind(std::string &IPaddr, unsigned int port) {}

int CANNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {
	struct sockaddr_can addr;
	struct ifreq ifr;

	this->socket = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);

	strcpy(ifr.ifr_name, "can0");
	ioctl(this->socket, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	::bind(this->socket, (struct sockaddr *)&addr, sizeof(addr));
}

int CANNetSocket::listen(unsigned int maxListen) {
	if (::listen(this->socket, maxListen) < 0) {
		SystemException ex(errno, "TCP socket: Failed to set listen {} - error: {}", maxListen, strerror(errno));
	}
	return 0;
}
int CANNetSocket::connect(std::string &ip, unsigned int port) {}

int CANNetSocket::connect(const INetAddress &p_addr, uint16_t p_port) {
	this->socket = ::socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
	struct sockaddr_can addr;
	struct ifreq ifr;

	strcpy(ifr.ifr_name, "can0");
	ioctl(this->socket, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	::connect(this->socket, (struct sockaddr *)&addr, sizeof(addr));

	this->netStatus = NetStatus::Status_Done;
}

int CANNetSocket::open(int p_type, int ip_type) {}

int CANNetSocket::poll(int p_type, int timeout) const {}
int CANNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, uint16_t &r_port,
						   bool p_peek) {
	int flag = 0;
	int res; //= recvfrom(this->socket, p_buffer, p_len, flag, connection->intaddr, &r_read);
	return res;
}
int CANNetSocket::recv(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	int res = ::recv(this->socket, (void *)pbuffer, p_len, flag);
	return res;
}
int CANNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {
	int flag = 0;
	int res = ::send(this->socket, p_buffer, p_len, flag);
	return res;
}
int CANNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip, uint16_t p_port) {
	int res; //=  sendto(this->socket, p_buffer, p_len, flag, connection->extaddr, connection->sclen);
	return res;
}

long int CANNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	long int res = ::send(this->socket, pbuffer, p_len, flag);
	return res;
}

Ref<NetSocket> CANNetSocket::accept(INetAddress &r_ip, uint16_t &r_port) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		// close();
	}
	CANNetSocket *_newsocket = new CANNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}

Ref<NetSocket> CANNetSocket::accept(std::string &ip, unsigned int port) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		// close();
	}
	CANNetSocket *_newsocket = new CANNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}
CANNetSocket::NetStatus CANNetSocket::accept(NetSocket &socket) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
	}
}
int CANNetSocket::read() {}
int CANNetSocket::write() {}
bool CANNetSocket::isBlocking() { /*	*/
}
void CANNetSocket::setBlocking(bool blocking) { /*	*/
}
CANNetSocket::NetStatus CANNetSocket::getStatus() const noexcept { return this->netStatus; }

long int CANNetSocket::writeFrame(unsigned int ID, unsigned int nBytes, uint8_t *data) {
	struct can_frame frame;
	int nbytes;

	this->send(&frame, sizeof(struct can_frame), nbytes);

	if (nbytes < sizeof(struct can_frame)) {
	}
	return nbytes;
}

// int CANNetSocket::getDomain(const INetAddress &address) {

// 	int domain = 0; // TODO be override by the NetAddress!
// 	switch (address.getNetworkProtocol()) {
// 	case INetAddress::NetworkProtocol::NetWorkProtocol_IP: {
// 		const IPAddress &ipAddress = static_cast<const IPAddress &>(address);
// 		if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV4)
// 			return AF_INET;
// 		else if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV6)
// 			return AF_INET6;
// 	} break;
// 	case INetAddress::NetworkProtocol::NetWorkProtocol_CAN:
// 		return AF_CAN;
// 		break;
// 	default:
// 		throw RuntimeException("Non Supported Network Protocol: {}", address.getNetworkProtocol());
// 	}
// }