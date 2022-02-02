#include "Core/IO/FileSystem.h"

#include <Core/IO/FileSystem.h>
#include <Core/IO/GZFileIO.h>
#include <Core/SystemInfo.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fmt/core.h>
#include <fstream>
#include <malloc.h>
// namespace fs = std::filesystem;
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
//	fs::remove(path);
#else
	throw NotImplementedException();
// if (::remove(path) != 0)
// throw RuntimeException"{}", strerror(errno));
#endif
}

void FileSystem::rename(const char *oldPath, const char *newPath) {
#if __cplusplus >= 201703L
	// return fs::rename(oldPath, newPath);
#else
	throw NotImplementedException();
// if (::rename(oldPath, newPath) != 0)
//     throw RuntimeException("{}", strerror(errno));
#endif
}

bool FileSystem::exists(const char *path) const {
#if __cplusplus >= 201703L
	// return fs::exists(path);
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
	FILE *f = fopen(path, "r+");
	if (f) {
		fclose(f);
		return true;
	}
	return false;
}

bool FileSystem::isWriteable(const char *path) const {
	FILE *f = fopen(path, "w+");
	if (f) {
		fclose(f);
		return true;
	}
	return false;
}

std::string FileSystem::getAbsolutePath(const char *path) {
	// return fs::absolute(path);
	// char resolved_path[PATH_MAX];
	// realpath(path, resolved_path);
	// return resolved_path;
	return "";
}

std::string FileSystem::getRelativePath(const char *path) { return ""; }

const char *FileSystem::getFileExtension(const char *path) {
	const char *dot = strrchr(path, '.');
	if (!dot || dot == path)
		return "";
	return dot + 1;
}

void FileSystem::createFile(const char *path) {
	FILE *f = fopen(path, "ab+");
	if (f == nullptr)
		throw RuntimeException("Failed to open file {}, {}", path, strerror(errno));
	fclose(f);
}

void FileSystem::createDirectory(const char *path) {
	// fs::create_directory(path);
	// mkdir(path);
}

bool FileSystem::isASyncSupported() const { return *this->getScheduler() != nullptr; }

bool FileSystem::isDirectory(const char *path) { return false; }
bool FileSystem::isFile(const char *path) {
	// TODO improve!
	std::ifstream ifs(path);
	return ifs.good();
}

std::vector<std::string> FileSystem::listFiles(const char *directory) const { return std::vector<std::string>(); }

std::vector<std::string> FileSystem::listDirectories(const char *directory) const {
	// for(auto& p: fs::directory_iterator("sandbox"))
	// 	std::cout << p.path() << '\n';
	// return std::vector<std::string>();
	return {};
}

std::vector<std::string> FileSystem::list(const char *directory) const {
	// for(auto& p: fs::directory_iterator("sandbox"))
	// 	std::cout << p.path() << '\n';
	// return std::vector<std::string>();
	return {};
}

static FileSystem *fileSystem = nullptr;
FileSystem *FileSystem::getFileSystem() {
	if (fileSystem == nullptr)
		throw RuntimeException("FileSystem must created before utilizing it.");
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
