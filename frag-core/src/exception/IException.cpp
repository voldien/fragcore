#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <sstream>
#include"Exception/IException.h"
using namespace fragcore;

static std::string Backtrace(int skip = 0) {
	void *callstack[128];
	const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
	char buf[1024];
	int nFrames = backtrace(callstack, nMaxFrames);
	char **symbols = backtrace_symbols(callstack, nFrames);

	std::ostringstream trace_buf;
	for (int i = skip; i < nFrames; i++) {
		Dl_info info;
		if (dladdr(callstack[i], &info)) {
			char *demangled = NULL;
			int status;
			demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
			const char *symbol = status == 0 ? demangled : info.dli_sname;
			snprintf(buf, sizeof(buf), "%-3d %*0p %s + %zd\n",
			         i, 2 + sizeof(void *) * 2,
			         symbol,
			         (char *) callstack[i] - (char *) info.dli_saddr);
			free(demangled);
		} else {
			snprintf(buf, sizeof(buf), "%-3d %*0p\n",
			         i, 2 + sizeof(void *) * 2, callstack[i]);
		}
		trace_buf << buf;

		snprintf(buf, sizeof(buf), "%s\n", symbols[i]);
		trace_buf << buf;
	}
	free(symbols);
	if (nFrames == nMaxFrames)
		trace_buf << "[truncated]\n";
	return trace_buf.str();
}

IException::IException(void) {
	this->generateStackTrace();
}

IException::IException(const char *what) {
	this->mwhat = std::string(what);
	this->generateStackTrace();
}

IException::IException(const std::string &what) {
	this->mwhat = what;
	this->generateStackTrace();
}


const std::string &IException::getBackTrace(void) const {
	return this->stackTrace;
}

void IException::generateStackTrace(void) {
	/*	*/
#if defined(_DEBUG)
	this->stackTrace = Backtrace(4);    // Get the stack before the exception was invoked.
#endif()
}

const char *IException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
	return this->mwhat.c_str();
}
