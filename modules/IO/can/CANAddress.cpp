#include "CANAddress.h"

using namespace fragcore;

CANAddress::CANAddress(const std::string &interface, unsigned int ID) : CANAddress(ID) {}

CANAddress::CANAddress(unsigned int ID)
	: INetAddress(NetworkProtocol::NetWorkProtocol_CAN), valid(false), id(ID), ifrIndex(0) {}