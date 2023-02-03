
#include "CANAddress.h"
#include "NetCANSocket.h"

using namespace fragcore;

int main(int argc, const char **argv) {

	try {

		/*	*/
		CANNetSocket net;
		CANAddress local(0);
		net.bind(local);
		net.enableFDFrames(true);

		/*	*/
		char data[32];
		int nr;

		/*	*/
		do {
			CANAddress recvAddr(0);
			int c = net.recvfrom((uint8_t *)data, sizeof(data), nr, recvAddr) / 1;
			if (c < 0) {
				break;
			}
			printf("Adddress: 0x%x - ", recvAddr.getID());
			for (int i = 0; i < c; i++){
				printf("%c", data[i]);
			}
			printf("\n");
			sleep(1);
			net.sendto((const uint8_t *)data, sizeof(data), nr, recvAddr);
		} while (true);
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex) << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Bye Bye" << std::endl;
	return EXIT_SUCCESS;
}