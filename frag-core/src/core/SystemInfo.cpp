#include "Core/SystemInfo.h"
#include "Core/IO/FileIO.h"
#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"
#include <Hpm.hpp>
#include <infoware.hpp>

using namespace fragcore;
static iware::system::OS_info_t info;

SystemInfo::KernelSystem SystemInfo::getSystemKernel(void) noexcept {
	switch (iware::system::kernel_info().variant) {
	default:
		return SystemInfo::UnKnownKernel;
	}
	// iware::system::OS_
}

SystemInfo::OperatingSystem SystemInfo::getOperatingSystem(void) noexcept {

	// iware::system::OS_info().full_name
	if (info.major == 0)
		info = iware::system::OS_info();

	/*  */
	if (strcmp(info.name.c_str(), "Linux") == 0)
		return SystemInfo::Linux;
	if (strcmp(info.name.c_str(), "Windows") == 0)
		return SystemInfo::Window;
	if (strcmp(info.name.c_str(), "Android") == 0)
		return SystemInfo::Android;
	if (strcmp(info.name.c_str(), "unix") == 0)
		return SystemInfo::Unix;
	if (strcmp(info.name.c_str(), "iOS") == 0)
		return SystemInfo::IOS;
	if (strcmp(info.name.c_str(), "Mac OS X") == 0)
		return SystemInfo::Mac;

	return SystemInfo::Unknown;
}

const std::string &SystemInfo::getOperatingSystemName(void) noexcept {
	if (info.major == 0)
		info = iware::system::OS_info();
	return info.full_name;
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

const char *SystemInfo::getCPUName(void) noexcept { return iware::cpu::model_name().c_str(); }

const char *SystemInfo::getCPUArchitecture(void) noexcept {
	switch (iware::cpu::architecture()) {
	case iware::cpu::architecture_t::x64:
		return "x64";
	case iware::cpu::architecture_t::arm:
		return "ARM";
	case iware::cpu::architecture_t::itanium:
		return "Itanium";
	case iware::cpu::architecture_t::x86:
		return "X86";
	default:
		return "Unknown";
	}
}

unsigned long SystemInfo::getCPUFrequence(void) noexcept { return iware::cpu::frequency(); }

SystemInfo::SIMD SystemInfo::getSupportedSIMD(void) {
	unsigned int supportedSIMD = HPM_NONE;

	/**/
	// iware::cpu::instruction_set_supported()

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

unsigned int SystemInfo::getCPUCoreCount(void) { return iware::cpu::quantities().logical; }

unsigned int SystemInfo::getCPUCacheLine(void) {
	const auto cache = iware::cpu::cache(2);
	return cache.line_size;
}

bool SystemInfo::supportsVibration(void) { return false; }

unsigned long int SystemInfo::systemMemorySize(void) {
	const auto memory = iware::system::memory();
	return memory.physical_available;
}

const char *SystemInfo::getCurrentDirectory(void) {
	return "";
	/*fs::current_path().c_str();*/
}

// TODO relocate to system or something, since it they are always exist more of the time.
static Ref<IO> stdoutRef = Ref<IO>(new FileIO(stdout));
static Ref<IO> stdinRef = Ref<IO>(new FileIO(stdin));
static Ref<IO> stderrRef = Ref<IO>(new FileIO(stderr));

Ref<IO> &SystemInfo::getStdOut(void) noexcept { return stdoutRef; }
Ref<IO> &SystemInfo::getStdIn(void) noexcept { return stdoutRef; }
Ref<IO> &SystemInfo::getStdErr(void) noexcept { return stdoutRef; }