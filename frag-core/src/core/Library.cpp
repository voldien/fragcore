#include "Core/Library.h"

#include <SDL2/SDL.h>
#include <fmt/core.h>
#include <stdexcept>
#include <utility>
using namespace fragcore;

Library::Library(void) { this->mlib = nullptr; }

Library::Library(const char *clibrary) {
	this->mlib = nullptr;
	this->open(clibrary);
	this->name = clibrary;
}

Library::Library(const Library &library) : Library(library.name.c_str()) {}

Library::Library(Library &&other) { std::exchange(this->mlib, other.mlib); }

Library::~Library(void) { /*	Nothing to release. Done by the kernel itself.	*/
}

bool Library::open(const char *clibrary) {
	this->mlib = SDL_LoadObject(clibrary);

	/*	Check for error.	*/
	if (this->mlib == nullptr) {

		throw RuntimeException("Failed open library : {}\n", SDL_GetError());
	}

	return this->mlib != nullptr;
}

void Library::close(void) {
	SDL_UnloadObject(this->mlib);
	this->mlib = nullptr;
}

bool Library::isValid(void) const { return this->mlib != nullptr; }

void *Library::getfunc(const char *pProcName) {
	void *func = SDL_LoadFunction(this->mlib, pProcName);

	if (func == nullptr) {
		std::string sdlerror = fmt::format("Failed to load function {}, {} from library {}.\n", pProcName,
										   SDL_GetError(), this->name.c_str());
		throw RuntimeException(sdlerror);
	}

	return func;
}
