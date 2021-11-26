#include "Core/IO/IOUtil.h"

#include <fmt/core.h>

using namespace fragcore;

long int IOUtil::loadFileMem(Ref<IO> &io, char **data) {
	char *d = nullptr;
	long dataSize = 0;

	/*  Check if file is readable.  */
	if (!io->isReadable())
		throw InvalidArgumentException("Failed to read from IO: {}", io->getName());
	// TODO encapsualte data to reuse
	// Page aligned;
	char buf[1024 * 4];
	long nbytes;
	while (!io->eof() && (nbytes = io->read(sizeof(buf), buf)) > 0) {
		d = static_cast<char *>(realloc(d, dataSize + nbytes));
		memcpy(&d[dataSize], buf, nbytes);
		dataSize += nbytes;
	}

	*data = d;
	return dataSize;
}

long int IOUtil::loadFile(Ref<IO> &in, Ref<IO> &out) {
	if (!in->isReadable())
		throw InvalidArgumentException("Failed to read from IO: {}", in->getName());
	if (!out->isWriteable())
		throw InvalidArgumentException("Failed to write to IO: {}", out->getName());

	char buf[1024 * 4];
	long nbytes;
	long dataSize = 0;
	while ((nbytes = in->read(sizeof(buf), buf)) > 0) {
		int outbytes = out->write(nbytes, buf);
		if (outbytes != nbytes) {
		}
		dataSize += nbytes;
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

// long int IOUtil::format(Ref<IO> &io, const char *vformat, ...) noexcept(noexcept(vformat != nullptr)) {
// 	va_list argptr;
// 	va_start(argptr, format);
// 	char buf[1024]; // Page;
// 	if (vformat == nullptr)
// 		throw InvalidPointerException("vformat invalid");

// 	// TODO add support for determine if fully or partial written.
// 	long int i = vsnprintf(buf, sizeof(buf), vformat, argptr);
// 	long nrBytes = io->write(i, (const void *)buf);
// 	va_end(argptr);
// 	return nrBytes;
// }