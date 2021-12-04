#include "Core/IO/ZipFileSystem.h"
#include "Core/IO/ZipFileIO.h"

#include <cassert>
#include <fmt/core.h>
#include <vector>
#include <zip.h>
#include <zlib.h>
using namespace fragcore;

IO *ZipFileSystem::openFile(const char *path, IO::IOMode mode) {
	struct zip_file *zfile;
	struct zip_stat stat;
	struct zip *zip = (struct zip *)this->pzip;
	int err, sys_err;
	zip_uint32_t attr;

	// TODO move to the index open method.

	zip_int64_t index = zip_name_locate(zip, path, 0);

	zip_uint8_t osSys = 0;
	err = zip_file_get_external_attributes((zip_t *)zip, index, 0, &osSys, &attr);
	if (err != 0) {
		char buf[1024];
		zip_error_get(zip, &err, &sys_err);
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw InvalidArgumentException("can't open `{}':\n\t{}", path, buf);
	}

	/*	*/
	err = zip_stat_index(zip, index, 0, &stat);
	if (err != 0) {
		char buf[1024];
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw InvalidArgumentException("can't open `{}':\n\t{}", path, buf);
	}

	// TODO check if encryption and compression.

	/*	Open file inside zip.	*/
	zfile = zip_fopen_index(zip, index, 0);
	if (!zfile) {
		char buf[1024];
		zip_error_get(zip, &err, &sys_err);
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw InvalidArgumentException("can't open `{}':\n\t{}", path, buf);
	}

	return new ZipFileIO(zfile, index, Ref<ZipFileSystem>(this));
}

IO *ZipFileSystem::openFile(unsigned int index) {
	struct zip_file *zfile;
	struct zip_stat stat;
	char buf[1024];
	int err;

	/*	*/
	err = zip_stat_index((struct zip *)this->pzip, index, 0, &stat);
	if (err != 0) {
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw InvalidArgumentException("can't open `{}':\n\t{}", index, buf);
	}

	zfile = zip_fopen_index((struct zip *)this->pzip, index, 0);
	if (!zfile) {
		err = zip_get_error((struct zip *)this->pzip)->zip_err;
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw InvalidArgumentException("can't open index `{}':\n\t{}", index, buf);
	}

	return new ZipFileIO(zfile, index, Ref<ZipFileSystem>(this));
}

bool ZipFileSystem::isASyncSupported() const { return *this->getScheduler() != nullptr; }

bool ZipFileSystem::isDirectory(const char *path) { return false; }
bool ZipFileSystem::isFile(const char *path) { return false; }

std::vector<std::string> ZipFileSystem::listFiles(const char *path) const {

	int err;
	struct zip *zip;
	ZipFileSystem *zipfile;
	char buf[4096];

	std::vector<std::string> files = std::vector<std::string>();

	/*	Attempt to open the file by file path.	*/
	zip_int64_t index = zip_name_locate((struct zip *)this->pzip, path, 0);
	if (index < 0)
		throw InvalidArgumentException("{}", path);

	zip_int64_t num_entries = zip_get_num_entries((struct zip *)this->pzip, 0);

	for (zip_uint64_t i = 0; i < (zip_uint64_t)num_entries; i++) {
		const char *name = zip_get_name((struct zip *)this->pzip, i, 0);
		if (strcmp(path, name) == 0)
			files.push_back(name);
	}

	return files;
}

std::vector<std::string> ZipFileSystem::listDirectories(const char *path) const {
	int err;
	struct zip *zip;
	ZipFileSystem *zipfile;
	char buf[4096];

	/*	Attempt to open the file by file path.	*/
	zip = zip_open(path, 0, &err);

	return std::vector<std::string>();
}

std::vector<std::string> ZipFileSystem::list(const char *path) const {
	std::vector<std::string> files = listFiles(path);
	std::vector<std::string> directories = listDirectories(path);
	/*  merge.  */
	// files.insert(files.end)
}

void ZipFileSystem::release() {
	if (this->deincreemnt()) {
		if (this->pzip)
			zip_close((zip_t *)this->pzip);
	}
	this->pzip = nullptr;
}

void ZipFileSystem::closeFile(IO *io) {}

