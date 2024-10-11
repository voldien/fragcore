#include "IO/IOUtil.h"
#include "FragDef.h"
#include <fmt/core.h>

using namespace fragcore;

long int IOUtil::loadFileMem(Ref<IO> &io, char **data) {
	uint8_t *populatedMemory = nullptr;
	long int numberByteRead = 0;

	const size_t current_pos = io->getPos();

	/*  Check if file is readable.  */
	if (!io->isReadable()) {
		throw InvalidArgumentException("Failed to read from IO: {}", io->getName());
	}

	// Page aligned;
	uint8_t buf[1024 * 4];
	long nbytes = 0;

	while ((nbytes = io->read(sizeof(buf), buf)) > 0) {
		populatedMemory = static_cast<uint8_t *>(realloc(populatedMemory, numberByteRead + nbytes));

		if (populatedMemory == nullptr) {
			throw SystemException();
		}

		memcpy(&populatedMemory[numberByteRead], buf, nbytes);
		numberByteRead += nbytes;
	}

	io->seek(current_pos, IO::Seek::SET);

	*data = (char *)populatedMemory;
	return numberByteRead;
}

long int IOUtil::loadFile(Ref<IO> &inRef, Ref<IO> &outRef) {

	if (!inRef->isReadable()) {
		throw InvalidArgumentException("Failed to read from IO: {}", inRef->getName());
	}
	if (!outRef->isWriteable()) {
		throw InvalidArgumentException("Failed to write to IO: {}", outRef->getName());
	}

	char buf[1024 * 4];
	long nbytes;
	long dataSize = 0;
	while ((nbytes = inRef->read(sizeof(buf), buf)) > 0) {
		int outbytes = outRef->write(nbytes, buf);
		if (outbytes > 0) {
			/*	*/
			dataSize += nbytes;
		} else {
			throw RuntimeException("Error while reading IO: {}", outRef->getName());
		}
	}

	return dataSize;
}

long int IOUtil::loadStringMem(Ref<IO> &io, char **string) {
	long int nbytes;

	nbytes = IOUtil::loadFileMem(io, string);
	*string = static_cast<char *>(realloc(*string, nbytes + 1));
	(*string)[nbytes] = '\0';

	return nbytes;
}

long int IOUtil::loadString(Ref<IO> &in, Ref<IO> &out) {

	if (!in->isReadable()) {
		throw InvalidArgumentException("Failed to read from IO: {}", in->getName());
	}
	if (!out->isWriteable()) {
		throw InvalidArgumentException("Failed to write to IO: {}", out->getName());
	}

	long int nbytes;

	nbytes = IOUtil::loadFile(in, out);
	char term = '\n';
	out->write(sizeof(term), &term);

	return nbytes;
}

long int IOUtil::saveFileMem(Ref<IO> &io, char *data, size_t size) {

	if (!io->isWriteable()) {
		throw InvalidArgumentException("Failed to write to IO: {}", io->getName());
	}

	long dataSize;
	dataSize = io->write(size, data);

	// char buf[1024 * 4];
	// long nbytes;
	// long dataSize = 0;
	// while ((nbytes = in->write(sizeof(buf), buf)) > 0) {
	//	int outbytes = out->write(nbytes, buf);
	//	if (outbytes > 0) {
	//		/*	*/
	//		dataSize += nbytes;
	//	} else {
	//		throw RuntimeException("Error while reading IO: {}", out->getName());
	//	}
	//}

	return dataSize;
}