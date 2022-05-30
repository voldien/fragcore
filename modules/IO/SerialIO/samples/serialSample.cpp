
#include "SerialIO.h"
#include <Core/Log.h>

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

		SerialIO io(argv[1], IO::IOMode::ACCESS);
		io.setBaudRate(baudRate);

		io.write(sizeof(dataPayload), dataPayload);
		while (io.read(sizeof(dataPayload), dataPayload) > 0) {
			Log::log("%x", dataPayload[0]);
			sleep(1);
			io.write(sizeof(dataPayload), dataPayload);
		}
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex) << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}