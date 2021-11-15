#include "Core/IO/FileIO.h"

#include <fmt/core.h>
#include <utility>

using namespace fragcore;

FileIO::FileIO() {
	this->mode = (IOMode)0;
	this->file = nullptr;
}

FileIO::FileIO(const char *path, IOMode mode) { this->open(path, mode); }

FileIO::FileIO(const std::string &path, IOMode mode) { this->open(path.c_str(), mode); }

FileIO::FileIO(FILE *file) { this->file = file; }

FileIO::FileIO(FileIO &&other) {
	this->file = std::exchange(other.file, nullptr);
	this->mode = other.mode;
}

void FileIO::open(const char *path, IOMode mode) {

	if (path == nullptr)
		throw InvalidPointerException("path must not be null.");

	const char *f_io_mode = nullptr;
	bool append = (mode & IO::IOMode::APPEND);
	switch (mode & ACCESS) {
	case READ:
		f_io_mode = "rb";
		break;
	case WRITE:
		if (append)
			f_io_mode = "wb+";
		else
			f_io_mode = "wb";
		break;
	case ACCESS:
		if (append)
			f_io_mode = "ab+";
		else
			f_io_mode = "ab";
		break;
	default:
		throw InvalidArgumentException("Invalid IO mode {}", mode);
	}

	file = fopen(path, f_io_mode);
	/*	Check if open was successful.	*/
	if (file == nullptr) {
		// TODO check the error
		switch (errno) {
		case ENOENT:
			throw InvalidArgumentException("Failed to open file {}, {}.\n", path, strerror(errno));
		case EPERM:	 // TODO add support for exception for permission.
		case EACCES: // TODO add support for exception for permission.
		case EBUSY:
		case ENFILE:
		default:
			throw RuntimeException("Failed to open file {}, {}.\n", path, strerror(errno));
		}
	}

	/*  keep the mode.  */
	this->mode = mode;
	this->setName(path);
}

void FileIO::close() {
	int rc = fclose(this->file);

	/*	Reset the value.	*/
	this->file = nullptr;
	this->mode = (IOMode)0;

	/*	Check the result.	*/
	switch (rc) {
	case EIO:
	case EBADF:
	case EINTR:
		break;
	default:
		break;
	}
}

long FileIO::read(long int nbytes, void *pbuffer) {
	long int nreadBytes;
	nreadBytes = fread((char *)pbuffer, 1, nbytes, this->file);
	if (nreadBytes < 0) {
	}
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

long int FileIO::peek(long int nBytes, void *pbuffer) {
	unsigned long cur_pos = this->getPos();

	long int nrBytes = this->read(nBytes, pbuffer);

	int rc = fseek(this->file, cur_pos, SEEK_SET);
	return nrBytes;
}

long FileIO::length() {
	/*	Get size of the file.	*/
	long int prev = ftell(this->file);
	if (prev < 0) {
	}
	long int flen;

	/*  */
	int rc = fseek(this->file, 0, SEEK_END);
	flen = ftell(this->file);
	rc = fseek(this->file, prev, SEEK_SET);
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

	int rc = fseek(this->file, nbytes, whence);
	if (rc != 0) {
		if (ferror(this->file))
			throw SystemException(errno, std::system_category(), fmt::format("{}", strerror(errno)));
	}
}

unsigned long FileIO::getPos() {
	fpos_t pos;
	int rc = fgetpos(this->file, &pos);
	return pos.__pos;
}

bool FileIO::isWriteable() const { return this->mode & WRITE; }

bool FileIO::isReadable() const { return this->mode & READ; }

bool FileIO::flush() {
	int rc = fflush(this->file);
	return rc == 0;
}
