#include "Core/IO/SocketIO.h"

#include <fmt/core.h>
using namespace fragcore;

SocketIO::SocketIO(Ref<NetSocket> &socket) : netSocket(socket) {}

// SocketIO::SocketIO(void) {
// 	this->mode = (IOMode)0;
// 	this->file = nullptr;
// }

// SocketIO::SocketIO(const char *path, IOMode mode) { this->open(path, mode); }

// SocketIO::SocketIO(FILE *file) { this->file = file; }

// SocketIO::SocketIO(SocketIO &&other) {
// 	this->file = std::exchange(other.file, nullptr);
// 	this->mode = other.mode;
// }

void SocketIO::open(const char *path, IOMode mode) {}

void SocketIO::close(void) {}

long SocketIO::read(long int nbytes, void *pbuffer) {}

long SocketIO::write(long int nbytes, const void *pbuffer) {
	int nWriten;
	this->netSocket->send(pbuffer, nbytes, nWriten);
	return nWriten;
}

long SocketIO::length(void) { throw NotSupportedException(); }

bool SocketIO::eof(void) const { return false; }

void SocketIO::seek(long int nbytes, Seek seek) { throw NotSupportedException(); }

unsigned long SocketIO::getPos(void) { throw NotSupportedException(); }

bool SocketIO::isWriteable(void) const { return false; }

bool SocketIO::isReadable(void) const { return false; }

bool SocketIO::flush(void) { return false; }
