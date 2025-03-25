#include "IO/BufferIO.h"

using namespace fragcore;

void BufferIO::open([[maybe_unused]] const char *path, [[maybe_unused]] IOMode mode) {}

void BufferIO::close() { /*	TODO reset values.	*/ }

long BufferIO::read(long int requestedBytes, void *pbuffer) {
	long int nBytesLeft = (this->nbytes - this->marker);
	long int nReadBytes = 0;

	/*	End of file.	*/
	if (nBytesLeft <= 0) {
		return 0;
	}

	if (nBytesLeft < requestedBytes) {
		/*	Clamp to the remaing part left.	*/
		nReadBytes = nBytesLeft;
	} else {
		/*	Fill the whole request.	*/
		nReadBytes = requestedBytes;
	}

	/*	Validate the state.	*/
	assert(this->marker < this->nbytes);
	assert(this->marker + nReadBytes <= this->nbytes);

	/*	*/
	std::memcpy(pbuffer, &this->buffer[this->marker], nReadBytes);

	/*	*/
	this->marker += nReadBytes;

	return nReadBytes;
}

long BufferIO::write(long int nbytes, const void *pbuffer) {
	if (this->isWriteable()) {
		unsigned long nWriteBytes = 0;
		memcpy(&this->buffer[this->marker], pbuffer, nWriteBytes);
		nWriteBytes = nbytes;

		return nWriteBytes;
	}
	throw RuntimeException(); // TODO determine if a another execption type is more appropicate.
}

long int BufferIO::peek(long int nBytes, void *pbuffer) {
	long nReadBytes = read(nBytes, pbuffer);
	return nReadBytes;
}

bool BufferIO::eof() const { return false; }

long BufferIO::length() { return this->nbytes; }

void BufferIO::seek(long int nbytes, const Seek seek) {
	switch (seek) {
	case SET:
		this->marker = nbytes;
		break;
	case CUR:
		this->marker += nbytes;
		break;
	case END:
		break;
	default:
		throw InvalidArgumentException("Invalid Seek Mode");
	}
}

unsigned long BufferIO::getPos() { return this->marker; }

bool BufferIO::isWriteable() const { return (this->ioMode & IOMode::WRITE) != 0; }

bool BufferIO::isReadable() const { return (this->ioMode & IOMode::READ) != 0; }

bool BufferIO::flush() { return true; }

BufferIO::BufferIO(const void *pBuffer, unsigned long size)
	: buffer((uint8_t *)pBuffer), nbytes(size), marker(0), expandable(false), ioMode(IOMode::READ) {}

BufferIO::BufferIO(void *pBuffer, unsigned long size)
	: buffer((uint8_t *)pBuffer), nbytes(size), marker(0), expandable(false),
	  ioMode(static_cast<IOMode>(IOMode::READ | IOMode::WRITE)) {}

BufferIO::BufferIO(unsigned long size, bool expandable)
	: buffer(static_cast<uint8_t *>(malloc(size))), nbytes(size), marker(0), expandable(expandable),
	  ioMode(static_cast<IOMode>(IOMode::READ | IOMode::WRITE)) {}

BufferIO::~BufferIO() {
	if (this->expandable) {
		free(this->buffer);
	}
}