#include "Core/SystemInfo.h"
#include "Core/IO/FileIO.h"
#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"
#include <HpmCpp.h>
#include <SDL2/SDL_cpuinfo.h>
#include <SDL2/SDL_platform.h>
#include <filesystem>

//namespace fs = std::filesystem;
using namespace fragcore;

SystemInfo::OperatingSystem SystemInfo::getOperatingSystem(void) {
	/*  */
	const char *os = SDL_GetPlatform();

	/*  */
	if (strcmp(os, "Linux") == 0)
		return SystemInfo::Linux;
	if (strcmp(os, "Windows") == 0)
		return SystemInfo::Window;
	if (strcmp(os, "Android") == 0)
		return SystemInfo::Android;
	if (strcmp(os, "unix") == 0)
		return SystemInfo::Unix;
	if (strcmp(os, "iOS") == 0)
		return SystemInfo::IOS;
	if (strcmp(os, "Mac OS X") == 0)
		return SystemInfo::Mac;

	return SystemInfo::Unknown;
}

const char *SystemInfo::getOperatingSystemName(SystemInfo::OperatingSystem os) {

	/*  */
	switch (os) {
	case SystemInfo::Linux:
		return "Linux";
	case SystemInfo::Window:
		return "Window";
	case SystemInfo::Unix:
		return "Unix";
	case SystemInfo::Android:
		return "Android";
	case SystemInfo::Mac:
		return "Mac";
	case SystemInfo::IOS:
		return "IOS";
	case SystemInfo::FreeBSD:
		return "FreeBSD";
	default:
		throw InvalidArgumentException("Invalid Operating System Enumerator");
	}
}

SystemInfo::SIMD SystemInfo::getSupportedSIMD(void) {
	unsigned int supportedSIMD = HPM_NONE;

	for (int i = 1; i < 11; i++) {
		if (hpm_support_cpu_feat(1 << i))
			supportedSIMD |= (1 << i);
	}
	return (SystemInfo::SIMD)supportedSIMD;
}

const char *SystemInfo::getAppliationName(void) {
	return "";
	/*fs::current_path().c_str();*/
}

const char *SystemInfo::getUserName(void) { return ""; }

unsigned int SystemInfo::getPageSize(void) { return 1024; }

unsigned int SystemInfo::getCPUCoreCount(void) { return SDL_GetCPUCount(); }

unsigned int SystemInfo::getCPUCacheLine(void) { return SDL_GetCPUCacheLineSize(); }

bool SystemInfo::supportsVibration(void) { return false; }

unsigned long int SystemInfo::systemMemorySize(void) { return SDL_GetSystemRAM(); }

const char *SystemInfo::getCurrentDirectory(void) {
	return "";
	/*fs::current_path().c_str();*/
}

// TODO relocate to system or something, since it they are always exist more of the time.
static Ref<IO> stdoutRef = Ref<IO>(new FileIO(stdout));
static Ref<IO> stdinRef = Ref<IO>(new FileIO(stdin));
static Ref<IO> stderrRef = Ref<IO>(new FileIO(stderr));

Ref<IO> &SystemInfo::getStdOut(void) {
	return stdoutRef;
	// Ref<IO>(stdoutIO);
}
Ref<IO> &SystemInfo::getStdIn(void) {
	return stdoutRef;
	// Ref<IO>(stdinIO);
}
Ref<IO> &SystemInfo::getStdErr(void) {
	return stdoutRef;
	// Ref<IO>(stderrIO);
}