#ifndef FRAG_CORE_INTERNAL_IMP
#define FRAG_CORE_INTERNAL_IMP 1
#endif

#include "FTPIO.h"
#include "IO/GZFileIO.h"

#include <curl/curl.h>
#include <curl/multi.h>
#include <fmt/core.h>
using namespace fragcore;

// TODO move to the network impl later.
/* Auxiliary function that waits on the socket. */
static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms) {
	struct timeval tv;
	fd_set infd, outfd, errfd;
	int res;

	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec = (timeout_ms % 1000) * 1000;

	FD_ZERO(&infd);
	FD_ZERO(&outfd);
	FD_ZERO(&errfd);

	FD_SET(sockfd, &errfd); /* always check for error */

	if (for_recv) {
		FD_SET(sockfd, &infd);
	} else {
		FD_SET(sockfd, &outfd);
	}

	/* select() returns the number of signalled sockets or -1 */
	res = select((int)sockfd + 1, &infd, &outfd, &errfd, &tv);
	return res;
}

void FTPFileIO::open(const char *path, IOMode mode) {}

void FTPFileIO::close() { curl_easy_cleanup(this->handle); }

long FTPFileIO::read(long int nbytes, void *pbuffer) {
	size_t nRecv;
	CURLcode rc;
	do {
		rc = curl_easy_recv(this->handle, pbuffer, nbytes, &nRecv);

		if (rc == CURLE_AGAIN && !wait_on_socket(this->sockfd, 1, 1000L)) {
			return -1;
		}
	} while (rc == CURLE_AGAIN);

	if (rc == CURLE_OK) {
		return nRecv;
	}
	return -1;
}

long FTPFileIO::write(long int nbytes, const void *pbuffer) {
	size_t nRecv;
	CURLcode rc;
	do {
		rc = curl_easy_send(this->handle, pbuffer, nbytes, &nRecv);

		if (rc == CURLE_AGAIN && !wait_on_socket(this->sockfd, 0, 1000L)) {
			return -1;
		}
	} while (rc == CURLE_AGAIN);

	if (rc == CURLE_OK) {
		return nRecv;
	}
	return -1;
}

long FTPFileIO::length() {
	double filesize = 0.0;
	CURLcode rc = curl_easy_getinfo(this->handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filesize);
	if (rc != CURLE_OK) {

		throw RuntimeException("{}", curl_easy_strerror(rc));
	}
	return filesize;
}

bool FTPFileIO::eof() const { return false; }

void FTPFileIO::seek(long int nbytes, const Seek seek) {
	// curl_easy_setopt(SEEK_CO)
	// gzrewind
	//	FileIO::seek(nbytes, seek);
}

unsigned long FTPFileIO::getPos() {
	// gzeof
	// gztell
	// return FileIO::getPos();
	return 0;
}

bool FTPFileIO::isWriteable() const { return (this->ioMode & IOMode::READ) != 0; }

bool FTPFileIO::isReadable() const { return (this->ioMode & IOMode::WRITE) != 0; }

bool FTPFileIO::flush() {

	// if (this->mode & WRITE) {
	// 	int error;
	// 	error = gzflush(this->gzFi, Z_FINISH);
	// 	if (error != Z_OK)
	// 		throw RuntimeException(zError(error));
	// 	return true;
	// }
	// return false;
	return true;
}

static size_t ftpio_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
	FTPFileIO *out = (FTPFileIO *)stream;
	//   if(!out->stream) {
	//     /* open file for writing */
	//     out->stream = fopen(out->filename, "wb");
	//     if(!out->stream)
	//       return -1; /* failure, can't open file to write */
	//   }
	//   return fwrite(buffer, size, nmemb, out->stream);
	return 0;
}

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data) {
	/* we are not interested in the headers itself,
	   so we only return the size we would have saved ... */
	return (size_t)(size * nmemb);
}

static int seek_cb(void *userp, curl_off_t offset, int origin) {
	FTPFileIO *out = (FTPFileIO *)userp;
	// lseek(our_fd, offset, origin);
	return CURL_SEEKFUNC_OK;
}

FTPFileIO::FTPFileIO(const char *path, IOMode mode) {
	//   curl_easy_setopt(CURL *handle, CURLOPT_SEEKFUNCTION, seek_cb);
	//   curl_easy_setopt(CURL *handle, CURLOPT_SEEKDATA, &seek_data);

	CURL *curl;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL,
						 path); // "ftp://ftp.example.com/curl/curl-7.9.2.tar.gz");
		/* Define our callback to get called when there's data to be written */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftpio_fwrite);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, throw_away);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

		/* Switch on full protocol/debug output */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
	}
	CURLcode rc = curl_easy_perform(curl);
}

int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
	if (type == CURLINFO_TEXT) {
	}
	return 0;
}

FTPFileIO::FTPFileIO(CURL *handle, const char *URL, IOMode mode) {
	if (handle == nullptr) {
		throw InvalidArgumentException("");
	}
	this->handle = handle;
	CURLcode rc;
	if (this->handle) {
		// "ftp://ftp.example.com/curl/curl-7.9.2.tar.gz");
		rc = curl_easy_setopt(this->handle, CURLOPT_URL, URL);
		this->ioMode = mode;

		if (mode & IOMode::READ) {
			rc = curl_easy_setopt(this->handle, CURLOPT_READFUNCTION, ftpio_fwrite);
			/* Set a pointer to our struct to pass to the callback */
			rc = curl_easy_setopt(this->handle, CURLOPT_READDATA, this);
		}
		if (mode & IOMode::WRITE) {
			rc = curl_easy_setopt(this->handle, CURLOPT_WRITEFUNCTION, ftpio_fwrite);
			/* Set a pointer to our struct to pass to the callback */
			rc = curl_easy_setopt(this->handle, CURLOPT_WRITEDATA, this);
		}

		rc = curl_easy_setopt(this->handle, CURLOPT_HEADERFUNCTION, throw_away);

		rc = curl_easy_setopt(this->handle, CURLOPT_HEADER, 0L);

		rc = curl_easy_setopt(this->handle, CURLOPT_SEEKFUNCTION, seek_cb);

		rc = curl_easy_setopt(this->handle, CURLOPT_SEEKDATA, (void *)this);

		/* Switch on full protocol/debug output */
		rc = curl_easy_setopt(this->handle, CURLOPT_VERBOSE, 1L);
		rc = curl_easy_setopt(this->handle, CURLOPT_DEBUGFUNCTION, debug_callback);

		rc = curl_easy_setopt(this->handle, CURLOPT_CONNECTTIMEOUT, 5L);

		// rc = curl_easy_setopt(this->handle, CURLOPT_CONNECT_ONLY, 1L);
	}
	rc = curl_easy_perform(this->handle);
	if (rc != CURLE_OK) {
		throw RuntimeException("{}", curl_easy_strerror(rc));
	}

	rc = curl_easy_getinfo(this->handle, CURLINFO_ACTIVESOCKET, &this->sockfd);
}
