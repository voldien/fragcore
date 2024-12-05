#ifndef FRAG_CORE_INTERNAL_IMP
#define FRAG_CORE_INTERNAL_IMP 1
#endif

#include "GZFileIO.h"
#include <fmt/core.h>
#include <zlib.h>
using namespace fragcore;

void GZFileIO::open(const char *path, IOMode mode) {
	assert(this->file);

	// TODO add other access modes.
	const char *gz_mode = nullptr;
	switch (mode & ACCESS) {
	case READ:
		gz_mode = "rb";
		break;
	case WRITE:
		gz_mode = "wb";
		break;
	default:
		throw InvalidArgumentException("Invalid IO mode.");
	}

	/*  */
	this->gzFi = gzdopen(::fileno(this->file), gz_mode);
	if (this->gzFi == nullptr) {
		int error = 0;
		const char *errMsg = gzerror(this->gzFi, &error);
		throw RuntimeException("Failed to open {} - error: {} | {}", path, error, errMsg);
	}

	/*  Set buffer size.    */
	gzbuffer(this->gzFi, 1024 * 8);
	if (mode & WRITE) {
		// gzsetparams
	}
}

void GZFileIO::close() {
	int error = 0;
	error = gzclose(this->gzFi);
	FileIO::close();
	if (error != Z_OK) {
		throw RuntimeException("Failed to close gzfile {}", zError(error));
	}
}

long GZFileIO::read(long int nbytes, void *pbuffer) {
	z_size_t nreadBytes = gzfread(pbuffer, 1, nbytes, this->gzFi);
	return nreadBytes;
}

long GZFileIO::write(long int nbytes, const void *pbuffer) {
	z_size_t nWrittenBytes = gzfwrite(pbuffer, 1, nbytes, this->gzFi);
	if (nWrittenBytes == 0) {
		throw RuntimeException("Failed to write to  gz file {}", gzerror(this->gzFi, nullptr));
	}
	return nWrittenBytes;
}

long GZFileIO::length() {
	// TODO improve!
	unsigned long curPos = getPos();
	gzseek(this->gzFi, curPos, SEEK_SET);
	return FileIO::length();
}

bool GZFileIO::eof() const { return FileIO::eof(); }

void GZFileIO::seek(long int nbytes, const Seek seek) { FileIO::seek(nbytes, seek); }

unsigned long GZFileIO::getPos() { return FileIO::getPos(); }

bool GZFileIO::isWriteable() const { return FileIO::isWriteable(); }

bool GZFileIO::isReadable() const { return FileIO::isReadable(); }

bool GZFileIO::flush() {
	if (this->mode & WRITE) {
		int error = 0;
		error = gzflush(this->gzFi, Z_FINISH);
		if (error != Z_OK) {
			throw RuntimeException(zError(error));
		}
		return true;
	}
	return false;
}

GZFileIO::GZFileIO(const char *path, IOMode mode) : FileIO(path, mode) { this->open(path, mode); }
