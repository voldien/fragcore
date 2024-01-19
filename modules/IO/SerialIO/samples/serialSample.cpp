
#include "SerialIO.h"

using namespace fragcore;

int main(int argc, const char **argv) {
	try {
		int baudRate = 4800;
		unsigned int dataPayload[10];
		if (argc < 2) {
			std::cerr << "Must provide a Path to a valid serial device." << std::endl;
			return EXIT_FAILURE;
		}
		if (argc == 3) {
			baudRate = atoi(argv[2]);
		}

		SerialIO serialIO(argv[1], IO::IOMode::ACCESS);
		serialIO.setBaudRate(baudRate);

		serialIO.write(sizeof(dataPayload), dataPayload);
		while (serialIO.read(sizeof(dataPayload), dataPayload) > 0) {
			printf("%x", dataPayload[0]);
			sleep(1);
			serialIO.write(sizeof(dataPayload), dataPayload);
		}
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex) << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}