#include "Core/IO/SocketIO.h"

#include <fmt/core.h>
using namespace fragcore;

SocketIO::SocketIO(Ref<NetSocket> &socket) : netSocket(socket) {}

// SocketIO::SocketIO(SocketIO &&other) {
// 	this->file = std::exchange(other.file, nullptr);
// 	this->mode = other.mode;
// }

void SocketIO::open(const char *path, IOMode mode)  {}

void SocketIO::close() {}

long SocketIO::read(long int nbytes, void *pbuffer) {
	int nWriten;
	this->netSocket->recv(pbuffer, nbytes, nWriten);
	return nWriten;
}

long SocketIO::write(long int nbytes, const void *pbuffer) {
	int nWriten;
	this->netSocket->send(pbuffer, nbytes, nWriten);
	return nWriten;
}

long int SocketIO::peek(long int nBytes, void *pbuffer){
	// this->netSocket->read()
};

long SocketIO::length() { throw NotSupportedException(); }

bool SocketIO::eof() const { return false; }

void SocketIO::seek(long int nbytes, Seek seek) { throw NotSupportedException(); }

unsigned long SocketIO::getPos() { throw NotSupportedException(); }

bool SocketIO::isWriteable() const { return false; }

bool SocketIO::isReadable() const { return false; }

bool SocketIO::flush() { return false; }
