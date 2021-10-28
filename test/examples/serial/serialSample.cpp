
#include "Core/Log.h"
#include "SerialIO.h"

using namespace fragcore;

int main(int argc, const char **argv) {
	try {
		SerialIO io("/dev/ttyS0", IO::IOMode::READ);

		char *data[10];
		while (io.read(1, data) > 0) {
			Log::log("%c", data[0]);
		}
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}