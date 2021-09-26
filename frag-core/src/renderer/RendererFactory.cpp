
//#include "Exception/RuntimeException.h"
#include <Core/IConfig.h>
#include <Core/Library.h>
#include <Renderer/RendererFactory.h>

#include <cassert>
#include <cstdio>
#include <exception>
#include <fmt/core.h>
#include <stdexcept>

using namespace fragcore;
typedef IRenderer *(*pcreateinternalrendering)(IConfig *config);

IRenderer *RenderingFactory::createRendering(RenderingFactory::RenderingAPI renderingapi, IConfig *config) {
	return RenderingFactory::createRendering(getInterfaceLibraryPath(renderingapi), config);
}

IRenderer *RenderingFactory::createRendering(const char *cpathlib, IConfig *config) noexcept {
	Library library;
	IRenderer *interface = nullptr;
	const char *funcsymbol = "createInternalRenderer";
	pcreateinternalrendering pfunc;

	/*	Validate parameters.	*/
	if (cpathlib == nullptr)
		throw InvalidArgumentException("path variable must not be null.");

	/*	Open library and validate.	*/
	library.open(cpathlib);
	if (library.isValid()) {

		/*	Get factory function for creating rendering instance.	*/
		pfunc = (pcreateinternalrendering)library.getfunc(funcsymbol);
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
	case RenderingAPI::DirectX:
		throw InvalidArgumentException("Not supported on Unix Systems.");
	case RenderingAPI::OpenCL:
		return "libfragcore-rcl.so";
	default:
		throw InvalidArgumentException("Not a valid rendering API enumerator.");
	}
#elif defined(FV_WINDOWS)
	switch (api) {
	case RenderingFactory::eOpenGL:
		return "libfragcore-rgl.dll";
	case RenderingFactory::eVulkan:
		return "libfragcore-rvk.dll";
	case RenderingAPI::eDirectX:
		return "libfragcore-rdx.dll";
	case RenderingAPI::eOpenCL:
		return "libfragcore-rcl.dll";
	default:
		throw InvalidArgumentException("Not a valid rendering API enumerator.");
	}
#endif
}