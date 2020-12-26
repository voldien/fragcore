#include"Core/IO/BZFileIO.h"
#include "Exception/RuntimeException.h"
#include"Utils/StringUtil.h"
#include <bzlib.h>


using namespace fragcore;

long BZFileIO::read(long int nbytes, void *pbuffer) {
	int bzerror;
	int len = BZ2_bzRead(&bzerror, this->bzFile, pbuffer, nbytes);
	if(bzerror != BZ_OK){
		throw RuntimeException(fvformatf("Failed to read %s", BZ2_bzerror(this->bzFile, &bzerror)));
	}
	return len;
}

long BZFileIO::write(long int nbytes, const void *pbuffer) {
	int bzerror;
	int len = nbytes;
	BZ2_bzWrite(&bzerror, this->bzFile, (void*)pbuffer, (int)nbytes);
	if(bzerror != BZ_OK){
		throw RuntimeException(fvformatf("Failed to write %s", BZ2_bzerror(this->bzFile, &bzerror)));
	}
	return len;
}

long BZFileIO::length(void) {
	return FileIO::length();
}

bool BZFileIO::eof(void) const {
	return FileIO::eof();
}

void BZFileIO::seek(long int nbytes, IO::Seek seek) {
	FileIO::seek(nbytes, seek);
}

unsigned long BZFileIO::getPos(void) {
	return FileIO::getPos();
}

bool BZFileIO::isWriteable(void) const {

	return FileIO::isWriteable();
}

bool BZFileIO::isReadable(void) const {
	return FileIO::isReadable();
}

bool BZFileIO::flush(void) {
	return BZ2_bzflush(this->bzFile);
//	return FileIO::flush();
}

void BZFileIO::close(void) {
	int bzerror;
	BZ2_bzReadClose(&bzerror, this->bzFile);

	unsigned int nbyte_in, nbytes_out;
	BZ2_bzWriteClose(&bzerror, this->bzFile, 0, &nbyte_in, &nbytes_out);

	FileIO::close();
}

void BZFileIO::open(const char *path, IO::Mode mode) {

	/*  */
	int bzerror;
	switch(mode & IO::ACCESS){
		case IO::READ:
			this->bzFile = BZ2_bzReadOpen(&bzerror, this->file, 0, NULL, NULL, 0);
			break;
		case IO::WRITE:
			this->bzFile = BZ2_bzWriteOpen(&bzerror, this->file, 0, 0,0);
			break;
		case IO::ACCESS:
			break;
		default:
			break;
	}

	/*  */
	if (this->bzFile == NULL)
		throw RuntimeException(fvformatf("Failed to open %s", BZ2_bzerror(this->bzFile, &bzerror)));
}

BZFileIO::BZFileIO(const char *path, IO::Mode mode) : FileIO(path, mode) {
	this->open(path, mode);
}