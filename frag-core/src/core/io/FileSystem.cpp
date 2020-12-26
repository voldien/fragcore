#include <cstdio>
#include <cassert>
#include <malloc.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <Utils/StringUtil.h>
#include <Core/IO/FileSystem.h>
#include <Core/IO/GZFileIO.h>
#include "Exception/RuntimeException.h"
#include"Exception/NotImplementedException.h"
#include"Exception/InvalidArgumentException.h"
#include <Core/SystemInfo.h>
#include"Core/IO/FileSystem.h"
#include<filesystem>

namespace fs = std::filesystem;
using namespace fragcore;

IO *FileSystem::openFile(const char *path, IO::Mode mode) {
	const char *extension = FileSystem::getFileExtension(path);

	// Parse if possible IO type exists.
	if (strcmp(extension, "gz") == 0)
		return new GZFileIO(path, mode);
	// Open default as IO file.
	return new FileIO(path, mode);
}

void FileSystem::closeFile(IO *io) {
	io->close();
	//TODO determine what shall be done more.
	//TODO determine how it can be released in a proper manner.
}
const char *FileSystem::getBaseName(const char *path) {
	return basename(path);  //TODO relocate to the OS IO table lookup.
}


void FileSystem::remove(const char *path) {
	#if __cplusplus >= 201703L
	fs::remove(path);	
	#else
	throw NotImplementedException();
	// if (::remove(path) != 0)
	// throw RuntimeException(fvformatf("%s", strerror(errno)));
	#endif

}

void FileSystem::rename(const char *oldPath, const char *newPath) {
	#if __cplusplus >= 201703L
		return fs::rename(oldPath,newPath);
	#else
	throw NotImplementedException();
	// if (::rename(oldPath, newPath) != 0)
	//     throw RuntimeException(fvformatf("%s", strerror(errno)));
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
	return fs::absolute(path);
	char resolved_path[PATH_MAX];
	realpath(path, resolved_path);
	return resolved_path;
}

std::string FileSystem::getRelativePath(const char *path)
{
	
}

const char *FileSystem::getFileExtension(const char *path) {
	const char *dot = strrchr(path, '.');
	if (!dot || dot == path) return "";
	return dot + 1;
}

void FileSystem::createFile(const char *path) {
	FILE *f = fopen(path, "ab+");
	if(f == NULL)
		throw RuntimeException(fvformatf("Failed to open file %s, %s.\n", path, strerror(errno)));
	fclose(f);
}

void FileSystem::createDirectory(const char *path) {
	fs::create_directory(path);
	// mkdir(path);
}

bool FileSystem::isASyncSupported(void) const{
	return *this->getScheduler() != NULL;
}

bool FileSystem::isDirectory(const char *path){

}
bool FileSystem::isFile(const char *path){
	
}

std::vector<std::string> FileSystem::listFiles(const char *directory) const {

	return std::vector<std::string>();
}

std::vector<std::string> FileSystem::listDirectories(const char *directory) const {
	for(auto& p: fs::directory_iterator("sandbox"))
		std::cout << p.path() << '\n';
	return std::vector<std::string>();
}

std::vector<std::string> FileSystem::list(const char *directory) const {
	for(auto& p: fs::directory_iterator("sandbox"))
		std::cout << p.path() << '\n';
	return std::vector<std::string>();
}

static FileSystem *fileSystem = NULL;
FileSystem *FileSystem::getFileSystem(void) {
	if (fileSystem == NULL)
		throw RuntimeException("FileSystem must created before utilizing it.");
	return fileSystem;
}

FileSystem *FileSystem::createFileSystem(Ref<IScheduler> &ref)
{
	fileSystem = new FileSystem(ref);
	return fileSystem;
}

FileSystem *FileSystem::createFileSystem(void){
	fileSystem = new FileSystem();
	return fileSystem;
}

FileSystem::FileSystem(Ref<IScheduler> &ref) : IFileSystem()
{

	/*  Lookup system io functions.  */
	//TODO add support for adding lookup table for how to access VFS functions.
	switch(SystemInfo::getOperatingSystem()){
		default:
			break;
	}

	this->setScheduleReference(ref);
}

FileSystem::FileSystem(void){
	
}

FileSystem::~FileSystem(void) {
	/*  Release all resources.  */
}


