#include "Core/Hash.h"

#include <fmt/core.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
using namespace fragcore;

Hash::Hash(Hash::ALGORITHM algorithm) {

	initHash(algorithm);
	/*      */
	this->algorithm = algorithm;
	this->nbytes = 0;
}

Hash::Hash(Hash &&other) {
	// std::swap(other.context);
}

Hash::~Hash() { free(this->context); }

void Hash::update(const void *pdata, size_t nbytes) {
	switch (this->algorithm) {
	case MD5:
		nbytes += MD5_Update((MD5_CTX *)this->context, pdata, nbytes);
		break;
	case SHA128:
		nbytes += SHA1_Update((SHA_CTX *)this->context, pdata, nbytes);
		break;
	case SHA256:
		nbytes += SHA256_Update((SHA256_CTX *)this->context, pdata, nbytes);
		break;
	case SHA512:
		nbytes += SHA512_Update((SHA512_CTX *)this->context, pdata, nbytes);
		break;
	default:
		assert(0);
		throw NotSupportedException("Not supported");
	}
	this->nbytes += nbytes;
}

void Hash::update(Ref<IO> &io) {
	char buffer[4096];
	long int prev_pos = io->getPos();
	long int len;

	while ((len = io->read(sizeof(buffer), buffer)) >= 0) {
		this->update(buffer, len);
	}
	/*	retain the original state.	*/
	io->seek(prev_pos, IO::Seek::SET);
}

void Hash::final(std::vector<unsigned char> &hash) {
	switch (this->algorithm) {
	case MD5:
		hash.resize(MD5_DIGEST_LENGTH);
		MD5_Final(hash.data(), (MD5_CTX *)this->context);
		break;
	case SHA128:
	case SHA256:
	case SHA512:
	default:
		throw NotSupportedException("Not supported");
	}

	/**/
}
void Hash::reset() noexcept {
	this->nbytes = 0;
	initHash(getAlgorithm());
}

unsigned int Hash::getResultSize() const {
	switch (this->getAlgorithm()) {
	case MD5:
	case SHA128:
	case SHA256:
	case SHA512:
		return 0;
	default:
		assert(0);
	}
}

unsigned long int Hash::getByteRead() const { return this->nbytes; }

Hash::ALGORITHM Hash::getAlgorithm() const noexcept { return this->algorithm; }

void Hash::initHash(ALGORITHM algorithm) {

	/*	*/
	switch (algorithm) {
	case MD5:
		this->context = malloc(sizeof(MD5_CTX));
		MD5_Init((MD5_CTX *)this->context);
		break;
	case SHA128:
		this->context = malloc(sizeof(SHA_CTX));
		SHA1_Init((SHA_CTX *)this->context);
		break;
	case SHA224:
		throw NotSupportedException();
	case SHA256:
		this->context = malloc(sizeof(SHA256_CTX));
		SHA256_Init((SHA256_CTX *)this->context);
		break;
	case SHA512:
		throw NotSupportedException();
	default:
		throw InvalidArgumentException("Invalid hash algorithm - {}", algorithm);
	}
}
