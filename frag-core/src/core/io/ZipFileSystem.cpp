#include "IO/ZipFileSystem.h"
#include "IO/BufferIO.h"
#include "IO/ZipFileIO.h"

#include <cassert>
#include <fmt/core.h>
#include <vector>
#include <zip.h>
#include <zlib.h>
using namespace fragcore;

IO *ZipFileSystem::openFile(const char *path, [[maybe_unused]] IO::IOMode mode) {
	struct zip_file *zfile;
	struct zip_stat stat;
	struct zip *zip = (struct zip *)this->pzip;
	int err, sys_err;
	zip_uint32_t attr;

	// TODO move to the index open method.

	zip_int64_t index = zip_name_locate(zip, path, 0);
	if (index == -1) {
		throw InvalidArgumentException("Failed to locate: {}", path);
	}

	zip_uint8_t osSys = 0;
	err = zip_file_get_external_attributes((zip_t *)zip, index, 0, &osSys, &attr);
	if (err != 0) {
		char errorStr[1024];
		zip_error_get(zip, &err, &sys_err);
		zip_error_to_str(errorStr, sizeof(errorStr), err, errno);
		throw InvalidArgumentException("Failed to get external attributes `{}':\n\t{}", path, errorStr);
	}

	/*	*/
	err = zip_stat_index(zip, index, 0, &stat);
	if (err != 0) {
		char errorStr[1024];
		zip_error_to_str(errorStr, sizeof(errorStr), err, errno);
		throw InvalidArgumentException("Failed to get stats from `{}':\n\t{}", path, errorStr);
	}

	// TODO check if encryption and compression.

	/*	Open file inside zip.	*/
	zfile = zip_fopen_index(zip, index, 0);
	if (!zfile) {
		char errorStr[1024];
		zip_error_get(zip, &err, &sys_err);
		zip_error_to_str(errorStr, sizeof(errorStr), err, sys_err);
		throw InvalidArgumentException("Failed to open, via index `{}':\n\t{}", path, zip_strerror(zip));
	}

	return new ZipFileIO(zfile, index, Ref<ZipFileSystem>(this));
}

IO *ZipFileSystem::openFile(unsigned int index) {
	struct zip_file *zfile;
	struct zip_stat stat;

	int err;

	/*	*/
	err = zip_stat_index((struct zip *)this->pzip, index, 0, &stat);
	if (err != 0) {
		char errorStr[1024];
		zip_error_to_str(errorStr, sizeof(errorStr), err, errno);
		throw InvalidArgumentException("Failed to get stats from `{}':\n\t{}", index, errorStr);
	}

	zfile = zip_fopen_index((struct zip *)this->pzip, index, 0);
	if (!zfile) {
		char errorStr[1024];
		err = zip_get_error((struct zip *)this->pzip)->zip_err;
		zip_error_to_str(errorStr, sizeof(errorStr), err, errno);
		throw InvalidArgumentException("Failed to open file, via index `{}':\n\t{}", index, errorStr);
	}

	return new ZipFileIO(zfile, index, Ref<ZipFileSystem>(this));
}

bool ZipFileSystem::isASyncSupported() const { return this->getScheduler() != nullptr; }

bool ZipFileSystem::isDirectory([[maybe_unused]] const char *path) const { return false; }
bool ZipFileSystem::isFile([[maybe_unused]] const char *path) const { return false; }

std::vector<std::string> ZipFileSystem::listFiles(const char *path) const {

	struct zip *zip = static_cast<struct zip *>(this->pzip);

	std::vector<std::string> files;

	/*	Attempt to open the file by file path.	*/
	zip_int64_t index = zip_name_locate(zip, path, 0);
	if (index < 0) {
		throw InvalidArgumentException("{}", path);
	}

	zip_int64_t num_entries = zip_get_num_entries(zip, 0);

	files.resize(num_entries);

	for (zip_uint64_t i = 0; i < (zip_uint64_t)num_entries; i++) {
		const char *name = zip_get_name(zip, i, 0);
		if (name == nullptr) {
		}

		if (strcmp(path, name) == 0) {
			files[i] = name;
		}
	}

	return files;
}

std::vector<std::string> ZipFileSystem::listDirectories(const char *path) const {
	struct zip *zip = static_cast<struct zip *>(this->pzip);

	std::vector<std::string> files;

	/*	Attempt to open the file by file path.	*/
	zip_int64_t index = zip_name_locate(zip, path, 0);
	if (index < 0) {
		throw InvalidArgumentException("{}", path);
	}

	zip_int64_t num_entries = zip_get_num_entries(zip, 0);

	files.resize(num_entries);

	for (zip_uint64_t i = 0; i < (zip_uint64_t)num_entries; i++) {
		const char *name = zip_get_name(zip, i, 0);

		if (name == nullptr) {
		}

		if (strcmp(path, name) == 0) {
			files[i] = name;
		}
	}

	return files;
}

