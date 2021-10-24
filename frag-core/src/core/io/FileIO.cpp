#include "Core/IO/FileIO.h"

#include <fmt/core.h>
#include <stdexcept>
#include <utility>

using namespace fragcore;

FileIO::FileIO() {
	this->mode = (IOMode)0;
	this->file = nullptr;
}

FileIO::FileIO(const char *path, IOMode mode) { this->open(path, mode); }

FileIO::FileIO(FILE *file) { this->file = file; }

FileIO::FileIO(FileIO &&other) {
	this->file = std::exchange(other.file, nullptr);
	this->mode = other.mode;
}

void FileIO::open(const char *path, IOMode mode) {

	if (path == nullptr)
		throw InvalidPointerException("path must not be null.");

	const char *m = nullptr;
	switch (mode & ACCESS) {
	case READ:
		m = "rb";
		break;
	case WRITE:
		if (mode & IO::IOMode::APPEND)
			m = "wb+";
		else
			m = "wb";
		break;
	case ACCESS:
		m = "ab+";
		break;
	default:
		throw InvalidArgumentException("Invalid IO mode {}", mode);
	}

	file = fopen(path, m);
	if (file == nullptr) {
		// TODO check the error
		switch (errno) {
		case ENOENT:
			throw InvalidArgumentException("Failed to open file {}, {}.\n", path, strerror(errno));
		case EPERM:	 // TODO add support for exception for permission.
		case EACCES: // TODO add support for exception for permission.
		default:
			throw RuntimeException("Failed to open file {}, {}.\n", path, strerror(errno));
		}
	}

	/*  keep the mode.  */
	this->mode = mode;
	this->setName(path);
}

void FileIO::close() {
	fclose(this->file);
	this->file = nullptr;
	this->mode = (IOMode)0;
}

long FileIO::read(long int nbytes, void *pbuffer) {
	long int nreadBytes;
	nreadBytes = fread((char *)pbuffer, 1, nbytes, this->file);
	return nreadBytes;
}

long FileIO::write(long int nbytes, const void *pbuffer) {
	long int nreadBytes;
	nreadBytes = fwrite((char *)pbuffer, 1, nbytes, this->file);
	if (nreadBytes != nbytes)
		throw RuntimeException("Failed to write to file, {}.\n", strerror(errno));

	int err = ferror(this->file);

	if (err != 0)
		throw RuntimeException("Failed to write to file, {}.\n", strerror(err));
	return nreadBytes;
}

long FileIO::length() {
	/*	Get size of the file.	*/
	long int prev = ftell(this->file);
	long int flen;

	/*  */
	fseek(this->file, 0, SEEK_END);
	flen = ftell(this->file);
	fseek(this->file, prev, SEEK_SET);
	return flen;
}

bool FileIO::eof() const { return feof(this->file) != 0; }

void FileIO::seek(long int nbytes, Seek seek) {
	int whence;
	switch (seek) {
	case SET:
		whence = SEEK_SET;
		break;
	case CUR:
		whence = SEEK_CUR;
		break;
	case END:
		whence = SEEK_END;
		break;
	default:
		throw InvalidArgumentException("Invalid seek enumerator.");
	}

	if (fseek(this->file, nbytes, whence) != 0) {
		if (ferror(this->file))
			throw RuntimeException(fmt::format("{}", strerror(errno)));
	}
}

unsigned long FileIO::getPos() {
	fpos_t pos;
	fgetpos(this->file, &pos);
	return pos.__pos;
}

bool FileIO::isWriteable() const { return this->mode & WRITE; }

bool FileIO::isReadable() const { return this->mode & READ; }

bool FileIO::flush() { return fflush(this->file) == 0; }
