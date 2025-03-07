#include "Core/Library.h"

#include <filesystem>
#include <fmt/core.h>
#include <unistd.h>
using namespace fragcore;

Library::Library() : mlib(nullptr) {}

Library::Library(const char *clibrary) : mlib(nullptr) {

	this->open(clibrary);
	if (clibrary) {
		this->path = clibrary;
		/*	*/

	} else {
		this->path = "";
	}
}

Library::Library(const std::string &clibrary) : Library(clibrary.c_str()) {}

Library::Library(const Library &other) : Library(other.path.c_str()) {}

Library::Library(Library &&other) : mlib(other.mlib) {}

Library::~Library() { /*	Nothing to release. Done by the kernel itself.	*/ }

Library &Library::operator=(Library &&other) {
	Object::operator=(other);

	this->path = other.path;
	this->mlib = other.mlib;
	// TODO: fix.
	return *this;
}

Library &Library::operator=(const Library &other) {
	Object::operator=(other);
	this->path = other.path;
	this->open(this->getPath().c_str());
	// TODO: fix.
	return *this;
}

bool Library::open(const char *clibrary) {
#ifdef FV_UNIX
	this->mlib = dlopen(clibrary, RTLD_LAZY | RTLD_NODELETE);

	/*	Check for error.	*/
	if (this->mlib == nullptr) {
		throw RuntimeException("Failed open library : {}", dlerror());
	}

#else

#endif

	// TODO extract filename without extension.
	std::filesystem::path pathObj(path);
	if (pathObj.has_stem()) {
		this->setName(pathObj.stem().string());
	}
	return this->mlib != nullptr;
}

void Library::close() {
#ifdef FV_UNIX
	int rcode = dlclose(this->mlib);
	if (rcode < 0) {
		throw SystemException(errno, std::system_category(), "Failed to close library: {}", dlerror());
	}
#endif
	this->mlib = nullptr;
}

bool Library::isValid() const noexcept { return this->mlib != nullptr; }

void *Library::getfunc(const char *cfunctionName) {
#ifdef FV_UNIX
	void *func = dlsym(this->mlib, cfunctionName);

	if (func == nullptr) {
		throw RuntimeException("Couldn't load function with symbol {} | {}", cfunctionName, dlerror());
	}

	return func;
#else
	return nullptr;
#endif
}
