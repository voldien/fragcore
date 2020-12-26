#include "Core/IO/IOUtil.h"
#include "Exception/InvalidArgumentException.h"
#include"Utils/StringUtil.h"

using namespace fragcore;

long int IOUtil::loadFileMem( Ref<IO> &io, char **data)
{
	char *d = NULL;
	long dataSize = 0;

	/*  Check if file is readable.  */
	if (!io->isReadable())
		throw InvalidArgumentException(fvformatf("Failed to read from IO: %s", io->getName()));

	// Page aligned;
	char buf[1024 * 4];
	long nbytes;
	while ((nbytes = io->read(sizeof(buf), buf)) > 0)
	{
		d = (char *)realloc(d, dataSize + nbytes);
		memcpy(&d[dataSize], buf, nbytes);
		dataSize += nbytes;
	}

	*data = d;
	return dataSize;
}

long int IOUtil::loadFile(Ref<IO> &in, Ref<IO> &out)
{
	if (!in->isReadable())
		throw InvalidArgumentException(fvformatf("Failed to read from IO: %s", in->getName()));
	if (!out->isWriteable())
		throw InvalidArgumentException(fvformatf("Failed to write to IO: %s", out->getName()));

	char buf[1024 * 4];
	long nbytes;
	long dataSize = 0;
	while ((nbytes = in->read(sizeof(buf), buf)) > 0)
	{
		int outbytes = out->write(nbytes, buf);
		if (outbytes != nbytes){

		}
		dataSize += nbytes;
	}

	return dataSize;
}

long int IOUtil::loadStringMem( Ref<IO> &io, char **string)
{
	long int nbytes;

	nbytes = loadFileMem(io, string);
	*string = (char *)realloc(*string, nbytes + 1);
	(*string)[nbytes] = '\0';

	return nbytes;
}

long int IOUtil::loadString( Ref<IO> &in, Ref<IO> &out) {

	if (!in->isReadable())
		throw InvalidArgumentException(fvformatf("Failed to read from IO: %s", in->getName()));
	if (!out->isWriteable())
		throw InvalidArgumentException(fvformatf("Failed to write to IO: %s", out->getName()));

	long int nbytes;

	nbytes = IOUtil::loadFile(in, out);
	char term = '\n';
	out->write(sizeof(term), &term);

	return nbytes;
}

long int IOUtil::format( Ref<IO> &io, const char *vformat, ...){
	va_list argptr;
	va_start(argptr, format);
	char buf[1024]; // Page;
	
	//TODO add support for determine if fully or partial written.
	long int i = vsnprintf(buf, sizeof(buf), vformat, argptr);
	io->write(i, (const void*)buf);
	va_end(argptr);
}