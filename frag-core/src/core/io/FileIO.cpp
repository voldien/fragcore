#include "IO/FileIO.h"
#include <fmt/core.h>
#include <magic_enum.hpp>
#include <sys/stat.h>
#include <utility>

using namespace fragcore;

FileIO::FileIO() : mode((IOMode)0) { this->file = nullptr; }

FileIO::FileIO(const char *path, IOMode mode) { FileIO::open(path, mode); }

FileIO::FileIO(const std::string &path, IOMode mode) { FileIO::open(path.c_str(), mode); }

FileIO::FileIO(FILE *file) : file(file), mode((IOMode)0) {

	/*	Extract stat of the file.	*/
	struct stat stat {};
	const int rcode = fstat(fileno(this->file), &stat);

	/*	Check file mode.	*/
	if (rcode == 0) {
		if (stat.st_mode & S_IWUSR) {
			this->mode = (IOMode)(this->mode | (int)IOMode::WRITE);
		}
		if (stat.st_mode & S_IRUSR) {
			this->mode = (IOMode)((int)IOMode::READ | this->mode);
		}
	}
}

FileIO::FileIO(FileIO &&other) : mode(other.mode) { this->file = std::exchange(other.file, nullptr); }

FileIO::~FileIO() { FileIO::close(); }

static const char *open_mode(const FileIO::IOMode mode) {
	const char *f_io_mode = nullptr;
	bool append = (mode & IO::IOMode::APPEND);
	switch (mode & FileIO::IOMode::ACCESS) {
	case FileIO::IOMode::READ:
		f_io_mode = "r";
		break;
	case FileIO::IOMode::WRITE:
		if (append) {
			f_io_mode = "w+";
		} else {
			f_io_mode = "w";
		}
		break;
	case FileIO::IOMode::ACCESS:
		if (append) {
			f_io_mode = "a+";
		} else {
			f_io_mode = "a";
		}
		break;
	default:
		throw InvalidArgumentException("Invalid IO mode: {}", magic_enum::enum_name(mode));
	}
	return f_io_mode;
}

void FileIO::open(const char *path, IOMode mode) {

	if (path == nullptr) {
		throw InvalidPointerException("path must not be null.");
	}

	const char *f_io_mode = open_mode(mode);

	this->file = fopen(path, f_io_mode);
	
	/*	Check if open was successful.	*/
	if (this->file == nullptr) {
		// TODO check the error
		switch (errno) {
		case ENOENT:
			throw InvalidArgumentException("Failed to open file {}, {}.", path, strerror(errno));
		case EPERM: // TODO add support for exception for permission.
			throw PermissionDeniedException("Failed to open file {}, {}.", path, strerror(errno));
		case EACCES: // TODO add support for exception for permission.
		case EBUSY:
		case ENFILE:
		default:
			throw RuntimeException("Failed to open file {}, {}.", path, strerror(errno));
		}
	}

	/*  keep the mode.  */
	this->mode = mode;
	this->setName(path);
}

FileIO::FileIO(int file_d, IOMode mode) {

	const char *f_io_mode = open_mode(mode);

	if (file_d == -1) {
		throw InvalidArgumentException("Failed to open file: {}.", strerror(errno));
	}

	this->file = fdopen(file_d, f_io_mode);

	/*	Check if open was successful.	*/
	if (this->file == nullptr) {
		// TODO check the error
		switch (errno) {
		case ENOENT:
			throw InvalidArgumentException("Failed to open file: {}.", strerror(errno));
		case EPERM: // TODO add support for exception for permission.
			throw PermissionDeniedException("Failed to open file: {}.", strerror(errno));
		case EACCES: // TODO add support for exception for permission.
		case EBUSY:
		case ENFILE:
		default:
			throw RuntimeException("Failed to open file: {}.", strerror(errno));
		}
	}
}

void FileIO::close() {
	if (this->file != nullptr) {
		int rcode = fclose(this->file);

		/*	Reset the value.	*/
		this->file = nullptr;
		this->mode = (IOMode)0;

		/*	Check the result.	*/
		switch (rcode) {
		case EIO:
		case EBADF:
		case EINTR:
			break;
		default:
			break;
		}
	}
}

long FileIO::read(long int nbytes, void *pbuffer) {
	long int nreadBytes = 0;
	nreadBytes = fread(pbuffer, 1, nbytes, this->file);
	if (nreadBytes < 0) {
		throw RuntimeException("Failed to read to file, {}.", strerror(errno));
	}
	return nreadBytes;
}

long FileIO::write(long int nbytes, const void *pbuffer) {
	long int nreadBytes = 0;

	nreadBytes = fwrite(pbuffer, 1, nbytes, this->file);
	if (nreadBytes != nbytes) {
		// throw RuntimeException("Failed to write to file, {}.", strerror(errno));
	}

	int err = ferror(this->file);

	if (err != 0) {
		throw RuntimeException("Failed to write to file, {}.", strerror(err));
	}
	return nreadBytes;
}

long int FileIO::peek(long int nBytes, void *pbuffer) {
	unsigned long cur_pos = this->getPos();

	long int nrBytes = this->read(nBytes, pbuffer);

	int rcode = fseek(this->file, cur_pos, SEEK_SET);
	if (rcode != 0) {
	}
	return nrBytes;
}

long FileIO::length() {
	struct stat stat {};
	fstat(fileno(this->file), &stat);
	return stat.st_size;
}

bool FileIO::eof() const { return feof(this->file) != 0; }

void FileIO::seek(long int nbytes, const Seek seek) {
	int whence = 0;
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
		throw InvalidArgumentException("Invalid seek enumerator");
	}

	int rcode = fseek(this->file, nbytes, whence);
	if (rcode != 0) {
		if (ferror(this->file)) {
			throw SystemException(errno, std::system_category(), fmt::format("{}", strerror(errno)));
		}
	}
}

unsigned long FileIO::getPos() {
	fpos_t pos;
	int rcode = fgetpos(this->file, &pos);
	if (rcode != 0) {
		throw SystemException(errno, std::system_category(), fmt::format("{}", strerror(errno)));
	}
	return pos.__pos;
}

bool FileIO::isWriteable() const { return (this->mode & WRITE) != 0; }

bool FileIO::isReadable() const { return (this->mode & READ) != 0; }

bool FileIO::flush() {
	int rcode = fflush(this->file);
	return rcode == 0;
}

void FileIO::setBlocking(bool blocking) {
	int saved_flags = fcntl(this->getFileDescriptor(), F_GETFL);
	int status = fcntl(this->getFileDescriptor(), F_SETFL, saved_flags & (blocking ? O_NONBLOCK : 0));
}