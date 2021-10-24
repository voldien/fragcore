#include "Core/Network/IPAddress.h"
#include <netdb.h>
using namespace fragcore;

IPAddress::IPAddress(const std::string &ip, IPAddressType type)
	: INetAddress(getNetworkProtocol()), ip(ip), type(type) {
	struct hostent *hosten = NULL; /*	*/
	/*	Get IP from hostname.	*/
	hosten = gethostbyname(ip.c_str());
	if (hosten == NULL) {

	} else {
		int domain = hosten->h_addrtype;
		//hosten->h_addr_list;
	}
	/*	Get hostname	*/

	/*	Get hostname to ipaddress.	*/
}