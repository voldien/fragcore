#include "BZ2Compression.h"
#include "FragDef.h"
#include <bzlib.h>
#include <cstdio>

using namespace fragcore;

BZ2Compression::BZ2Compression(const size_t compressionFactor) {
	/*	Allocate.	*/
	bzip2com = (bz_stream *)malloc(sizeof(bz_stream));
	bzip2uncom = (bz_stream *)malloc(sizeof(bz_stream));

	memset(bzip2com, 0, sizeof(bz_stream));
	memset(bzip2uncom, 0, sizeof(bz_stream));

	/*	*/
	int err = BZ2_bzCompressInit(bzip2com, compressionFactor, 0, 0);
	if (err < BZ_OK) {
		throw RuntimeException("bzip2 failed to initialize with error {}", err);
	}

	err = BZ2_bzDecompressInit(bzip2uncom, 0, 0);
	if (err < BZ_OK) {
		throw RuntimeException("bzip2 failed to initialize with error {}", err);
	}
}

BZ2Compression::~BZ2Compression() {
	int bzError = BZ2_bzCompressEnd(bzip2com);
	bzError = BZ2_bzDecompressEnd(bzip2uncom);
}

ssize_t BZ2Compression::inflate(const void *data_in, size_t inSize, void *out, size_t out_size) {
	// int len = BZ2_bzRead(&bzerror, this->bzFile, pbuffer, nbytes);
	bzip2uncom->next_in = (char *)data_in;
	bzip2uncom->avail_in = inSize;
	bzip2uncom->next_out = (char *)out;
	bzip2uncom->avail_out = out_size;

	int err = BZ2_bzDecompress(bzip2uncom);
	switch (err) {
	default:
	case BZ_PARAM_ERROR:
	case BZ_DATA_ERROR:
	case BZ_DATA_ERROR_MAGIC:
	case BZ_MEM_ERROR:
	case BZ_STREAM_END:
		throw RuntimeException("bzip2 failed to initialize with error {}", err);
	case BZ_OK:
		break;
	}

	ssize_t inflen = out_size - bzip2uncom->avail_out;
	return inflen; // inSize - bzip2uncom->avail_in;
}

ssize_t BZ2Compression::deflate(const void *data_in, size_t inSize, void *out, size_t out_size) {

	bzip2com->next_in = (char *)data_in;
	bzip2com->avail_in = inSize;
	bzip2com->next_out = (char *)out;
	bzip2com->avail_out = out_size;

	int err = BZ2_bzCompress(bzip2com, BZ_RUN);
	err = BZ2_bzCompress(bzip2com, BZ_FLUSH);

	switch (err) {
	case BZ_PARAM_ERROR:
	case BZ_DATA_ERROR:
	case BZ_DATA_ERROR_MAGIC:
	case BZ_MEM_ERROR:
	case BZ_STREAM_END:
	case BZ_OK:
		break;
	}

	ssize_t deflen = out_size - bzip2com->avail_out;
	return deflen;
}