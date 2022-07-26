#include "CANAddress.h"

using namespace fragcore;

CANAddress::CANAddress(const std::string &interface, unsigned int ID) : CANAddress(ID) {}

CANAddress::CANAddress(unsigned int ID) : INetAddress(), valid(false), id(ID), ifrIndex(0) {}

const std::string CANAddress::getInterface() const {
	// struct ifreq ifr;
	// ifr.ifr_ifindex = ifrIndex;
	// ioctl(s, SIOCGIFNAME, &ifr);

	return "";
}