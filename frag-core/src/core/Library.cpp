#include"Core/Library.h"
#include<SDL2/SDL.h>
#include <stdexcept>
#include "Exception/RuntimeException.h"
#include"Utils/StringUtil.h"
using namespace fragcore;

Library::Library(void) {
	this->mlib = NULL;
}

Library::Library(const char *clibrary) {
	this->mlib = NULL;
	this->open(clibrary);
	this->name = clibrary;
}

Library::Library(const Library &library) {
	this->mlib = NULL;
	this->open(library.name.c_str());
	this->name = library.name;
}

Library::~Library(void) {
	/*	Nothing to release. Done by the kernel itself.	*/
}

bool Library::open(const char *clibrary) {
	this->mlib = SDL_LoadObject(clibrary);

	/*	Check for error.	*/
	if (this->mlib == NULL) {

		std::string sdlerror = fvformatf("Failed open library : %s\n", SDL_GetError());
		throw RuntimeException(sdlerror);
	}

	return this->mlib != NULL;
}

void Library::close(void) {
	SDL_UnloadObject(this->mlib);
}

bool Library::isValid(void) const {
	return this->mlib != NULL;
}

void *Library::getfunc(const char *pProcName) {
	void *func = SDL_LoadFunction(this->mlib, pProcName);

	if (func == NULL) {
		std::string sdlerror = fvformatf("Failed to load function %s, %s from library %s.\n", pProcName, SDL_GetError(),
		                                 this->name.c_str());
		throw RuntimeException(sdlerror);
	}

	return func;
}
