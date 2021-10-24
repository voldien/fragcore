#include "Core/IO/ZipFileIO.h"
#include "Core/IO/ZipFileSystem.h"

#include <stdexcept>
#include <zip.h>
#include <zlib.h>
#include<fmt/core.h>
using namespace fragcore;

ZipFileIO::ZipFileIO(zip_file_t *file, zip_int64_t index, Ref<ZipFileSystem> ref) {
	this->file = file;
	this->zipfile = ref;
	this->index = index;
	this->zipfile->increment();
}

ZipFileIO::ZipFileIO() { this->file = nullptr; }

ZipFileIO::ZipFileIO(const ZipFileIO &other) { this->file = other.file; }

ZipFileIO::~ZipFileIO() {}

void ZipFileIO::open(const char *path, IOMode mode) {}

void ZipFileIO::close() {
	int err;

	err = zip_fclose(this->file);
	if (err != ZIP_ER_OK) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw RuntimeException(fmt::format("Failed to close zip file {}", buf));
	}

	// zip_file_add

	if (this->zipfile->deincreemnt()) {
		this->zipfile->release();
		delete this->zipfile.ptr();
	}
}

long ZipFileIO::read(long int nbytes, void *pbuffer) {
	nbytes = zip_fread(this->file, pbuffer, nbytes);
	if (nbytes == -1) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), nbytes, errno);
		throw RuntimeException(fmt::format("Failed to close zip file {}", buf));
	}
	return nbytes;
}

long ZipFileIO::write(long int nbytes, const void *pbuffer) {
	throw NotImplementedException();
	return nbytes;
}

bool ZipFileIO::eof() const { return false; }

long ZipFileIO::length() {
	zip_int64_t prev;
	zip_int64_t len;

	struct zip_stat state;
	// TODO add error check.
	zip_stat_index((zip_t *)this->zipfile->pzip, this->index, 0, &state);

	//	prev = zip_ftell(this->file);
	//
	//	zip_fseek(this->file, 0, SEEK_END);
	//	len = zip_ftell(this->file);
	//	zip_fseek(this->file, prev, 0);
	return state.size;
}

void ZipFileIO::seek(long int nbytes, Seek seek) {
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

	//	zip_int8_t err = zip_fseek(this->file, nbytes, whence);
	//	if (err != 0) {
	//		char buf[1024];
	//		int sys_err;
	////		zip_error_get(zip, &err, &sys_err);
	////		zip_error_to_str(buf, sizeof(buf), err, errno);
	////		throw InvalidArgumentException("can't open `{}':\n\t{}\n", path, buf));
	//	}
	//	//zip_fseek
}

unsigned long ZipFileIO::getPos() {
	return 0; // zip_ftell(this->file);
}

bool ZipFileIO::isWriteable() const {
	zip_uint32_t attr;
	// int err = zip_file_get_external_attributes((zip_t*)this->zipfile->getZipObject(), 0, ZIP_OPSYS_UNIX, nullptr,
	// &attr);

	return true;
}

bool ZipFileIO::isReadable() const {
	// ZIP_RDONLY
	return true;
}

bool ZipFileIO::flush() { return false; }