std::vector<std::string> ZipFileSystem::list(const char *path) const {
	struct zip *zip = static_cast<struct zip *>(this->pzip);

	std::vector<std::string> files;

	/*	Attempt to open the file by file path.	*/
	zip_int64_t index = zip_name_locate(zip, path, 0);
	if (index < 0) {
		throw InvalidArgumentException("{}", path);
	}

	zip_int64_t num_entries = zip_get_num_entries(zip, 0);

	files.resize(num_entries);

	for (zip_uint64_t i = 0; i < (zip_uint64_t)num_entries; i++) {
		const char *name = zip_get_name(zip, i, 0);
		if (name == nullptr) {
		}

		if (strcmp(path, name) == 0) {
			files[i] = name;
		}
	}

	return files;
}

void ZipFileSystem::release() {
	if (this->deincreemnt()) {
		if (this->pzip) {
			zip_close((zip_t *)this->pzip);
		}
	}
	this->pzip = nullptr;
}

void ZipFileSystem::closeFile([[maybe_unused]] IO *io) {}

void ZipFileSystem::remove(const char *path) {
	int err;

	// Find the file.
	zip_int64_t index = zip_name_locate((struct zip *)this->pzip, path, 0);
	if (index < 0) {
		char errorStr[4096];
		zip_error_to_str(errorStr, sizeof(errorStr), index, errno);
		throw InvalidArgumentException("Could not find file {}, error: {}", path, errorStr);
	}

	err = zip_delete((zip_t *)this->pzip, index);
	if (err != ZIP_ER_OK) {
		char errorStr[4096];
		zip_error_to_str(errorStr, sizeof(errorStr), err, errno);
		throw InvalidArgumentException("Could not delete file {}, error: {}", path, errorStr);
	}
}

void ZipFileSystem::rename(const char *oldPath, const char *newPath) {

	zip_int64_t index = zip_name_locate((struct zip *)this->pzip, oldPath, 0);
	if (index < 0) {
		char errorStr[4096];
		zip_error_to_str(errorStr, sizeof(errorStr), index, errno);
		throw InvalidArgumentException("Could not find file {}, error: {}", oldPath, errorStr);
	}

	zip_int64_t error = zip_rename(nullptr, index, newPath);
	if (error != ZIP_ER_OK) {
		char errorStr[4096];
		zip_error_to_str(errorStr, sizeof(errorStr), error, errno);
		throw InvalidArgumentException("Could not rename file {}, error: {}", oldPath, errorStr);
	}
}

void ZipFileSystem::createFile(const char *path) {

	zip_int64_t error = zip_add((zip_t *)this->pzip, path, nullptr);
	if (error != ZIP_ER_OK) {
		char errorStr[4096];
		zip_error_to_str(errorStr, sizeof(errorStr), error, errno);
		throw InvalidArgumentException("Could not create file {}, error: {}", path, errorStr);
	}
}

void ZipFileSystem::createDirectory(const char *path) {
	zip_int64_t error = zip_dir_add((zip_t *)this->pzip, path, 0);
	if (error != ZIP_ER_OK) {
		char errorStr[4096];
		zip_error_to_str(errorStr, sizeof(errorStr), error, errno);
		throw InvalidArgumentException("Could not create Directory {}, error: {}", path, errorStr);
	}
}

bool ZipFileSystem::isReadable([[maybe_unused]] const char *path) const {
	// err = zip_file_get_external_attributes((zip_t*)zip, index, 0, &osSys, &attr);
	return false;
}

bool ZipFileSystem::isWriteable([[maybe_unused]] const char *path) const {
	// err = zip_file_get_external_attributes((zip_t*)zip, index, 0, &osSys, &attr);
	return false;
}

bool ZipFileSystem::exists(const char *path) const {
	struct zip_stat stat;
	int err;

	/*	*/
	err = zip_stat((struct zip *)this->pzip, path, 0, &stat);
	if (err != ZIP_ER_OK) {
		return false;
	}
	return true;
}

ZipFileSystem *ZipFileSystem::createZipFileObject(const char *cfilename, Ref<IScheduler> ref) {

	int err;
	struct zip *zip;
	ZipFileSystem *zipfile;

	/*	Attempt to open the file by file path.	*/
	zip = zip_open(cfilename, 0, &err);
	if (zip == nullptr) {

		char errorStr[4096];
		/*	Display error and return.	*/
		zip_error_to_str(errorStr, sizeof(errorStr), err, errno);

		if (err == ZIP_ER_NOENT) {
			throw InvalidArgumentException("Can not open/create zip file {} - {}", cfilename, errorStr);
		}
		throw RuntimeException("Failed to open Zip {} - {}", cfilename, errorStr);
	}

	/*	Create object and store zip pointer.	*/
	zipfile = new ZipFileSystem(ref);
	zipfile->pzip = zip;

	/*	*/
	return zipfile;
}

