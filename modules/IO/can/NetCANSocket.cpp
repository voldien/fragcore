
#include "NetCANSocket.h"
#include "CANAddress.h"
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

// int CANNetSocket::bind(std::string &IPaddr, unsigned int port) {}

int CANNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {

	const CANAddress &canAddress = static_cast<const CANAddress &>(p_addr);
	struct sockaddr_can addr = {0};
	struct ifreq ifr;

	this->socket = ::socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (this->socket < 0) {
		throw RuntimeException("Failed to create CAN socket, {}", strerror(errno));
	}

	// TODO resolve
	strcpy(ifr.ifr_name, "vcan0");
	if (ioctl(this->socket, SIOCGIFINDEX, &ifr) == 0) {
		this->ifrIndex = ifr.ifr_ifindex;
	} else {
		throw SystemException(errno, "Failed to set interface index");
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	int rc = ::bind(this->socket, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0) {
		this->netStatus = NetStatus::Status_Error;
		RuntimeException ex("Failed to bind CAN socket, {}", strerror(errno));
		throw ex;
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
}

int CANNetSocket::listen(unsigned int maxListen) {
	// if (::listen(this->socket, maxListen) < 0) {
	// 	SystemException ex(errno, "TCP socket: Failed to set listen {} - error: {}", maxListen, strerror(errno));
	// }
	return 0;
}
// int CANNetSocket::connect(std::string &ip, unsigned int port) {}

int CANNetSocket::connect(const INetAddress &p_addr, uint16_t p_port) {

	const CANAddress &canAddress = static_cast<const CANAddress &>(p_addr);

	this->socket = ::socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
	if (this->socket < 0) {
		throw RuntimeException("Failed to create CAN socket, {}", strerror(errno));
	}
	struct sockaddr_can addr = {0};
	struct ifreq ifr;

	strcpy(ifr.ifr_name, "vcan0");
	if (ioctl(this->socket, SIOCGIFINDEX, &ifr) == 0) {
		this->ifrIndex = ifr.ifr_ifindex;
	} else {
		throw SystemException(errno, "Failed to set interface index");
	}

	addr.can_family = PF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	int rc = ::connect(this->socket, (struct sockaddr *)&addr, sizeof(addr));
	if (rc != 0) {
		throw RuntimeException("Failed connect CAN socket, {}", strerror(errno));
	} else {
		this->netStatus = NetStatus::Status_Done;
	}

	this->netStatus = NetStatus::Status_Done;
}

int CANNetSocket::open(int p_type, int ip_type) {}

int CANNetSocket::poll(int p_type, int timeout) const {}
int CANNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, uint16_t &r_port,
						   bool p_peek) {
	int flag = 0;
	CANAddress &canAddress = static_cast<CANAddress &>(r_ip);
	struct sockaddr_can addr = {0};
	socklen_t len = sizeof(addr);

	struct can_frame frame;
	static const uint32_t frameMaxBuffer = sizeof(frame.data);

	// TODO Recv all data.

	int res = ::recvfrom(this->socket, &frame, sizeof(frame), flag, (sockaddr *)&addr, &len);
	if (res != sizeof(frame))
		throw RuntimeException("Invalid Frame");
	memcpy(p_buffer, frame.data, frameMaxBuffer);
	canAddress = CANAddress(frame.can_id);
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
	int flag = 0;
	const CANAddress &canAddress = static_cast<const CANAddress &>(p_ip);

	/*	*/
	struct sockaddr_can addr = {0};
	addr.can_ifindex = this->ifrIndex;
	addr.can_family = AF_CAN;

	/*	*/
	struct can_frame frame = {0};
	static const uint32_t frameMaxBuffer = sizeof(frame.data);

	for (unsigned int i = 0; i < p_len; i += frameMaxBuffer) {

		/*	*/
		memcpy(frame.data, &p_buffer[i], p_len % frameMaxBuffer);
		frame.can_dlc = p_len % frameMaxBuffer;
		frame.can_id = canAddress.getID();

		int res = ::sendto(this->socket, &frame, sizeof(frame), flag, (sockaddr *)&addr, sizeof(addr));
		if (res != sizeof(frame))
			throw RuntimeException("Invalid Frame");
	}

	return p_len;
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

	frame.can_id = ID;
	frame.can_dlc = nBytes;

	this->send(&frame, sizeof(struct can_frame), nbytes);

	if (nbytes < sizeof(struct can_frame)) {
	}
	return nbytes;
}
void CANNetSocket::setFilter(std::vector<uint32_t> &ids) {
	std::vector<struct can_filter> rfilters(ids.size());
	for (unsigned int i = 0; i < ids.size(); i++) {
		rfilters[i].can_id = ids[i];
		rfilters[i].can_mask = 0xFF0;
	}

	int rc =
		setsockopt(this->socket, SOL_CAN_RAW, CAN_RAW_FILTER, rfilters.data(), sizeof(rfilters[0]) * rfilters.size());
	if (rc != 0) {
		throw SystemException(errno, "Failed to set CAN Filter");
	}
}