#include "IO/SocketIO.h"
#include <fmt/core.h>

using namespace fragcore;

SocketIO::SocketIO(Ref<NetSocket> &socket) : netSocket(socket) {}

SocketIO::SocketIO(SocketIO &&other) { this->netSocket = std::exchange(other.netSocket, nullptr); }

void SocketIO::open([[maybe_unused]] const char *path,[[maybe_unused]] IOMode mode) {}

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

long int SocketIO::peek(long int nbytes, void *pbuffer) {
	int nWriten;
	this->netSocket->recv(pbuffer, nbytes, nWriten, true);
	return nWriten;
}

long SocketIO::length() { throw NotSupportedException(); }

bool SocketIO::eof() const { return false; }

void SocketIO::seek([[maybe_unused]] long int nbytes, [[maybe_unused]] const Seek seek) { throw NotSupportedException(); }

unsigned long SocketIO::getPos() { throw NotSupportedException(); }

bool SocketIO::isWriteable() const { return true; }

bool SocketIO::isReadable() const { return true; }

bool SocketIO::flush() { return true; }
