#include "Core/Library.h"

#include <fmt/core.h>
#include <unistd.h>
#include <utility>
using namespace fragcore;

Library::Library() { this->mlib = nullptr; }

Library::Library(const char *clibrary) {
	this->mlib = nullptr;
	this->open(clibrary);
	this->name = clibrary;
}

Library::Library(const Library &library) : Library(library.name.c_str()) {}

Library::Library(Library &&other) { std::exchange(this->mlib, other.mlib); }

Library::~Library() { /*	Nothing to release. Done by the kernel itself.	*/
}

bool Library::open(const char *clibrary) {
	this->mlib = dlopen(clibrary, RTLD_LAZY | RTLD_NODELETE);

	/*	Check for error.	*/
	// if (this->mlib == nullptr) {

	// 	throw RuntimeException("Failed open library : {}\n", SDL_GetError());
	// }

	return this->mlib != nullptr;
}

void Library::close() {
	dlclose(this->mlib);
	this->mlib = nullptr;
}

bool Library::isValid() const { return this->mlib != nullptr; }

void *Library::getfunc(const char *pProcName) {
	void *func = dlsym(this->mlib, pProcName);

	if (func == nullptr) {
		// std::string sdlerror = fmt::format("Failed to load function {}, {} from library {}.\n", pProcName,
		// 								   SDL_GetError(), this->name.c_str());
		//throw RuntimeException(sdlerror);
	}

	return func;
}
