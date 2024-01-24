#ifndef FRAG_CORE_INTERNAL_IMP
#define FRAG_CORE_INTERNAL_IMP 1
#endif

#include "Core/IO/GZFileIO.h"
#include <fmt/core.h>
#include <zlib.h>
using namespace fragcore;

void GZFileIO::open(const char *path, IOMode mode) {
	// TODO change.
	FileIO::open(path, mode);

	// TODO add other access modes.
	const char *m = nullptr;
	switch (mode & ACCESS) {
	case READ:
		m = "rb";
		break;
	case WRITE:
		m = "wb";
		break;
	default:
		throw InvalidArgumentException("Invalid IO mode.");
	}

	/*  */
	this->gzFi = gzdopen(fileno(this->file), m);
	if (this->gzFi == nullptr) {
		int error;
		const char *errMsg = gzerror(this->gzFi, &error);
		throw RuntimeException("Failed to open {} - error: {} | {}", path, error, errMsg);
	}

	/*  Set buffer size.    */
	gzbuffer(this->gzFi, 8192);
	if (mode & WRITE) {
		// gzsetparams
	}
}

void GZFileIO::close() {
	int error;
	error = gzclose(this->gzFi);
	if (error != Z_OK) {
		FileIO::close();
		throw RuntimeException("Failed to close gzfile {}", zError(error));
	}
	FileIO::close();
}

long GZFileIO::read(long int nbytes, void *pbuffer) {
	long int nreadBytes = gzfread(pbuffer, 1, nbytes, this->gzFi);
	return nreadBytes;
}

long GZFileIO::write(long int nbytes, const void *pbuffer) {
	long int nWrittenBytes = gzfwrite(pbuffer, 1, nbytes, this->gzFi);
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

void GZFileIO::seek(long int nbytes, Seek seek) {
	// gzrewind
	FileIO::seek(nbytes, seek);
}

unsigned long GZFileIO::getPos() {
	// gzeof
	// gztell
	return FileIO::getPos();
}

bool GZFileIO::isWriteable() const { return FileIO::isWriteable(); }

bool GZFileIO::isReadable() const { return FileIO::isReadable(); }

bool GZFileIO::flush() {

	if (this->mode & WRITE) {
		int error;
		error = gzflush(this->gzFi, Z_FINISH);
		if (error != Z_OK) {
			throw RuntimeException(zError(error));
		}
		return true;
	}
	return false;
}

GZFileIO::GZFileIO(const char *path, IOMode mode) { this->open(path, mode); }

// GZFileIO::GZFileIO(Ref<IO> &io){

// }
