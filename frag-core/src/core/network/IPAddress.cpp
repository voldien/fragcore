#include "Core/Network/IPAddress.h"
#include <arpa/inet.h>
#include <netdb.h>
using namespace fragcore;

IPAddress::IPAddress() : INetAddress(), valid(false) {}

IPAddress::IPAddress(const std::string &ip, IPAddressType type) : INetAddress(), ip(ip), type(type), valid(false) {
	struct hostent *hosten = nullptr; /*	*/
	/*	Get IP from hostname.	*/
	//	hosten = gethostbyname(ip.c_str());

	int domain = getDomain(type);
	if (inet_pton(domain, ip.c_str(), &field8[0]) < 0) {
		throw RuntimeException("Failed to convert {} to IP Address", ip);
	}
	this->valid = true;
}

IPAddress::IPAddress(void *encoded, IPAddressType type) {
	//TODO: impl
}

IPAddress::IPAddress(const std::string &hostname)
	: INetAddress(), type(IPAddressType::IPAddress_Type_NONE), valid(false) {
	struct hostent *hosten = nullptr; /*	*/
									  /*	Get IP from hostname.	*/
	/*	*/
	hosten = gethostbyname(hostname.c_str());
	/*	*/
	if (hosten != nullptr) {
		struct in_addr **addr_list;
		int i;
		addr_list = (struct in_addr **)hosten->h_addr_list;

		for (i = 0; addr_list[i] != nullptr; i++) {
			// Return the first one;
			const char *ipAddress = inet_ntoa(*addr_list[i]);
			int domain = hosten->h_addrtype;
			if (inet_pton(domain, ipAddress, &field8[0]) != 1) {
				throw RuntimeException("Failed to convert '{}' to Address", hosten->h_name);
			} else {
				this->type = convertDomain2AddressType(domain);
				this->valid = true;
			}
		}

	} else {
		/*	*/
		throw RuntimeException("Failed to get address {} - {}", hostname, strerror(errno));
	}
}

NetInterface IPAddress::getInterface() {
	NetInterface interface;
	return interface;
}

const uint8_t *IPAddress::getAddress(IPAddressType addressType) const noexcept {
	switch (addressType) {
	default:
	case IPAddress::IPAddressType::IPAddress_Type_IPV4:
	case IPAddress::IPAddressType::IPAddress_Type_IPV6:
		return field8;
	}
}

bool IPAddress::isValid() const noexcept { return valid; }

unsigned int IPAddress::getDomain(IPAddressType addressType) noexcept {
	switch (addressType) {
	case IPAddressType::IPAddress_Type_IPV4:
		return AF_INET;
	case IPAddressType::IPAddress_Type_IPV6:
		return AF_INET6;
	default:
		return 0;
	}
}

IPAddress::IPAddressType IPAddress::convertDomain2AddressType(int domain) {
	switch (domain) {
	case AF_INET:
		return IPAddress::IPAddressType::IPAddress_Type_IPV4;
	case AF_INET6:
		return IPAddress::IPAddressType::IPAddress_Type_IPV6;
	default:
		return IPAddress::IPAddressType::IPAddress_Type_NONE;
	}
}