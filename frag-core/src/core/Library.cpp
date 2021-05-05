#include "Core/Library.h"
#include "Exception/RuntimeException.h"
#include "Utils/StringUtil.h"
#include <SDL2/SDL.h>
#include <stdexcept>
#include<fmt/core.h>
#include <utility>
using namespace fragcore;

Library::Library(void) { this->mlib = nullptr; }

Library::Library(const char *clibrary) {
	this->mlib = nullptr;
	this->open(clibrary);
	this->name = clibrary;
}

Library::Library(const Library &library) {
	this->mlib = nullptr;
	this->open(library.name.c_str());
	this->name = library.name;
}

Library::Library(Library &&other) { std::exchange(this->mlib, other.mlib); }

Library::~Library(void) { /*	Nothing to release. Done by the kernel itself.	*/
}

bool Library::open(const char *clibrary) {
	this->mlib = SDL_LoadObject(clibrary);

	/*	Check for error.	*/
	if (this->mlib == nullptr) {

		std::string sdlerror = fmt::format("Failed open library : %s\n", SDL_GetError());
		throw RuntimeException(sdlerror);
	}

	return this->mlib != nullptr;
}

void Library::close(void) { SDL_UnloadObject(this->mlib); }

bool Library::isValid(void) const { return this->mlib != nullptr; }

void *Library::getfunc(const char *pProcName) {
	void *func = SDL_LoadFunction(this->mlib, pProcName);

	if (func == nullptr) {
		std::string sdlerror = fmt::format("Failed to load function %s, %s from library %s.\n", pProcName,
										   SDL_GetError(), this->name.c_str());
		throw RuntimeException(sdlerror);
	}

	return func;
}
