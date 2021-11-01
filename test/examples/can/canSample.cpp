
#include "CANAddress.h"
#include "Core/Log.h"
#include "NetCANSocket.h"

using namespace fragcore;

int main(int argc, const char **argv) {

	Log::setVerbosity(Log::VERBOSITY::Debug);
	try {
		CANNetSocket net;
		CANAddress local(0);
		net.bind(local);

		int data[10];
		int nr;
		uint16_t port;

		do {
			CANAddress recvAddr(0);
			unsigned int c = net.recvfrom((uint8_t *)data, sizeof(data), nr, recvAddr, port);
			if (c < 0)
				break;
			for (unsigned int i = 0; i < c; i++)
				printf("%x", data[i]);
			printf("\n");
			sleep(1);
			net.sendto((const uint8_t *)data, sizeof(data), nr, recvAddr, 0);
		} while (true);
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}