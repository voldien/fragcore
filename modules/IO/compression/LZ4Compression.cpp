#include "LZ4Compression.h"
#include <lz4.h>

using namespace fragcore;

LZ4Compression::LZ4Compression(const size_t block_size) : block_size((int)LZ4_COMPRESSBOUND(block_size)) {}

ssize_t LZ4Compression::inflate(const void *data_in, size_t inSize, void *out, size_t out_size) {

	return LZ4_decompress_safe(static_cast<const char *>(data_in), static_cast<char *>(out), inSize, out_size);
}

ssize_t LZ4Compression::deflate(const void *data_in, size_t inSize, void *out, size_t out_size) {

	return LZ4_compress_default(static_cast<const char *>(data_in), static_cast<char *>(out), (int)inSize, (int)inSize);
}