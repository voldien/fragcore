#include "Core/Log.h"
#include "Core/IO/IOUtil.h"
#include "Core/Ref.h"
#include "Core/dataStructure/Queue.h"
#include<spdlog/spdlog-inl.h>
#include <cstdarg>
#include <cstdio>
#include <list>

using namespace fragcore;

static Log::VERBOSITY g_verbosity = Log::Quite;
static IO *verboseIO = nullptr;
std::list<Ref<IO>> ios;
typedef struct log_io_buf_t {
	Log::VERBOSITY verbosity;
	Ref<IO> io;
} LogIOMap;
// static Queue<Ref<IO>> iosQueue;
// TOOD determine if queue, should be used.

void Log::setVerbosity(VERBOSITY verbosity) {
	switch (verbosity) {
	case VERBOSITY::Quite:
	case VERBOSITY::Error:
	case VERBOSITY::Verbose:
	case VERBOSITY::Warning:
	case VERBOSITY::Debug:
		g_verbosity = verbosity;
		break;
	default:
		throw InvalidArgumentException("");
	}
}

Log::VERBOSITY Log::getVerbosity() { return g_verbosity; }

int Log::log(VERBOSITY verbosity, const char *format, ...) {
	int l = 0;
	va_list vl;

	va_start(vl, format);
	l = logv(verbosity, format, vl);
	va_end(vl);

	return l;
}

int Log::log(const char *format, ...) {
	int l = 0;
	va_list vl;

	va_start(vl, format);
	l = Log::logv(Verbose, format, vl);
	va_end(vl);

	return l;
}

int Log::error(const char *format, ...) {
	int l = 0;
	va_list argptr;

	va_start(argptr, format);
	/*	output to stdout	*/
	l = Log::logv(Error, format, argptr);
	va_end(argptr);

	return l;
}

int Log::logv(VERBOSITY verbosity, const char *format, va_list va) {
	// TODO add IO object support.
	if (ios.size() > 0) {
		std::list<Ref<IO>>::iterator it = ios.begin();

		for (; it != ios.end(); it++) {
			Ref<IO> &ioRef = (*it);

			// TODO resolve for variable list variable.
			// IOUtil::format(ioRef, format);
		}
		if (verbosity <= getVerbosity()) {
			return vprintf(format, va);
		}
	}
	if (verbosity <= getVerbosity()) {
		return vprintf(format, va);
	}
	return 0;
}

void Log::addIOOutput(Ref<IO> &io, const VERBOSITY mapping) {

	io->increment();
	ios.push_back(io);
}

void Log::removeIOOutPut(Ref<IO> &io) {
	std::list<Ref<IO>>::iterator it = ios.begin();

	for (; it != ios.end(); it++) {
		if (*io == *(*it)) {
			(*it)->deincreemnt();
			ios.erase(it);
			break;
		}
	}
	throw RuntimeException("Invalid IO object");
}
