#include "GZCompression.h"

#include <bzlib.h>

using namespace fragcore;

ssize_t GZCompression::inflate(const void *data_in, size_t inSize, void *out, size_t out_size) { return -1; }

ssize_t GZCompression::deflate(const void *data_in, size_t inSize, void *out, size_t out_size) { return -1; }