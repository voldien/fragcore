#include "IO/ZipFileIO.h"
#include "IO/ZipFileSystem.h"

#include <fmt/core.h>
#include <zip.h>
#include <zlib.h>
using namespace fragcore;

ZipFileIO::ZipFileIO(zip_file_t *file, zip_int64_t index, Ref<ZipFileSystem> ref) {
	this->file = file;
	this->index = index;
	this->zipfile = ref;
	this->zipfile->increment();
}

ZipFileIO::ZipFileIO() { this->file = nullptr; }

ZipFileIO::ZipFileIO(const ZipFileIO &other) { this->file = other.file; }

ZipFileIO::~ZipFileIO() {}

void ZipFileIO::open([[maybe_unused]] const char *path, [[maybe_unused]] IOMode mode) {}

void ZipFileIO::close() {
	int err;

	err = zip_fclose(this->file);
	if (err != ZIP_ER_OK) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw RuntimeException("Failed to close zip file {}", buf);
	}

	if (this->zipfile->deincreemnt()) {
		this->zipfile->release();
		delete this->zipfile.ptr();
	}
}

long ZipFileIO::read(long int nbytes, void *pbuffer) {
	zip_int64_t nread = zip_fread(this->file, pbuffer, nbytes);

	if (nread == -1) {
		throw RuntimeException("Failed to read zip file {} - number of bytes: {}", zip_file_strerror(this->file),
							   nbytes);
	}
	return nread;
}

long ZipFileIO::write([[maybe_unused]] long int nbytes, [[maybe_unused]] const void *pbuffer) {
	throw NotImplementedException();
	return nbytes;
}

long int ZipFileIO::peek(long int nBytes, void *pbuffer) { return this->read(nBytes, pbuffer); }

bool ZipFileIO::eof() const { return false; }

long ZipFileIO::length() {

	struct zip_stat state;

	int err = zip_stat_index((zip_t *)this->zipfile->pzip, this->index, 0, &state);
	if (err != ZIP_ER_OK) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw RuntimeException("Failed to get zip file size {}", buf);
	}

	return state.size;
}

void ZipFileIO::seek(long int nbytes, const Seek seek) {
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
		throw InvalidArgumentException("Invalid seek enumerator.");
	}

	zip_int8_t err = zip_fseek(this->file, nbytes, whence);
	if (err == -1) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw InvalidArgumentException("Can not seek {}", zip_file_strerror(this->file));
	}
}

unsigned long ZipFileIO::getPos() {

	// zip_int64_t tell = zip_ftell(this->file);
	// if (tell == -1) {
	//	//		throw InvalidArgumentException("Can not tell: {} - Path {}", zip_file_strerror(this->file),
	//	//									   zip_get_name((zip_t *)this->zipfile->getZipObject(), this->index, 0));
	//}
	return 0;
}

bool ZipFileIO::isWriteable() const {
	zip_uint32_t attr;
	int err = zip_file_get_external_attributes((zip_t *)this->zipfile->getZipObject(), this->index, ZIP_OPSYS_UNIX,
											   nullptr, &attr);

	if (err != ZIP_ER_OK) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw RuntimeException("Failed to close zip file {}", buf);
	}
	return (attr & ZIP_RDONLY);
}

bool ZipFileIO::isReadable() const {
	zip_uint32_t attr;
	int err = zip_file_get_external_attributes((zip_t *)this->zipfile->getZipObject(), this->index, ZIP_OPSYS_UNIX,
											   nullptr, &attr);

	if (err != ZIP_ER_OK) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw RuntimeException("Failed to close zip file {}", buf);
	}
	return true;
}

bool ZipFileIO::flush() { return false; }
