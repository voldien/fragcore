
#include "Core/Log.h"
#include "FTPFileSystem.h"

using namespace fragcore;

int main(int argc, const char **argv) {
	try {
		int port = 21;
		unsigned int dataPayload[10];
		if (argc < 2) {
			std::cerr << "Must provide a Path";
			return EXIT_FAILURE;
		}
		/*	Override default port.	*/
		if (argc >= 3) {
			port = atoi(argv[2]);
		}

		/*	Create FTP filesystem.	*/
		FTPFileSystem *ftp = FTPFileSystem::createFileSystem(argv[1], port);

		/*	Added credentials if provided.	*/
		if (argc >= 5) {
			ftp->setCredentials(argv[3], argv[4]);
		}

		/*	Display all files in directory.	*/
		std::vector<std::string> files = ftp->list("");
		for (size_t x = 0; x < files.size(); x++) {
			std::cout << files[x] << std::endl;
		}
		
	} catch (const std::exception &ex) {
		std::cerr << cxxexcept::getStackMessage(ex) << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}