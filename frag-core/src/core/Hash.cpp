#include"Core/Hash.h"
#include"Utils/StringUtil.h"
#include<openssl/md5.h>
#include <Exception/InvalidArgumentException.h>
#include <Exception/NotSupportedException.h>

using namespace fragcore;

Hash::Hash(Hash::ALGORITHM algorithm) {
	switch(algorithm) {
		case MD5:
			this->context = malloc(sizeof(MD5_CTX));
			MD5_Init ((MD5_CTX*)this->context);
			break;
		case SHA128:
		case SHA256:
		case SHA512:
			throw NotSupportedException();
		default:
			throw InvalidArgumentException(fvformatf("Invalid hash enumerator - %d", algorithm));
	}

	/*      */
	this->algorithm = algorithm;
	this->nbytes = 0;
}

Hash::~Hash(void) {
	free(this->context);
}

void Hash::update(const void *pdata, size_t nbytes) {
	switch(this->algorithm){
		case MD5:
			nbytes += MD5_Update((MD5_CTX *)this->context, pdata, nbytes);
			break;
		case SHA128:
		case SHA256:
		case SHA512:
		default:
			throw NotSupportedException("Not supported");
	}
}

void Hash::update(Ref<IO> &io){
	char buffer[4096];
	long int prev_pos = io->getPos();
	long int len;

	while((len = io->read(sizeof(buffer),buffer)) >= 0){
		this->update(buffer, len);
	}
	/*	retain the original state.	*/
	io->seek(prev_pos, IO::Seek::SET);
}

void Hash::final(std::vector<unsigned char> &hash) {
	switch(this->algorithm){
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
}

unsigned int Hash::getResultSize(void) const{
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

long int Hash::getByteRead(void) const{
	return this->nbytes;
}

Hash::ALGORITHM Hash::getAlgorithm(void) const {
	return this->algorithm;
}

Hash::Hash(const Hash &other) {

}

Hash::Hash(void) {

}
