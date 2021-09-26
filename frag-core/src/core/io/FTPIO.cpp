#define FRAG_CORE_INTERNAL_IMP

#include "Core/IO/FTPIO.h"
#include "Core/IO/GZFileIO.h"
//#include "Exception/InvalidArgumentException.h"
//#include "Exception/RuntimeException.h"

#include <curl/curl.h>
#include <curl/multi.h>
#include <fmt/core.h>
using namespace fragcore;

void FTPFileIO::open(const char *path, IOMode mode) {}

void FTPFileIO::close(void) { curl_easy_cleanup(this->handle); }

long FTPFileIO::read(long int nbytes, void *pbuffer) {
	size_t nRecv;
	if (curl_easy_recv(this->handle, pbuffer, nbytes, &nRecv) == CURLE_OK)
		return nRecv;
	else {
		return 0;
	}
}

long FTPFileIO::write(long int nbytes, const void *pbuffer) {
	size_t nRecv;
	if (curl_easy_send(this->handle, pbuffer, nbytes, &nRecv) == CURLE_OK)
		return nRecv;
	else {
		return 0;
	}
}

long FTPFileIO::length(void) {

	double filesize = 0.0;
	CURLcode res = curl_easy_getinfo(this->handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filesize);
	return filesize;
}

bool FTPFileIO::eof(void) const { return 0; }

void FTPFileIO::seek(long int nbytes, Seek seek) {
	// curl_easy_setopt(SEEK_CO)
	// gzrewind
	//	FileIO::seek(nbytes, seek);
}

unsigned long FTPFileIO::getPos(void) {
	// gzeof
	// gztell
	// return FileIO::getPos();
}

bool FTPFileIO::isWriteable(void) const { return false; }

bool FTPFileIO::isReadable(void) const { return false; }

bool FTPFileIO::flush(void) {

	// if (this->mode & WRITE) {
	// 	int error;
	// 	error = gzflush(this->gzFi, Z_FINISH);
	// 	if (error != Z_OK)
	// 		throw RuntimeException(zError(error));
	// 	return true;
	// }
	// return false;
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
}

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data) {
	(void)ptr;
	(void)data;
	/* we are not interested in the headers itself,
	   so we only return the size we would have saved ... */
	return (size_t)(size * nmemb);
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
	}
}

FTPFileIO::FTPFileIO(CURL *handle, const char *path, IOMode mode) {
	if (handle == nullptr)
		throw InvalidArgumentException("");

	if (handle) {
		curl_easy_setopt(handle, CURLOPT_URL,
						 path); // "ftp://ftp.example.com/curl/curl-7.9.2.tar.gz");
		/* Define our callback to get called when there's data to be written */
		switch (mode) {
		case IOMode::READ:
			/* code */
			break;
		case IOMode::WRITE:
			break;
		case IOMode::ACCESS:
			break;
		default:
			break;
		}
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, ftpio_fwrite);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);

		curl_easy_setopt(handle, CURLOPT_READFUNCTION, ftpio_fwrite);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(handle, CURLOPT_READDATA, this);

		curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, throw_away);

		curl_easy_setopt(handle, CURLOPT_HEADER, 0L);

		/* Switch on full protocol/debug output */
		curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
	}
}

// FTPFileIO::FTPFileIO(Ref<IO> &io){

// }
