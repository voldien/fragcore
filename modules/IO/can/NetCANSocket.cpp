
#include "NetCANSocket.h"
#include "CANAddress.h"
#include <Core/Math.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace fragcore;

CANNetSocket::CANNetSocket() : socket(0), netStatus(NetStatus::Status_Disconnected) {}
CANNetSocket::CANNetSocket(int socket) : socket(socket) {}
CANNetSocket::~CANNetSocket() { this->close(); }

CANNetSocket::TransportProtocol CANNetSocket::getTransportProtocol() const noexcept {
	return CANNetSocket::TransportProtocol::TransportProtocolCAN;
}

int CANNetSocket::close() {
	int status = ::close(this->socket);
	if (status != 0)
		throw RuntimeException("Failed to close socket: {}", strerror(errno));
	this->socket = 0; /*	*/
	this->netStatus = NetStatus::Status_Disconnected;
	return 0;
}

// struct {
// 	struct bcm_msg_head msg_head;
// 	struct can_frame frame[4];
// } mytxmsg;

int CANNetSocket::bind(const INetAddress &p_addr) {

	/*	*/
	const CANAddress &canAddress = static_cast<const CANAddress &>(p_addr);

	int domain = this->getDomain(p_addr);

	this->socket = ::socket(domain, SOCK_RAW, CAN_RAW);
	if (this->socket < 0) {
		throw RuntimeException("Failed to create CAN socket, {}", strerror(errno));
	}

	struct sockaddr_can addr = {};
	size_t addrlen = this->setupAddress(reinterpret_cast<struct sockaddr *>(&addr), p_addr);

	int rc = ::bind(this->socket, (struct sockaddr *)&addr, addrlen);
	if (rc < 0) {
		this->netStatus = NetStatus::Status_Error;
		RuntimeException ex("Failed to bind CAN socket, {}", strerror(errno));
		throw ex;
	} else {
		this->netStatus = NetStatus::Status_Done;
	}
	return 0;
}

int CANNetSocket::listen(unsigned int maxListen) {
	if (::listen(this->socket, maxListen) < 0) {
		SystemException ex(errno, std::system_category(), "CAN socket: Failed to set listen {}", maxListen);
	}
	return 0;
}

int CANNetSocket::connect(const INetAddress &p_addr) {

	const CANAddress &canAddress = static_cast<const CANAddress &>(p_addr);
	int domain = getDomain(p_addr);

	/*	*/
	this->socket = ::socket(domain, SOCK_DGRAM, CAN_BCM);
	if (this->socket < 0) {
		throw RuntimeException("Failed to create CAN socket, {}", strerror(errno));
	}

	struct sockaddr_can addr = {};
	size_t addrlen = setupAddress(reinterpret_cast<struct sockaddr *>(&addr), p_addr);

	int rc = ::connect(this->socket, (struct sockaddr *)&addr, addrlen);
	if (rc != 0) {
		throw RuntimeException("Failed connect CAN socket, {}", strerror(errno));
	} else {
		this->netStatus = NetStatus::Status_Done;
	}

	this->netStatus = NetStatus::Status_Done;
	return 0;
}

int CANNetSocket::poll(int p_type, int timeout) const { /*	*/
	return 0;
}

int CANNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) {
	int flag = 0;
	CANAddress &canAddress = static_cast<CANAddress &>(r_ip);
	struct sockaddr_can addr;
	socklen_t len = sizeof(addr);

	struct canfd_frame frame;
	if (p_peek)
		flag |= MSG_PEEK;

	/*	*/
	int res = ::recvfrom(this->socket, &frame, sizeof(frame), flag, (sockaddr *)&addr, &len);
	if (res < 0) {
		// TODO add implement to handle. reconnect.
		switch (errno) {
		case EAGAIN:
		case ECONNREFUSED:
		case EINTR:
		case ENOTCONN:
			this->netStatus = NetStatus::Status_Partial;
			/* code */
			break;
		default: /*	Failure*/
			this->netStatus = NetStatus::Status_Error;
			break;
		}
	}

	/*	Check what kind of package was received.	*/
	if (res == CANFD_MTU) {
		r_read = std::min((size_t)p_len, (size_t)frame.len);

	} else if (res == CAN_MTU) {
		r_read = std::min((size_t)p_len, (size_t)frame.len);
	} else {
		throw RuntimeException("Invalid Frame");
		return -1;
	}
	memcpy(p_buffer, frame.data, r_read);

	canAddress = CANAddress(frame.can_id);
	return res;
}

int CANNetSocket::recv(void *pbuffer, int p_len, int &sent, bool peek) {
	int flag = 0;
	CANAddress addr(0);
	long res = this->recvfrom((uint8_t *)pbuffer, p_len, sent, addr, peek);
	return res;
}

int CANNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {

	CANAddress addr(0);
	int res = this->sendto(p_buffer, p_len, r_sent, addr);

	return res;
}

int CANNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) {
	int flag = 0;
	const CANAddress &canAddress = static_cast<const CANAddress &>(p_ip);

	/*	*/
	struct sockaddr_can addr = {};
	struct ifreq ifr;
	addr.can_ifindex = this->ifrIndex;
	addr.can_family = AF_CAN;

	const char *interface = "vcan0";
	strcpy(ifr.ifr_name, interface);
	if (ioctl(this->socket, SIOCGIFINDEX, &ifr) == 0) {
		addr.can_ifindex = ifr.ifr_ifindex;
	} else {
		throw SystemException(errno, std::system_category(), "Failed to set interface index {}", interface);
	}

	/*	*/
	struct can_frame frame;
	const uint32_t frameMaxBuffer = sizeof(frame.data);

	for (int i = 0; i < p_len; i += frameMaxBuffer) {

		const size_t nrByteLeft = p_len - i;
		frame.can_dlc = static_cast<unsigned char>(Math::min<size_t>(nrByteLeft, frameMaxBuffer));
		frame.can_id = canAddress.getID();
		/*	*/
		memcpy(&frame.data[0], &p_buffer[i], frame.can_dlc);

		int res = ::sendto(this->socket, &frame, sizeof(frame), flag, (sockaddr *)&addr, sizeof(addr));
		if (res != sizeof(frame))
			throw RuntimeException("Invalid Frame");
	}
	r_sent = p_len;
	return p_len;
}

long int CANNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	CANAddress addr(0);
	int res = this->sendto((uint8_t *)pbuffer, p_len, sent, addr);

	return res;
}

Ref<NetSocket> CANNetSocket::accept(INetAddress &r_ip) {
	struct sockaddr acceptAddr; /*	*/
	socklen_t aclen = 0;		/*	*/
	int aaccept_socket = ::accept(this->socket, &acceptAddr, &aclen);
	if (aaccept_socket < 0) {
		// close();
	}
	// acceptAddr.sa_family

	/*	Create net socket.	*/
	CANNetSocket *_newsocket = new CANNetSocket(aaccept_socket);
	/*	*/
	return Ref<NetSocket>(_newsocket);
}

CANNetSocket::NetStatus CANNetSocket::accept(NetSocket &socket) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
	}
}

int CANNetSocket::read() { return 0; }
int CANNetSocket::write() { return 0; }
bool CANNetSocket::isBlocking() { /*	*/
	int flags = fcntl(this->socket, F_GETFL, 0);
	if (flags == -1) {
		return false; // TODO validate.
	}
	return (flags & ~O_NONBLOCK) == 0;
}

void CANNetSocket::setBlocking(bool blocking) { /*	*/
	int flags = fcntl(this->socket, F_GETFL, 0);
	if (flags == -1) {
		//	return false;
	}
	flags = (flags & ~O_NONBLOCK);
	int rc = fcntl(this->socket, F_SETFL, flags);
	if (rc < 0) {
	}
}

CANNetSocket::NetStatus CANNetSocket::getStatus() const noexcept { return this->netStatus; }

long int CANNetSocket::writeFrame(unsigned int ID, size_t nBytes, uint8_t *data) {
	struct can_frame frame;
	int nbytes;

	frame.can_id = ID;
	frame.can_dlc = nBytes;
	long int nsent;

	this->send(&frame, sizeof(struct can_frame), nbytes);

	if (nbytes < sizeof(struct can_frame)) {
	}
	return nbytes;
}
void CANNetSocket::setFilter(std::vector<uint32_t> &ids) {
	std::vector<struct can_filter> rfilters(ids.size());
	for (unsigned int i = 0; i < ids.size(); i++) {
		// TODO improve
		rfilters[i].can_id = ids[i];
		rfilters[i].can_mask = (CAN_EFF_FLAG | CAN_RTR_FLAG | CAN_SFF_MASK);
	}

	int rc =
		setsockopt(this->socket, SOL_CAN_RAW, CAN_RAW_FILTER, rfilters.data(), sizeof(rfilters[0]) * rfilters.size());
	if (rc != 0) {
		throw SystemException(errno, std::system_category(), "Failed to set CAN Filter");
	}
}

// can_err_mask_t err_mask = (CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF);

// setsockopt(s, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));

size_t CANNetSocket::setupAddress(struct sockaddr *paddr, const INetAddress &p_addr) {

	struct sockaddr_can addr = {};
	size_t addrlen = sizeof(addr);
	struct ifreq ifr;

	const CANAddress &canAddress = static_cast<const CANAddress &>(p_addr);
	canAddress.getStandard();

	// TODO resolve
	const char *interface = "vcan0";
	strcpy(ifr.ifr_name, interface);
	if (ioctl(this->socket, SIOCGIFINDEX, &ifr) == 0) {
		this->ifrIndex = ifr.ifr_ifindex;
	} else {
		throw SystemException(errno, std::system_category(), "Failed to set interface index {}", interface);
	}

	addr.can_family = this->getDomain(p_addr);
	addr.can_ifindex = ifr.ifr_ifindex;

	memcpy(paddr, &addr, addrlen);

	return addrlen;
}

void CANNetSocket::enableFDFrames(bool enable) {

	int enable_canfd = 1;
	if (!enable)
		enable_canfd = 0;
	int rc = setsockopt(this->socket, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd));
	if (rc != 0) {
		throw SystemException(errno, std::system_category(), "Failed to set CAN Filter");
	}
}

size_t CANNetSocket::getFrameSize() const noexcept { return sizeof(struct can_frame); }

int CANNetSocket::getDomain(const INetAddress &address) { return AF_CAN; }