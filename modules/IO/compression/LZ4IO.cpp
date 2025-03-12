#include "LZ4IO.h"
#include "IO/FileIO.h"
#include <bzlib.h>
#include <fmt/core.h>

using namespace fragcore;

long LZ4IO::read(long int nbytes, void *pbuffer) {

	uint8_t buffer[4096];

	const size_t len = FileIO::read(nbytes, pbuffer);

	const size_t uncompressed_len = this->compressor.inflate(pbuffer, len, buffer, sizeof(buffer));
	return len;
}

long LZ4IO::write(long int nbytes, const void *pbuffer) {
	uint8_t output_buffer[4096];

	int len = this->compressor.deflate(pbuffer, nbytes, output_buffer, sizeof(output_buffer));
	return FileIO::write(len, output_buffer);
}

long LZ4IO::length() { return FileIO::length(); }

bool LZ4IO::eof() const { return FileIO::eof(); }

void LZ4IO::seek(long int nbytes, const IO::Seek seek) { FileIO::seek(nbytes, seek); }

unsigned long LZ4IO::getPos() { return FileIO::getPos(); }

bool LZ4IO::isWriteable() const { return FileIO::isWriteable(); }

bool LZ4IO::isReadable() const { return FileIO::isReadable(); }

bool LZ4IO::flush() { return FileIO::flush(); }

void LZ4IO::close() { FileIO::close(); }

void LZ4IO::open([[maybe_unused]] const char *path, IO::IOMode mode) {}

LZ4IO::LZ4IO(const char *path, IO::IOMode mode) : FileIO(path, mode) { this->open(path, mode); }