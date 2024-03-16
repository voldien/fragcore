#include "Core/IO/BufferIO.h"

using namespace fragcore;

void BufferIO::open([[maybe_unused]] const char *path, [[maybe_unused]] IOMode mode) {}

void BufferIO::close() { /*	TOOD reset values.	*/
}

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
	memcpy(pbuffer, &this->buffer[this->marker], nReadBytes);

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
		throw InvalidArgumentException("");
	}
}

unsigned long BufferIO::getPos() { return this->marker; }

bool BufferIO::isWriteable() const { return (this->ioMode & IOMode::WRITE) != 0; }

bool BufferIO::isReadable() const { return (this->ioMode & IOMode::READ) != 0; }

bool BufferIO::flush() { return true; }

BufferIO::BufferIO(const void *pBuffer, unsigned long size) {
	this->marker = 0;
	this->nbytes = size;
	this->buffer = (uint8_t *)pBuffer;
	this->ioMode = IOMode::READ;
	this->expandable = false;
}

BufferIO::BufferIO(void *pBuffer, unsigned long size) {
	this->marker = 0;
	this->nbytes = size;
	this->buffer = (uint8_t *)pBuffer;
	this->ioMode = static_cast<IOMode>(IOMode::READ | IOMode::WRITE);
	this->expandable = false;
}

BufferIO::BufferIO(unsigned long size, bool expandable) {
	this->marker = 0;
	this->nbytes = size;
	this->buffer = static_cast<uint8_t *>(malloc(size));
	this->ioMode = static_cast<IOMode>(IOMode::READ | IOMode::WRITE);
	this->expandable = expandable;
}

BufferIO::~BufferIO() {
	if (this->expandable) {
		free(this->buffer);
	}
}