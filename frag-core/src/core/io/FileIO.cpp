#include <stdexcept>
#include "Core/IO/FileIO.h"
#include "Utils/StringUtil.h"
#include "Exception/InvalidArgumentException.h"
#include"Exception/InvalidPointerException.h"
#include "Exception/RuntimeException.h"
#include"Core/IO/FileIO.h"
using namespace fragcore;
#include <filesystem>

FileIO::FileIO(void) {
	this->mode = (Mode) 0;
	this->file = NULL;
}

FileIO::FileIO(const char *path, Mode mode) {
	this->open(path, mode);
}

FileIO::FileIO(FILE *file) {
	this->file = file;
}

FileIO::FileIO(const FileIO &other) {
	this->file = other.file;
}

void FileIO::open(const char *path, Mode mode) {

	if (path == NULL)
		throw InvalidPointerException("path must not be null.");

	const char *m = NULL;
	switch (mode & ACCESS) {
		case READ:
			m = "rb";
			break;
		case WRITE:
			if (mode & IO::Mode::APPEND)
				m = "wb+";
			else
				m = "wb";
			break;
		case ACCESS:
			m = "ab+";
			break;
		default:
			throw InvalidArgumentException("Invalid IO mode.");
	}

	file = fopen(path, m);
	if (file == NULL) {
		//TODO check the error
		switch(errno){
			case ENOENT:
				throw InvalidArgumentException(fvformatf("Failed to open file %s, %s.\n", path,strerror(errno)));
			case EPERM://TODO add support for exception for permission.
			case EACCES://TODO add support for exception for permission.
			default:
				throw RuntimeException(fvformatf("Failed to open file %s, %s.\n", path, strerror(errno)));
		}
	}

	/*  keep the mode.  */
	this->mode = mode;
	this->setName(path);
}

void FileIO::close(void) {
	fclose(this->file);
	this->file = NULL;
	this->mode = (Mode) 0;
}

long FileIO::read(long int nbytes, void *pbuffer) {
	long int nreadBytes;
	nreadBytes = fread((char *) pbuffer, 1, nbytes, this->file);
	return nreadBytes;
}

long FileIO::write(long int nbytes, const void *pbuffer) {
	long int nreadBytes;
	nreadBytes = fwrite((char *) pbuffer, 1, nbytes, this->file);
	if (nreadBytes != nbytes && ferror(this->file) != 0)
		throw RuntimeException(fvformatf("Failed to write to file, %s.\n", strerror(errno)));
	return nreadBytes;
}

long FileIO::length(void) {
	/*	Get size of the file.	*/
	long int prev = ftell(this->file);
	long int flen;

	/*  */
	fseek(this->file, 0, SEEK_END);
	flen = ftell(this->file);
	fseek(this->file, prev, SEEK_SET);
	return flen;
}

bool FileIO::eof(void) const {
	return feof(this->file) != 0;
}

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
			throw RuntimeException(fvformatf("%s", strerror(errno)));
	}
}

unsigned long FileIO::getPos(void) {
	fpos_t pos;
	fgetpos(this->file, &pos);
	return pos.__pos;
}

bool FileIO::isWriteable(void) const {
	return this->mode & WRITE;
}

bool FileIO::isReadable(void) const {
	return this->mode & READ;
}

bool FileIO::flush(void) {
	return fflush(this->file) == 0;
}
