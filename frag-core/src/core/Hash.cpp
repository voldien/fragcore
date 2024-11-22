#include "Core/Hash.h"

#include <fmt/core.h>
#include <magic_enum.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>

using namespace fragcore;

Hash::Hash(Hash::ALGORITHM algorithm) : nbytes(0) {

	this->initHash(algorithm);
	/*      */
}

Hash::Hash(Hash &&other) : algorithm(other.algorithm) { std::swap(other.context, this->context); }

Hash::~Hash() { free(this->context); }

void Hash::update(const void *pdata, size_t nbytes) {
	size_t status = 0;
	switch (this->getAlgorithm()) {
	case Hash::ALGORITHM::MD5:
		status = MD5_Update(static_cast<MD5_CTX *>(this->context), pdata, nbytes);
		break;
	case Hash::ALGORITHM::SHA128:
		status = SHA1_Update(static_cast<SHA_CTX *>(this->context), pdata, nbytes);
		break;
	case Hash::ALGORITHM::SHA224:
		status = SHA224_Update(static_cast<SHA256_CTX *>(this->context), pdata, nbytes);
		break;
	case Hash::ALGORITHM::SHA256:
		status = SHA256_Update(static_cast<SHA256_CTX *>(this->context), pdata, nbytes);
		break;
	case Hash::ALGORITHM::SHA512:
		status = SHA512_Update(static_cast<SHA512_CTX *>(this->context), pdata, nbytes);
		break;
	default:
		assert(0);
	}
	if (status != 1) {
		throw RuntimeException("Error");
	}
	this->nbytes += nbytes;
}

void Hash::update(Ref<IO> &io_in) {
	char buffer[4096];
	long int prev_pos = io_in->getPos();
	long int len = 0;

	/*	*/
	while ((len = io_in->read(sizeof(buffer), buffer)) > 0) {
		this->update(buffer, len);
	}

	/*	retain the original state.	*/
	io_in->seek(prev_pos, IO::Seek::SET);
}

void Hash::final(std::vector<unsigned char> &hash) {
	int status = 0;
	switch (this->algorithm) {
	case Hash::ALGORITHM::MD5:
		hash.resize(MD5_DIGEST_LENGTH);
		status = MD5_Final(hash.data(), static_cast<MD5_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA128:
		hash.resize(SHA_DIGEST_LENGTH);
		status = SHA1_Final(hash.data(), static_cast<SHA_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA256:
		hash.resize(SHA256_DIGEST_LENGTH);
		status = SHA256_Final(hash.data(), static_cast<SHA256_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA512:
		hash.resize(SHA512_DIGEST_LENGTH);
		status = SHA512_Final(hash.data(), static_cast<SHA512_CTX *>(this->context));
		break;
	default:
		throw NotImplementedException();
	}
	if (status != 1) {
		throw RuntimeException("Failed to compute final Hash");
	}
}

void Hash::reset() {
	this->nbytes = 0;
	this->initHash(this->getAlgorithm());
}

unsigned int Hash::getResultSize() const {
	switch (this->getAlgorithm()) {
	case Hash::ALGORITHM::MD5:
		return MD5_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA128:
		return SHA_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA224:
		return SHA224_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA256:
		return SHA256_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA384:
		return SHA384_DIGEST_LENGTH;
	case Hash::ALGORITHM::SHA512:
		return SHA512_DIGEST_LENGTH;
	default:
		return 0;
	}
}

size_t Hash::getByteRead() const { return this->nbytes; }

Hash::ALGORITHM Hash::getAlgorithm() const noexcept { return this->algorithm; }

void Hash::initHash(ALGORITHM algorithm) {

	/*	*/
	int status = 0;
	switch (algorithm) {
	case Hash::ALGORITHM::MD5:
		this->context = malloc(sizeof(MD5_CTX));
		status = MD5_Init(static_cast<MD5_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA128:
		this->context = malloc(sizeof(SHA_CTX));
		status = SHA1_Init(static_cast<SHA_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA224:
		this->context = malloc(sizeof(SHA256_CTX));
		status = SHA224_Init(static_cast<SHA256_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA256:
		this->context = malloc(sizeof(SHA256_CTX));
		status = SHA256_Init(static_cast<SHA256_CTX *>(this->context));
		break;
	case Hash::ALGORITHM::SHA512:
		this->context = malloc(sizeof(SHA512_CTX));
		status = SHA512_Init(static_cast<SHA512_CTX *>(this->context));
		break;
	default:
		throw InvalidArgumentException("Invalid hash algorithm - {}", static_cast<int>(algorithm));
	}
	/*	*/

	if (status != 1) {
		throw RuntimeException("Failed to initlize Hash Algorithm {}", magic_enum::enum_name(algorithm));
	}
	this->algorithm = algorithm;
}
