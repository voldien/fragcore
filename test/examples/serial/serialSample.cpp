
#include "Core/Log.h"
#include "SerialIO.h"

using namespace fragcore;

int main(int argc, const char **argv) {
	try {
		SerialIO io(argv[1], IO::IOMode::ACCESS);
		io.setBaudRate(4800);
		unsigned int data[10];

		io.write(sizeof(data), data);
		while (io.read(sizeof(data), data) > 0) {
			Log::log("%x", data[0]);
		}
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}