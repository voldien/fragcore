
#include "Core/Network/TCPUDPAddress.h"
#include "NetModbusSocket.h"

using namespace fragcore;

int main(int argc, const char **argv) {

	/*	TODO get values from arguments	*/

	try {
		ModbusNetSocket net;
		IPAddress local("127.0.0.1", IPAddress::IPAddressType::IPAddress_Type_IPV4);
		TCPUDPAddress tcpAdddress(local, 1502);
		net.connect(tcpAdddress);

		int data[10];
		int nr;

		do {

			int c = net.recv(data, sizeof(data), nr);
			if (c < 0) {
				break;
			}
			for (int i = 0; i < c; i++)
				printf("%d", data[i]);
			printf("\n");
			sleep(1);
		} while (true);
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex) << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}