void ZipFileSystem::remove(const char *path) {
	int err;
	char buf[4096];

	// Find the file.
	int index = 0;

	err = zip_delete((zip_t *)this->pzip, index);
	if (err != ZIP_ER_OK) {
		zip_error_to_str(buf, sizeof(buf), err, errno);
		throw InvalidArgumentException("Could not delete file {}, error: {}", path, buf);
	}
}

void ZipFileSystem::rename(const char *oldPath, const char *newPath) {}

void ZipFileSystem::createFile(const char *path) { zip_int64_t error = zip_add((zip_t *)this->pzip, path, nullptr); }

void ZipFileSystem::createDirectory(const char *path) { zip_int64_t error = zip_dir_add((zip_t *)this->pzip, path, 0); }

bool ZipFileSystem::isReadable(const char *path) const {
	// err = zip_file_get_external_attributes((zip_t*)zip, index, 0, &osSys, &attr);
	return false;
}

bool ZipFileSystem::isWriteable(const char *path) const {
	// err = zip_file_get_external_attributes((zip_t*)zip, index, 0, &osSys, &attr);
	return false;
}

bool ZipFileSystem::exists(const char *path) const {
	struct zip_file *zfile;
	struct zip_stat stat;
	int err;

	/*	*/
	err = zip_stat((struct zip *)this->pzip, path, 0, &stat);
	if (err != ZIP_ER_OK)
		return false;
	return true;
}

ZipFileSystem *ZipFileSystem::createZipFileObject(const char *cfilename, Ref<IScheduler> ref) {

	int err;
	struct zip *zip;
	ZipFileSystem *zipfile;

	/*	Attempt to open the file by file path.	*/
	zip = zip_open(cfilename, 0, &err);
	if (zip == nullptr) {
		char buf[4096];
		/*	Display error and return.	*/
		zip_error_to_str(buf, sizeof(buf), err, errno);
		if (err == ZIP_ER_NOENT) {
			throw InvalidArgumentException("can't open zip archive {} - {}", cfilename, buf);
		} else {
			throw RuntimeException("{}", buf);
		}
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
		zip_stat_t *st;
		if (len < sizeof(*st)) {
			//::zip_error_set(&ctx->libzip_error_, ZIP_ER_INVAL, 0);
			return -1;
		}

		st = (zip_stat_t *)data;
		zip_stat_init(st);
		st->mtime = time(nullptr);
		st->size = (*ioRef)->length();
		st->comp_size = st->size;
		st->comp_method = ZIP_CM_STORE;
		st->encryption_method = ZIP_EM_NONE;
		st->valid =
			ZIP_STAT_MTIME | ZIP_STAT_SIZE | ZIP_STAT_COMP_SIZE | ZIP_STAT_COMP_METHOD | ZIP_STAT_ENCRYPTION_METHOD;
		return sizeof(*st);
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
		char buf[4096];

		/*  Release.    */
		zip_source_free(zipSource);
		zip_source_close(zipSource);

		/*	Display error and return.	*/
		int err = zip_error_code_zip(&error);
		zip_error_to_str(buf, sizeof(buf), err, errno);
		zip_error_fini(&error);
		if (err == ZIP_ER_NOENT)
			throw InvalidArgumentException("can't open zip archive - {}", buf);
		else
			throw RuntimeException("Can't open zip file: {}", zip_error_strerror(&error));
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
	ZipFileSystem *zipfile;
	struct zip *zip;
	zip_error_t error;

	zip_error_init(&error);

	// zip_source* zipSource = zip_source_buffer_create(source, size, ZIP_RDONLY, error);

	return nullptr;
}

void *ZipFileSystem::getZipObject() const { return this->pzip; }

ZipFileSystem::ZipFileSystem(Ref<IScheduler> ref) { this->setScheduleReference(ref); }

ZipFileSystem::ZipFileSystem(const ZipFileSystem &other) { this->pzip = nullptr; }

ZipFileSystem::~ZipFileSystem() {

	struct zip *zip = static_cast<struct zip *>(this->pzip);
	/*	*/
	int rc = zip_close(zip);
}

// FileAccess ZipFile::getFileAccess(const char *path) {
//	return FileAccess();
//}
