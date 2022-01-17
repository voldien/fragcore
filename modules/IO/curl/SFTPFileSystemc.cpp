#include"SFTPFileSystem.h"

using namespace fragcore;

SFTPFileSystem *SFTPFileSystem::createFileSystem(const char *ip, int port, const Ref<IScheduler> &sch) {

	SFTPFileSystem *filesystem = new SFTPFileSystem(ip, port, nullptr, nullptr, sch);
	return filesystem;
}

void SFTPFileSystem::setCredentials(const std::string &username, const std::string &password) {
	if (handle == nullptr)
		throw RuntimeException("SFTP FileSystem Not Setup Properly");

	std::string cred = fmt::format("{}:{}", username, password);
	curl_easy_setopt(handle, CURLOPT_USERPWD, cred.c_str());
	/*	Remove the cred from memory once created.	*/
	memset(cred.data(), 0, cred.length());
}

SFTPFileSystem::SFTPFileSystem(const char *ip, int port, const char *username, const char *password,
							   const Ref<IScheduler> &ref)
	: SFTPFileSystem(ip, port, username, password) {

	this->setScheduleReference(ref);
}

SFTPFileSystem::SFTPFileSystem(const char *ip, int port, const char *username, const char *password) {
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

	curl_easy_setopt(handle, CURLOPT_URL, fmt::format("sftp://{}@{}/", username, ip).c_str());

#ifndef DISABLE_SSH_AGENT
	/* We activate ssh agent. For this to work you need
	   to have ssh-agent running (type set | grep SSH_AGENT to check) or
	   pageant on Windows (there is an icon in systray if so) */
	curl_easy_setopt(handle, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_AGENT);
#endif

	if (password && username)
		setCredentials(username, password);
}

SFTPFileSystem::~SFTPFileSystem() { /*  Release all resources.  */
	curl_easy_cleanup(handle);
}