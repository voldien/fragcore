#include "Core/Network/IPInterface.h"

#include <ifaddrs.h>
#include <sys/types.h>

using namespace fragcore;

IPInterface::IPInterface(const std::string &ip, Type type) {}

std::vector<IPInterface> IPInterface::getLocalAddresses() { return {}; }
std::vector<IPInterface> IPInterface::getLocalInterface() {

	struct ifaddrs *addrs, *tmp;

	getifaddrs(&addrs);
	tmp = addrs;

	while (tmp) {
		if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
			//printf("{}\n", tmp->ifa_name);
			// tmp->ifa_addr
		}

		tmp = tmp->ifa_next;
	}

	freeifaddrs(addrs);

	// struct ifreq ifr;				/*	*/
	// struct ifconf ifcon;			/*	*/
	// struct ifreq* ifcr;				/*	*/

	// /*	Get interface list associated with the socket.	*/
	// if(ioctl(connection->tcpsock, SIOCGIFCONF, &ifcon) < 0){
	// 	sntLogErrorPrintf("ioctl %s.\n", strerror(errno));
	// }
	return {};
}