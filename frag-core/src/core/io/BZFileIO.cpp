#include "IO/BZFileIO.h"
#include <bzlib.h>
#include <fmt/core.h>

using namespace fragcore;

long BZFileIO::read(long int nbytes, void *pbuffer) {
	int bzerror;
	int len = BZ2_bzRead(&bzerror, this->bzFile, pbuffer, nbytes);
	if (bzerror != BZ_OK) {
		throw RuntimeException("Failed to read {}", BZ2_bzerror(this->bzFile, &bzerror));
	}
	return len;
}

long BZFileIO::write(long int nbytes, const void *pbuffer) {
	int bzerror;
	int len = nbytes;
	BZ2_bzWrite(&bzerror, this->bzFile, (void *)pbuffer, static_cast<int>(nbytes));
	if (bzerror != BZ_OK) {
		throw RuntimeException("Failed to write {}", BZ2_bzerror(this->bzFile, &bzerror));
	}
	return len;
}

long BZFileIO::length() { return FileIO::length(); }

bool BZFileIO::eof() const { return FileIO::eof(); }

void BZFileIO::seek(long int nbytes, const IO::Seek seek) { FileIO::seek(nbytes, seek); }

unsigned long BZFileIO::getPos() { return FileIO::getPos(); }

bool BZFileIO::isWriteable() const { return FileIO::isWriteable(); }

bool BZFileIO::isReadable() const { return FileIO::isReadable(); }

bool BZFileIO::flush() { return BZ2_bzflush(this->bzFile); }

void BZFileIO::close() {
	int bzerror;
	BZ2_bzReadClose(&bzerror, this->bzFile);

	unsigned int nbyte_in, nbytes_out;
	BZ2_bzWriteClose(&bzerror, this->bzFile, 0, &nbyte_in, &nbytes_out);

	FileIO::close();
}

void BZFileIO::open([[maybe_unused]] const char *path, IO::IOMode mode) {

	/*  */
	int bzerror;
	switch (mode & IO::IOMode::ACCESS) {
	case IO::IOMode::READ:
		this->bzFile = BZ2_bzReadOpen(&bzerror, this->file, 0, 0, nullptr, 0);
		break;
	case IO::IOMode::WRITE:
		this->bzFile = BZ2_bzWriteOpen(&bzerror, this->file, 0, 0, 0);
		break;
	case IO::IOMode::ACCESS:
		break;
	default:
		break;
	}

	/*  */
	if (this->bzFile == nullptr) {
		throw RuntimeException("Failed to open {}", BZ2_bzerror(this->bzFile, &bzerror));
	}
}

BZFileIO::BZFileIO(const char *path, IO::IOMode mode) : FileIO(path, mode) { this->open(path, mode); }