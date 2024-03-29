#include "Core/IO/IOUtil.h"
#include "Core/IO/FileIO.h"
#include "Core/SystemInfo.h"
#include <fmt/core.h>

using namespace fragcore;

long int IOUtil::loadFileMem(Ref<IO> &io, char **data) {
	char *d = nullptr;
	long int dataSize = 0;

	size_t current_pos = io->getPos();

	/*  Check if file is readable.  */
	if (!io->isReadable()) {
		throw InvalidArgumentException("Failed to read from IO: {}", io->getName());
	}
	// TODO encapsualte data to reuse

	// Page aligned;
	char buf[1024 * 4];
	long nbytes = 0;

	while ((nbytes = io->read(sizeof(buf), buf)) > 0) {
		d = static_cast<char *>(realloc(d, dataSize + nbytes));
		memcpy(&d[dataSize], buf, nbytes);
		dataSize += nbytes;
	}

	io->seek(current_pos, IO::Seek::SET);

	*data = d;
	return dataSize;
}

long int IOUtil::loadFile(Ref<IO> &in, Ref<IO> &out) {

	if (!in->isReadable()) {
		throw InvalidArgumentException("Failed to read from IO: {}", in->getName());
	}
	if (!out->isWriteable()) {
		throw InvalidArgumentException("Failed to write to IO: {}", out->getName());
	}

	char buf[1024 * 4];
	long nbytes;
	long dataSize = 0;
	while ((nbytes = in->read(sizeof(buf), buf)) > 0) {
		int outbytes = out->write(nbytes, buf);
		if (outbytes > 0) {
			/*	*/
			dataSize += nbytes;
		} else {
			throw RuntimeException("Error while reading IO: {}", out->getName());
		}
	}

	return dataSize;
}

long int IOUtil::loadStringMem(Ref<IO> &io, char **string) {
	long int nbytes;

	nbytes = loadFileMem(io, string);
	*string = static_cast<char *>(realloc(*string, nbytes + 1));
	(*string)[nbytes] = '\0';

	return nbytes;
}

long int IOUtil::loadString(Ref<IO> &in, Ref<IO> &out) {

	if (!in->isReadable())
		throw InvalidArgumentException("Failed to read from IO: {}", in->getName());
	if (!out->isWriteable())
		throw InvalidArgumentException("Failed to write to IO: {}", out->getName());

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