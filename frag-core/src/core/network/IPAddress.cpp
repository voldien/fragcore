#include "Core/Network/IPAddress.h"
#include <arpa/inet.h>
#include <netdb.h>
using namespace fragcore;

IPAddress::IPAddress(const std::string &ip, IPAddressType type)
	: INetAddress(getNetworkProtocol()), ip(ip), type(type), valid(false) {
	struct hostent *hosten = nullptr; /*	*/
	/*	Get IP from hostname.	*/
	hosten = gethostbyname(ip.c_str());

	hosten = nullptr;
	if (hosten == nullptr) {
		int domain = getDomain(type);
		if (inet_pton(domain, ip.c_str(), &field8[0]) < 0) {
			throw RuntimeException("Bad");
		}

	} else {
		int domain = hosten->h_addrtype;
		for (char **address = hosten->h_addr_list; address != nullptr; address++) {
			// if (inet_pton(domain, *address, &field8[0]) < 0) {
			throw RuntimeException("Bad");
			//}
		}
		// hosten->h_addr_list;
	}
	/*	Get hostname	*/

	/*	Get hostname to ipaddress.	*/
}

IPAddress::IPAddress(const std::string &hostname)
	: INetAddress(getNetworkProtocol()), valid(false), type(IPAddressType::IPAddress_Type_NONE) {
	struct hostent *hosten = nullptr; /*	*/
									  /*	Get IP from hostname.	*/
	hosten = gethostbyname(hostname.c_str());
	if (hosten != nullptr) {
		int domain = hosten->h_addrtype;
		for (char **address = hosten->h_addr_list; *address != nullptr; address++) {
			if (inet_pton(domain, *address, &field8[0]) < 0) {
				throw RuntimeException("Failed to convert {} to Address", *address);
			} else {
				/*	TODO get domain.	*/
				valid = true;
				break;
			}
		}
	}
}

const uint8_t *IPAddress::getAddress(IPAddressType addressType) const noexcept { return field8; }

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