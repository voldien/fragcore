#include "Core/IO/FileSystem.h"

#include <Core/IO/FileSystem.h>
#include <Core/IO/GZFileIO.h>
#include <Core/SystemInfo.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <malloc.h>
namespace fs = std::filesystem;

using namespace fragcore;

IO *FileSystem::openFile(const char *path, IO::IOMode mode) {
	const char *extension = FileSystem::getFileExtension(path);

	// Parse if possible IO type exists.
	if (strcmp(extension, "gz") == 0)
		return new GZFileIO(path, mode);
	// Open default as IO file.
	return new FileIO(path, mode);
}

void FileSystem::closeFile(IO *io) {
	io->close();
	// TODO determine what shall be done more.
	// TODO determine how it can be released in a proper manner.
}
const char *FileSystem::getBaseName(const char *path) {
	return basename(path); // TODO relocate to the OS IO table lookup.
}

void FileSystem::remove(const char *path) {
#if __cplusplus >= 201703L
	fs::remove(path);
#else
	throw NotImplementedException();
// if (::remove(path) != 0)
// throw RuntimeException"{}", strerror(errno));
#endif
}

void FileSystem::rename(const char *oldPath, const char *newPath) {
#if __cplusplus >= 201703L
	return fs::rename(oldPath, newPath);
#else
	throw NotImplementedException();
// if (::rename(oldPath, newPath) != 0)
//     throw RuntimeException("{}", strerror(errno));
#endif
}

bool FileSystem::exists(const char *path) const {
#if __cplusplus >= 201703L
	return fs::exists(path);
#else
	FILE *f = fopen(path, "r+");
	if (f) {
		fclose(f);
		return true;
	}
	return false;
#endif
	return false;
}

bool FileSystem::isReadable(const char *path) const {
#if __cplusplus >= 201703L
	if (exists(path)) {
		fs::perms permissionStatus = fs::status(path).permissions();
		return (static_cast<unsigned int>(permissionStatus) & static_cast<unsigned int>(fs::perms::owner_read)) != 0;
	}
	throw RuntimeException();
#else
	throw NotImplementedException();
#endif
	return false;
}

bool FileSystem::isWriteable(const char *path) const {
#if __cplusplus >= 201703L
	if (exists(path)) {
		fs::perms permissionStatus = fs::status(path).permissions();
		return (static_cast<unsigned int>(permissionStatus) & static_cast<unsigned int>(fs::perms::owner_write)) != 0;
	}
	throw RuntimeException();
#else
	throw NotImplementedException();
#endif
	return false;
}

std::string FileSystem::getAbsolutePath(const char *path) {
#if __cplusplus >= 201703L
	return fs::absolute(path);
#else
	throw NotImplementedException();
#endif
}

std::string FileSystem::getRelativePath(const char *path) {
#if __cplusplus >= 201703L
	return fs::relative(path);
#else
	throw NotImplementedException();
#endif
}

const char *FileSystem::getFileExtension(const char *path) {
	const char *dot = strrchr(path, '.');
	if (!dot || dot == path)
		return "";
	return dot + 1;
}

void FileSystem::createFile(const char *path) {
	FILE *f = fopen(path, "ab+");
	if (f == nullptr) {
		throw RuntimeException("Failed to open file {}, {}", path, strerror(errno));
	}
	fclose(f);
}

void FileSystem::createDirectory(const char *path) {
#if __cplusplus >= 201703L
	fs::create_directory(path);
#else
	throw NotImplementedException();
#endif
}

bool FileSystem::isASyncSupported() const { return *this->getScheduler() != nullptr; }

bool FileSystem::isDirectory(const char *path) const {
#if __cplusplus >= 201703L
	return fs::is_directory(path);
#else
	throw NotImplementedException();
#endif
}
bool FileSystem::isFile(const char *path) const {
#if __cplusplus >= 201703L
	return fs::is_regular_file(path);
#else
	throw NotImplementedException();
#endif
}

std::vector<std::string> FileSystem::listFiles(const char *directory) const {
#if __cplusplus >= 201703L
	if (isDirectory(directory)) {
		std::vector<std::string> files;
		for (const auto &entry : fs::directory_iterator(directory)) {
			if (entry.is_regular_file()) {
				files.push_back(entry.path());
			}
		}
		return files;
	}
	throw RuntimeException();
#else
	throw NotImplementedException();
#endif
}

std::vector<std::string> FileSystem::listDirectories(const char *directory) const {
#if __cplusplus >= 201703L
	if (isDirectory(directory)) {
		std::vector<std::string> files;
		for (const auto &entry : fs::directory_iterator(directory)) {
			if (entry.is_directory()) {
				files.push_back(entry.path());
			}
		}
		return files;
	}
	throw RuntimeException();
#else
	throw NotImplementedException();
#endif
}

std::vector<std::string> FileSystem::list(const char *directory) const {
#if __cplusplus >= 201703L
	if (isDirectory(directory)) {
		std::vector<std::string> files;
		for (const auto &entry : fs::directory_iterator(directory)) {
			files.push_back(entry.path());
		}
		return files;
	}
	throw RuntimeException();
#else
	throw NotImplementedException();
#endif
}

// TODO realocate and improve
IO *FileSystem::createFIFO(const std::string &path) {
	// unlink(path.c_str());
	int ret = mkfifo(path.c_str(), S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH | O_RDWR);

	// FileIO *io = new FileIO(path.c_str(), (IO::IOMode)( IO::IOMode::WRITE)); //IO::IOMode::APPEND |

	return nullptr;
}

static FileSystem *fileSystem = nullptr;
FileSystem *FileSystem::getFileSystem() {
	if (fileSystem == nullptr) {
		throw RuntimeException("FileSystem must created before utilizing it.");
	}
	return fileSystem;
}

FileSystem *FileSystem::createFileSystem(Ref<IScheduler> &ref) {
	fileSystem = new FileSystem(ref);
	return fileSystem;
}

FileSystem *FileSystem::createFileSystem() {
	fileSystem = new FileSystem();
	return fileSystem;
}

FileSystem::FileSystem(Ref<IScheduler> &ref) : IFileSystem() {

	/*  Lookup system io functions.  */
	// TODO add support for adding lookup table for how to access VFS functions.
	switch (SystemInfo::getOperatingSystem()) {
	default:
		break;
	}

	this->setScheduleReference(ref);
}

FileSystem::FileSystem() {}

FileSystem::~FileSystem() { /*  Release all resources.  */
}
