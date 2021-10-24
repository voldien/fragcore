#include "Core/Network/IPAddress.h"

using namespace fragcore;

IPAddress::IPAddress(const std::string &ip, IPAddressType type) : INetAddress(getNetworkProtocol()), ip(ip), type(type) {}