#include "ICompression.h"

using namespace fragcore;

ssize_t ICompression::inflate(Ref<IO> io_in, Ref<IO> out) {

	char buf[1024 * 4];
	long nbytes = 0;
	long dataSize = 0;
	while ((nbytes = io_in->read(sizeof(buf), buf)) > 0) {
		int inflate_size = this->inflate(buf, nbytes, buf, sizeof(buf));

		int outbytes = out->write(inflate_size, buf);
		if (outbytes > 0) {
			/*	*/
			dataSize += nbytes;
		} else {
			throw RuntimeException("Error while reading IO: {}", out->getName());
		}
	}
	return dataSize;
}

ssize_t ICompression::deflate(Ref<IO> io_in, Ref<IO> out) { return 0; }