#include "Core/SystemInfo.h"
#include "Core/IO/FileIO.h"
#include <infoware.hpp>

using namespace fragcore;
static iware::system::OS_info_t info;

SystemInfo::KernelSystem SystemInfo::getSystemKernel() noexcept {
	switch (iware::system::kernel_info().variant) {
	default:
		return SystemInfo::KernelSystem::UnKnownKernel;
	}
	// iware::system::OS_
}

SystemInfo::OperatingSystem SystemInfo::getOperatingSystem() noexcept {

	// iware::system::OS_info().full_name
	if (info.major == 0)
		info = iware::system::OS_info();

	/*  */
	if (strcmp(info.name.c_str(), "Linux") == 0)
		return SystemInfo::OperatingSystem::Linux;
	if (strcmp(info.name.c_str(), "Windows") == 0)
		return SystemInfo::OperatingSystem::Window;
	if (strcmp(info.name.c_str(), "Android") == 0)
		return SystemInfo::OperatingSystem::Android;
	if (strcmp(info.name.c_str(), "unix") == 0)
		return SystemInfo::OperatingSystem::Unix;
	if (strcmp(info.name.c_str(), "iOS") == 0)
		return SystemInfo::OperatingSystem::IOS;
	if (strcmp(info.name.c_str(), "Mac OS X") == 0)
		return SystemInfo::OperatingSystem::Mac;

	return SystemInfo::OperatingSystem::Unknown;
}

const std::string &SystemInfo::getOperatingSystemName() noexcept {
	if (info.major == 0)
		info = iware::system::OS_info();
	return info.full_name;
}

const char *SystemInfo::getOperatingSystemName(SystemInfo::OperatingSystem os) {

	/*  */
	switch (os) {
	case SystemInfo::OperatingSystem::Linux:
		return "Linux";
	case SystemInfo::OperatingSystem::Window:
		return "Window";
	case SystemInfo::OperatingSystem::Unix:
		return "Unix";
	case SystemInfo::OperatingSystem::Android:
		return "Android";
	case SystemInfo::OperatingSystem::Mac:
		return "Mac";
	case SystemInfo::OperatingSystem::IOS:
		return "IOS";
	case SystemInfo::OperatingSystem::FreeBSD:
		return "FreeBSD";
	default:
		throw InvalidArgumentException("Invalid Operating System Enumerator");
	}
}

std::string SystemInfo::getCPUName() noexcept { return iware::cpu::model_name(); }

const char *SystemInfo::getCPUArchitecture() noexcept {
	switch (iware::cpu::architecture()) {
	case iware::cpu::architecture_t::x64:
		return "x64";
	case iware::cpu::architecture_t::arm:
		return "ARM";
	case iware::cpu::architecture_t::itanium:
		return "Itanium";
	case iware::cpu::architecture_t::x86:
		return "X86";
	case iware::cpu::architecture_t::unknown:
	default:
		return "Unknown";
	}
}

unsigned long SystemInfo::getCPUFrequence() noexcept { return iware::cpu::frequency(); }

SystemInfo::SIMD SystemInfo::getSupportedSIMD() {
	unsigned int supportedSIMD = 0;

	/**/
	// iware::cpu::instruction_set_supported()
	iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::adx);

	// for (int i = 1; i < 11; i++) {
	// 	if (hpm_support_cpu_feat(1 << i))
	// 		supportedSIMD |= (1 << i);
	// }
	return (SystemInfo::SIMD)supportedSIMD;
}

SystemInfo::Endianness getEndianness() {
	switch (iware::cpu::endianness()) {
	case iware::cpu::endianness_t::big:
		return SystemInfo::Endianness::BigEndian;
	case iware::cpu::endianness_t::little:
		return SystemInfo::Endianness::LittleEndian;
	default:
		assert(0);
		throw RuntimeException();
	}
}

const char *SystemInfo::getAppliationName() {
	return "";
	/*fs::current_path().c_str();*/
}

const char *SystemInfo::getUserName() { return ""; }

unsigned int SystemInfo::getPageSize() { return 1024; }

unsigned int SystemInfo::getCPUCoreCount() { return iware::cpu::quantities().logical; }

unsigned int SystemInfo::getCPUCacheLine() {
	const auto cache = iware::cpu::cache(2);
	return cache.line_size;
}

bool SystemInfo::supportsVibration() { return false; }

unsigned long int SystemInfo::systemMemorySize() {
	const auto memory = iware::system::memory();
	return memory.physical_available;
}

const char *SystemInfo::getCurrentDirectory() {
	return "";
	/*fs::current_path().c_str();*/
}

// TODO relocate to system or something, since it they are always exist more of the time.
static Ref<IO> stdoutRef = Ref<IO>(new FileIO(stdout));
static Ref<IO> stdinRef = Ref<IO>(new FileIO(stdin));
static Ref<IO> stderrRef = Ref<IO>(new FileIO(stderr));

Ref<IO> &SystemInfo::getStdOut() noexcept { return stdoutRef; }
Ref<IO> &SystemInfo::getStdIn() noexcept { return stdoutRef; }
Ref<IO> &SystemInfo::getStdErr() noexcept { return stdoutRef; }