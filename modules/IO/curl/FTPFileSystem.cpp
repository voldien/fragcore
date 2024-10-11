#ifndef FRAG_CORE_INTERNAL_IMP
#define FRAG_CORE_INTERNAL_IMP
#endif
#include "FTPFileSystem.h"
#include "FTPIO.h"
#include <Core/SystemInfo.h>
#include <IO/GZFileIO.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <curl/curl.h>
#include <fmt/core.h>
#include <malloc.h>

using namespace fragcore;

IO *FTPFileSystem::openFile(const char *path, IO::IOMode mode) {

	CURL *fileHandle = curl_easy_duphandle(this->handle);
	if (!fileHandle) {
		curl_global_cleanup();
	}
	return new FTPFileIO(fileHandle, fmt::format("{}{}", this->URL, path).c_str(), mode);
}

void FTPFileSystem::closeFile(IO *io) { throw NotImplementedException(); }

void FTPFileSystem::remove(const char *path) { throw NotImplementedException(); }

void FTPFileSystem::rename(const char *oldPath, const char *newPath) { throw NotImplementedException(); }

bool FTPFileSystem::exists(const char *path) const { throw NotImplementedException(); }

bool FTPFileSystem::isReadable(const char *path) const { throw NotImplementedException(); }

bool FTPFileSystem::isWriteable(const char *path) const { throw NotImplementedException(); }

// const char *FTPFileSystem::getBaseName(const char *path) {
//	return basename(path); // TODO relocate to the OS IO table lookup.
//}
// std::string FTPFileSystem::getAbsolutePath(const char *path) { throw NotImplementedException(); }
//
// std::string FTPFileSystem::getRelativePath(const char *path) { throw NotImplementedException(); }
//
// const char *FTPFileSystem::getFileExtension(const char *path) {
//	const char *dot = strrchr(path, '.');
//	if (!dot || dot == path)
//		return "";
//	return dot + 1;
//}

void FTPFileSystem::createFile(const char *path) { throw NotImplementedException(); }

void FTPFileSystem::createDirectory(const char *path) { throw NotImplementedException(); }

bool FTPFileSystem::isASyncSupported() const { return *this->getScheduler() != nullptr; }

bool FTPFileSystem::isDirectory(const char *path) const { throw NotImplementedException(); }
bool FTPFileSystem::isFile(const char *path) const { throw NotImplementedException(); }

void write_file_dir_callback(void *data, size_t size, size_t nmemb, void *ptr) {

	std::vector<std::string> *list = static_cast<std::vector<std::string> *>(ptr);

	if (data && size * nmemb > 0) {
		std::string line = std::string((char *)data, size * nmemb);
		list->push_back(line);
	}
}

std::vector<std::string> FTPFileSystem::listFiles(const char *directory) const {

	CURL *fileHandle = curl_easy_duphandle(this->handle);
	if (!fileHandle) {
		curl_global_cleanup();
	}
	curl_easy_setopt(fileHandle, CURLOPT_DIRLISTONLY, 1L);

	curl_easy_perform(fileHandle);

	return std::vector<std::string>();
}

std::vector<std::string> FTPFileSystem::listDirectories(const char *directory) const {
	CURL *curl = curl_easy_duphandle(this->handle);

	if (curl) {

		// curl_easy_getinfo(this->curl, CURLINFO_FTP_ENTRY_PATH)

		curl_easy_setopt(curl, CURLOPT_URL, fmt::format("ftp://{}/", this->URL).c_str());

		/* list only */
		curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, 1L);

		CURLcode rc = curl_easy_perform(curl);
		if (rc != CURLE_OK) {

			throw RuntimeException("{}", curl_easy_strerror(rc));
		}

		curl_easy_cleanup(curl);
	}
	return {};
}

std::vector<std::string> FTPFileSystem::list(const char *directory) const {

	std::vector<std::string> list;
	list.reserve(64);

	CURL *fileHandle = curl_easy_duphandle(this->handle);
	if (!fileHandle) {
		curl_global_cleanup();
	}
	char *url = nullptr;
	CURLcode rc = curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);
	if (rc != CURLE_OK) {

		throw RuntimeException("Failed fetch URL: {}", curl_easy_strerror(rc));
	}
	rc = curl_easy_setopt(fileHandle, CURLOPT_URL, fmt::format("{}/{}", url, directory).c_str());

	rc = curl_easy_setopt(fileHandle, CURLOPT_WRITEFUNCTION, write_file_dir_callback);

	rc = curl_easy_setopt(fileHandle, CURLOPT_WRITEDATA, &list);

	rc = curl_easy_setopt(fileHandle, CURLOPT_DIRLISTONLY, 1L);

	rc = curl_easy_perform(fileHandle);
	if (rc != CURLE_OK) {
		throw RuntimeException("Failed to Fetch List: {}", curl_easy_strerror(rc));
	}

	/*	*/
	curl_easy_cleanup(fileHandle);

	return list;
}

FTPFileSystem *FTPFileSystem::createFileSystem(const char *ip, int port, const Ref<IScheduler> &sch) {

	FTPFileSystem *filesystem = new FTPFileSystem(ip, port, nullptr, nullptr, sch);
	return filesystem;
}

void FTPFileSystem::setCredentials(const std::string &username, const std::string &password) {
	if (handle == nullptr)
		throw RuntimeException("FTP FileSystem Not Setup Properly");

	std::string cred = fmt::format("{}:{}", username, password);
	curl_easy_setopt(handle, CURLOPT_USERPWD, cred.c_str());
	/*	Remove the cred from memory once created.	*/
	memset(cred.data(), 0, cred.length());
}

FTPFileSystem::FTPFileSystem(const char *ip, int port, const char *username, const char *password,
							 const Ref<IScheduler> &ref)
	: FTPFileSystem(ip, port, username, password) {

	this->setScheduleReference(ref);
}

FTPFileSystem::FTPFileSystem(const char *hostname, int port, const char *username, const char *password) {
	CURLcode rc = curl_global_init(CURL_GLOBAL_ALL);
	if (rc) {
		throw RuntimeException("Failed: {}", curl_easy_strerror(rc));
	}

	handle = curl_easy_init();
	if (!handle) {
		curl_global_cleanup();
		throw RuntimeException("Failed: ");
		// return CURLE_OUT_OF_MEMORY;
	}

	rc = curl_easy_setopt(handle, CURLOPT_URL, fmt::format("ftp://{}/", hostname).c_str());

	char *url;
	rc = curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);
	this->URL = url;

	if (password && username)
		setCredentials(username, password);
}

FTPFileSystem::~FTPFileSystem() { /*  Release all resources.  */
	curl_easy_cleanup(handle);
}