static zip_int64_t io_callback(void *userdata, void *data, zip_uint64_t len, zip_source_cmd_t cmd) {
	Ref<IO> *ioRef = (Ref<IO> *)userdata;
	switch (cmd) {
	case ZIP_SOURCE_OPEN:
		return 0;
	case ZIP_SOURCE_CLOSE:
		return 0;
	case ZIP_SOURCE_READ:
		return (*ioRef)->read(len, data);
	case ZIP_SOURCE_WRITE:
		return 0;
	case ZIP_SOURCE_TELL:
		return 0;
	case ZIP_SOURCE_SEEK: {
		struct zip_source_args_seek *seek = (struct zip_source_args_seek *)data;
		if (len < sizeof(*seek)) {
			// zip_error_set(&ctx->libzip_error_, ZIP_ER_INVAL, 0);
			return -1;
		}
		// ZIP_SOURCE_GET_ARGS
		// ZIP_SOURCE_GET_ARGS
		// zip_source_seek_compute_offset
		(*ioRef)->seek(seek->offset, (IO::Seek)seek->whence);
		return 0;
	}
	case ZIP_SOURCE_STAT: {
		zip_stat_t *stat;
		if (len < sizeof(*stat)) {
			//::zip_error_set(&ctx->libzip_error_, ZIP_ER_INVAL, 0);
			return -1;
		}

		stat = (zip_stat_t *)data;
		zip_stat_init(stat);
		stat->mtime = time(nullptr);
		stat->size = (*ioRef)->length();
		stat->comp_size = stat->size;
		stat->comp_method = ZIP_CM_STORE;
		stat->encryption_method = ZIP_EM_NONE;
		stat->valid =
			ZIP_STAT_MTIME | ZIP_STAT_SIZE | ZIP_STAT_COMP_SIZE | ZIP_STAT_COMP_METHOD | ZIP_STAT_ENCRYPTION_METHOD;
		return sizeof(*stat);
	}
	case ZIP_SOURCE_ERROR:
		return 0;
	case ZIP_SOURCE_FREE:
		(*ioRef)->deincreemnt();
		return 0;
	case ZIP_SOURCE_SUPPORTS:
		return zip_source_make_command_bitmap(ZIP_SOURCE_OPEN, ZIP_SOURCE_READ, ZIP_SOURCE_CLOSE, ZIP_SOURCE_SEEK,
											  ZIP_SOURCE_TELL, ZIP_SOURCE_STAT, ZIP_SOURCE_FREE, -1);
	default:
		assert(0); // TODO determine if this is state it shall never reach or not.
		return 0;
	}
}

ZipFileSystem *ZipFileSystem::createZipFileObject(Ref<IO> &ioRef, Ref<IScheduler> ref) {
	ZipFileSystem *zipfile;
	struct zip *zip;
	zip_error_t error;

	zip_error_init(&error);

	/*	*/
	ioRef->seek(0, IO::SET);
	zip_source *zipSource = zip_source_function_create(io_callback, &ioRef, nullptr);
	ioRef->increment();

	zip = zip_open_from_source(zipSource, ZIP_RDONLY, &error);
	if (zip == nullptr) {
		char errorStr[4096];

		/*  Release.    */
		zip_source_free(zipSource);
		zip_source_close(zipSource);

		/*	Display error and return.	*/
		int err = zip_error_code_zip(&error);
		zip_error_to_str(errorStr, sizeof(errorStr), err, errno);
		zip_error_fini(&error);
		if (err == ZIP_ER_NOENT) {
			throw InvalidArgumentException("Failed to open zip archive - {}", errorStr);
		}
		throw RuntimeException("Failed to open zip file: {}", zip_error_strerror(&error));
	}
	zip_source_keep(zipSource);

	/*	Create object and store zip pointer.	*/
	zipfile = new ZipFileSystem(ref);
	zipfile->pzip = zip;

	/*	*/
	zip_error_fini(&error);
	return zipfile;
}

ZipFileSystem *ZipFileSystem::createZipFileObject(void *source, int size, Ref<IScheduler> ref) {

	Ref<IO> io = Ref<IO>(new BufferIO(source, size));
	return ZipFileSystem::createZipFileObject(io, ref);
}

void *ZipFileSystem::getZipObject() const { return this->pzip; }

ZipFileSystem::ZipFileSystem(Ref<IScheduler> ref) { this->setScheduleReference(ref); }

ZipFileSystem::ZipFileSystem(const ZipFileSystem &other) : IFileSystem(other) { this->pzip = other.pzip; }

ZipFileSystem::~ZipFileSystem() {

	struct zip *zip = static_cast<struct zip *>(this->pzip);
	/*	*/
	zip_close(zip);
}
