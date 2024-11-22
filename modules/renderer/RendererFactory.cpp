
#include <Core/IConfig.h>
#include <Core/Library.h>
#include <RendererFactory.h>

#include <cassert>
#include <cstdio>
#include <fmt/core.h>

using namespace fragcore;
using pcreateinternalrendering = IRenderer *(*)(IConfig *);

IRenderer *RenderingFactory::createRendering(RenderingFactory::RenderingAPI renderingapi, IConfig *config) {
	return RenderingFactory::createRendering(getInterfaceLibraryPath(renderingapi), config);
}

IRenderer *RenderingFactory::createRendering(const char *cpathlib, IConfig *config) {
	Library library;
	IRenderer *interface = nullptr;
	const char *funcsymbol = "createInternalRenderer";
	pcreateinternalrendering pfunc = nullptr;

	/*	Validate parameters.	*/
	if (cpathlib == nullptr) {
		throw InvalidArgumentException("path variable must not be null.");
	}

	/*	Open library and validate.	*/
	library.open(cpathlib);
	if (library.isValid()) {

		/*	Get factory function for creating rendering instance.	*/
		pfunc = reinterpret_cast<pcreateinternalrendering>(library.getfunc(funcsymbol));
		interface = pfunc(config);

	} else {
		throw RuntimeException("Failed loading {} library for creating renderer.", cpathlib);
	}

	return interface;
}

const char *RenderingFactory::getInterfaceLibraryPath(RenderingFactory::RenderingAPI api) {
	// TODO append api value to the exception string.
#ifdef FV_UNIX
	switch (api) {
	case RenderingFactory::OpenGL:
		return "libfragcore-rgl.so";
	case RenderingFactory::Vulkan:
		return "libfragcore-rvk.so";
	default:
		throw InvalidArgumentException("Not a valid rendering API enumerator.");
	}
#elif defined(FV_WINDOWS)
	switch (api) {
	case RenderingFactory::eOpenGL:
		return "libfragcore-rgl.dll";
	case RenderingFactory::eVulkan:
		return "libfragcore-rvk.dll";
	default:
		throw InvalidArgumentException("Not a valid rendering API enumerator.");
	}
#endif
}