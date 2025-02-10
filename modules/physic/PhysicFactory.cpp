#include "PhysicFactory.h"
#include "Core/Library.h"

using namespace fragcore;

/**
 *	Internal function data type for creating physic
 *	interface.
 */
using pcreateInternalPhysicInterface = PhysicInterface *(*)(IConfig *);

/*	TODO update with the new naming.	*/
const char *bulletlibpath = "libfragcore-pbu.so"; /*	Default bullet library.	*/

PhysicInterface *PhysicFactory::createPhysic(PhysicAPI api, IConfig *overrideOption) {
	return PhysicFactory::createPhysic(PhysicFactory::getInterfaceLibraryPath(api), overrideOption);
}

PhysicInterface *PhysicFactory::createPhysic(const char *libpath, IConfig *overrideOption) {

	const char *funcsymbol = "createInternalPhysicInterface";
	pcreateInternalPhysicInterface pfunc = nullptr;
	PhysicInterface *interface = nullptr;
	Library library;

	/*	Validate parameters.	*/
	if (libpath == nullptr) {
		throw InvalidArgumentException("Invalid filepath do dynamic library: {}", libpath);
	}

	/*	Open dynamicInterface library and create dynamicInterface object.	*/
	library.open(libpath);
	if (library.isValid()) {
		pfunc = (pcreateInternalPhysicInterface)library.getfunc(funcsymbol);

		if (pfunc) {
			interface = pfunc(overrideOption);
		} else {
			/*	Error	*/
			//	Log::log("Couldn't find symbol %s in %s.\n", funcsymbol, libpath);
		}
	} else {
		// Log::error("Failed loading %s library for creating physic dynamicInterface.\n", libpath);
	}

	return interface;
}

const char *PhysicFactory::getInterfaceLibraryPath(const PhysicAPI api) {
#ifdef FV_UNIX
	switch (api) {
	case PhysicFactory::PhysicAPI::Bullet:
		return bulletlibpath;
	default:
		throw InvalidArgumentException("Invalid API");
	}
#else
#endif
}
