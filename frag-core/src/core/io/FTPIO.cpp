#define FRAG_CORE_INTERNAL_IMP

#include "Core/IO/FTPIO.h"
#include "Core/IO/GZFileIO.h"
#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"
#include "Utils/StringUtil.h"
#include <curl/curl.h>
#include <fmt/core.h>
#include <stdexcept>
using namespace fragcore;

void FTPFileIO::open(const char *path, Mode mode) {
	// TODO change.
	// FileIO::open(path, mode);

	// // TODO add other access modes.
	// const char *m = NULL;
	// switch (mode & ACCESS) {
	// case READ:
	// 	m = "rb";
	// 	break;
	// case WRITE:
	// 	m = "wb";
	// 	break;
	// default:
	// 	throw InvalidArgumentException("Invalid IO mode.");
	// }

	// /*  */
	// this->gzFi = gzdopen(fileno(this->file), m);
	// if (this->gzFi == NULL) {
	// 	int error;
	// 	const char *errMsg = gzerror(this->gzFi, &error);
	// 	throw RuntimeException(fmt::format("Failed to open %s - error: %d | %s", path, error, errMsg));
	// }

	// /*  Set buffer size.    */
	// gzbuffer(this->gzFi, 8192);
	// if (mode & WRITE) {
	// 	// gzsetparams
	// }
}

void FTPFileIO::close(void) {
	// int error;
	// error = gzclose(this->gzFi);
	// if (error != Z_OK) {
	// 	FileIO::close();
	// 	throw RuntimeException(fmt::format("Failed to close gzfile %s", zError(error)));
	// }
	// FileIO::close();
}

long FTPFileIO::read(long int nbytes, void *pbuffer) {
	// long int nreadBytes = gzfread(pbuffer, 1, nbytes, this->gzFi);
	// return nreadBytes;
}

long FTPFileIO::write(long int nbytes, const void *pbuffer) {
	// long int nWrittenBytes = gzfwrite(pbuffer, 1, nbytes, this->gzFi);
	// if (nWrittenBytes == 0)
	// 	throw RuntimeException(fmt::format("Failed to write to  gz file %s", gzerror(this->gzFi, NULL)));
	// return nWrittenBytes;
}

long FTPFileIO::length(void) {
	// long size;
	// unsigned long curPos = getPos();
	// FileIO::seek(0, SET);
	// FileIO::read(4, info);
	// return FileIO::length();
}

bool FTPFileIO::eof(void) const { return 0; }

void FTPFileIO::seek(long int nbytes, Seek seek) {
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

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
	FTPFileIO *out = (FTPFileIO *)stream;
	//   if(!out->stream) {
	//     /* open file for writing */
	//     out->stream = fopen(out->filename, "wb");
	//     if(!out->stream)
	//       return -1; /* failure, can't open file to write */
	//   }
	//   return fwrite(buffer, size, nmemb, out->stream);
}

FTPFileIO::FTPFileIO(const char *path, Mode mode) {
	//   curl_easy_setopt(CURL *handle, CURLOPT_SEEKFUNCTION, seek_cb);
	//   curl_easy_setopt(CURL *handle, CURLOPT_SEEKDATA, &seek_data);
    CURL *curl;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL,
						 path); // "ftp://ftp.example.com/curl/curl-7.9.2.tar.gz");
		/* Define our callback to get called when there's data to be written */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		/* Switch on full protocol/debug output */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	}
}

// FTPFileIO::FTPFileIO(Ref<IO> &io){

// }
