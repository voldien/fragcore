#include"Core/IO/BufferIO.h"
using namespace fragcore;

void BufferIO::open(const char *path, IOMode mode) {

}

void BufferIO::close() {

}

long BufferIO::read(long int nbytes, void *pbuffer) {
	unsigned long nBytesLeft = this->nbytes - this->marker;
	unsigned long nReadBytes;
	if (nBytesLeft == 0)
		return 0;

	if (nBytesLeft < nbytes)
		nReadBytes = nBytesLeft;
	else
		nReadBytes = nbytes;
	memcpy(pbuffer, &this->buffer[this->marker], nReadBytes);

	this->marker += nReadBytes;
	return nReadBytes;
}

long BufferIO::write(long int nbytes, const void *pbuffer) {
	if(isWriteable()){
		unsigned long nWriteBytes = 0;
		memcpy(&this->buffer[this->marker], pbuffer, nWriteBytes);

		return nWriteBytes;
	}
	throw RuntimeException(); //TODO determine if a another execption type is more appropicate.
}

long int BufferIO::peek(long int nBytes, void *pbuffer) { long nReadBytes = read(nBytes, pbuffer);
	return nReadBytes;
}

bool BufferIO::eof() const {
	return false;
}

long BufferIO::length() {
	return this->nbytes;
}

void BufferIO::seek(long int nbytes, Seek seek) {
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

unsigned long BufferIO::getPos() {
	return this->marker;
}

bool BufferIO::isWriteable() const {
	return !this->readOnly;
}

bool BufferIO::isReadable() const {
	return true;
}

bool BufferIO::flush() {
	return true;
}

BufferIO::BufferIO(const void *pBuffer, unsigned long size) {
	this->marker = 0;
	this->nbytes = size;
	this->buffer = (char*)pBuffer;
	this->readOnly = true;
}

BufferIO::BufferIO(void* pBuffer, unsigned long size){
	this->marker = 0;
	this->nbytes = size;
	this->buffer = (char*)pBuffer;
	this->readOnly = false;
}

BufferIO::BufferIO(unsigned long size, bool expandable)
{
	this->marker = 0;
	this->nbytes = size;
	this->buffer = (char*)malloc(size);
	this->readOnly = false;
	this->expandable = expandable;
}

BufferIO::~BufferIO(){
	if(this->expandable)
		free(this->buffer);
}