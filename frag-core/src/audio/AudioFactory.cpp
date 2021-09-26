#include "Audio/AudioFactory.h"
#include "Core/Library.h"
#include "Core/Log.h"

// <Exception/InvalidArgumentException.h>
using namespace fragcore;

typedef AudioInterface *(*pcreateinternalaudio)(IConfig *config);

AudioInterface *AudioFactory::createAudioInterface(AudioAPI api, IConfig *config) {
	return AudioFactory::createAudioInterface(getInterfaceLibraryPath(api), config);
}

AudioInterface *AudioFactory::createAudioInterface(const char *cpathlib, IConfig *config) {
	Library library;
	AudioInterface *interface = nullptr;
	const char *funcsymbol = "createInternalAudioInterface";
	pcreateinternalaudio pfunc;

	if (cpathlib == nullptr)
		throw InvalidArgumentException(fmt::format("Invalid filepath do dynamic library: %s", cpathlib));

	// TODO allow for the plugin to have a default state.

	/*	Open library and validate.	*/
	library.open(cpathlib);
	if (library.isValid()) {

		/*	Get factory function for creating rendering dynamicInterface instance.	*/
		pfunc = (pcreateinternalaudio)library.getfunc(funcsymbol);
		interface = pfunc(config);

		/*	Assign resource object in order to work.	*/
		// interface->setResource(resources);
	} else {
		Log::error("Failed loading: %s, library for creating audio interface.\n", cpathlib);
	}

	return interface;
}

const char *AudioFactory::getInterfaceLibraryPath(AudioAPI api) {
#ifdef FV_UNIX
	// TODO add info string for the exception of what api value.
	switch (api) {
	case AudioFactory::OpenAL:
		return "libfragcore-aal.so";
	case AudioFactory::FMOD:
		return "libfragcore-afm.so";
	default:
		throw InvalidArgumentException("");
	}
#else
	assert(0);
#endif
}
