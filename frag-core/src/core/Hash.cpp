#include "Core/Hash.h"

#include <fmt/core.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
using namespace fragcore;

Hash::Hash(Hash::ALGORITHM algorithm) {

	this->initHash(algorithm);
	/*      */
	this->algorithm = algorithm;
	this->nbytes = 0;
}

Hash::Hash(Hash &&other) {
	std::swap(other.context, this->context);
	this->algorithm = other.algorithm;
}

Hash::~Hash() { free(this->context); }

void Hash::update(const void *pdata, size_t nbytes) {
	switch (this->algorithm) {
	case Hash::ALGORITHM::MD5:
		nbytes += MD5_Update(static_cast<MD5_CTX *>(this->context), pdata, nbytes);
		break;
	case Hash::ALGORITHM::SHA128:
		nbytes += SHA1_Update(static_cast<SHA_CTX *>(this->context), pdata, nbytes);
		break;
	case Hash::ALGORITHM::SHA256:
		nbytes += SHA256_Update(static_cast<SHA256_CTX *>(this->context), pdata, nbytes);
		break;
	case Hash::ALGORITHM::SHA512:
		nbytes += SHA512_Update(static_cast<SHA512_CTX *>(this->context), pdata, nbytes);
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

	/*	*/
	while ((len = io->read(sizeof(buffer), buffer)) >= 0) {
		this->update(buffer, len);
	}
	/*	retain the original state.	*/
	io->seek(prev_pos, IO::Seek::SET);
}

void Hash::final(std::vector<unsigned char> &hash) {
	switch (this->algorithm) {
	case Hash::ALGORITHM::MD5:
		hash.resize(MD5_DIGEST_LENGTH);
		MD5_Final(hash.data(), static_cast<MD5_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA128:
	case Hash::ALGORITHM::SHA256:
	case Hash::ALGORITHM::SHA512:
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
	case Hash::ALGORITHM::MD5:
		return MD5_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA128:
		return SHA_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA256:
		return SHA256_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA512:
		return SHA512_DIGEST_LENGTH;
	default:
		assert(0);
		return 0;
	}
}

unsigned long int Hash::getByteRead() const { return this->nbytes; }

Hash::ALGORITHM Hash::getAlgorithm() const noexcept { return this->algorithm; }

void Hash::initHash(ALGORITHM algorithm) {

	/*	*/
	switch (algorithm) {
	case Hash::ALGORITHM::MD5:
		this->context = malloc(sizeof(MD5_CTX));
		MD5_Init(static_cast<MD5_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA128:
		this->context = malloc(sizeof(SHA_CTX));
		SHA1_Init(static_cast<SHA_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA224:
		throw NotSupportedException();
	case Hash::ALGORITHM::SHA256:
		this->context = malloc(sizeof(SHA256_CTX));
		SHA256_Init(static_cast<SHA256_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA512:
		this->context = malloc(sizeof(SHA512_CTX));
		SHA512_Init(static_cast<SHA512_CTX *>(this->context));
		throw NotSupportedException();
	default:
		throw InvalidArgumentException("Invalid hash algorithm - {}", static_cast<int>(this->algorithm));
	}
}
