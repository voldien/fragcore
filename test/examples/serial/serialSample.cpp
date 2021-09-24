
#include "Core/IO/SerialIO.h"
#include"Core/Log.h"

using namespace fragcore;

int main(int argc, const char **argv) {
	SerialIO io("/dev/ttyS0", IO::IOMode::READ);

	char *data[10];
	while(io.read(1, data) > 0){
		Log::log("%c", data[0]);
	}

	return EXIT_SUCCESS;
}