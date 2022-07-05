#include "Core/SystemInfo.h"
#include "Core/IO/FileIO.h"
#include <filesystem>
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
	if (info.major == 0) {
		info = iware::system::OS_info();
	}
	/*  */
	if (strcmp(info.name.c_str(), "Linux") == 0) {
		return SystemInfo::OperatingSystem::Linux;
	}
	if (strcmp(info.name.c_str(), "Windows") == 0) {
		return SystemInfo::OperatingSystem::Window;
	}
	if (strcmp(info.name.c_str(), "Android") == 0) {
		return SystemInfo::OperatingSystem::Android;
	}
	if (strcmp(info.name.c_str(), "unix") == 0) {
		return SystemInfo::OperatingSystem::Unix;
	}
	if (strcmp(info.name.c_str(), "iOS") == 0) {
		return SystemInfo::OperatingSystem::IOS;
	}
	if (strcmp(info.name.c_str(), "Mac OS X") == 0) {
		return SystemInfo::OperatingSystem::Mac;
	}

	return SystemInfo::OperatingSystem::Unknown;
}

const std::string &SystemInfo::getOperatingSystemName() noexcept {
	if (info.major == 0) {
		info = iware::system::OS_info();
	}
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
	case SystemInfo::OperatingSystem::Unknown:
	default:
		return "Unkown Operating System";
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

unsigned long SystemInfo::getCPUFrequency() noexcept { return iware::cpu::frequency(); }

bool SystemInfo::isSupportedInstruction(SIMD instruction) noexcept {
	iware::cpu::instruction_set_t _instruction = iware::cpu::instruction_set_t::mmx;
	switch (instruction) {
	case SystemInfo::SIMD::MMX:
		_instruction = iware::cpu::instruction_set_t::mmx;
		break;

	default:
		break;
	}
	return iware::cpu::instruction_set_supported(_instruction);
}

SystemInfo::SIMD SystemInfo::getSupportedSIMD() {
	unsigned int supportedSIMD = 0;

	iware::cpu::instruction_set_supported(iware::cpu::instruction_set_t::adx);

	return (SystemInfo::SIMD)supportedSIMD;
}

SystemInfo::Endianness SystemInfo::getEndianness() noexcept {
	switch (iware::cpu::endianness()) {
	case iware::cpu::endianness_t::big:
		return SystemInfo::Endianness::BigEndian;
	case iware::cpu::endianness_t::little:
		return SystemInfo::Endianness::LittleEndian;
	default: // unknown.
		return SystemInfo::Endianness::UnKnown;
	}
}

std::vector<SystemInfo::GPUInformation> SystemInfo::getGPUDevices() noexcept {
	std::vector<iware::gpu::device_properties_t> devices = iware::gpu::device_properties();
	std::vector<GPUInformation> gpuDevices(devices.size());

	for (size_t i = 0; i < devices.size(); i++) {
		gpuDevices[i].name = devices[i].name;
		gpuDevices[i].memorySize = devices[i].memory_size;
	}
	return gpuDevices;
}

std::string SystemInfo::getAppliationName() {
#if defined(PLATFORM_POSIX) || defined(__linux__) // check defines for your setup

	std::string sp;
	std::ifstream("/proc/self/comm") >> sp;
	return sp;

#elif defined(_WIN32)

	char buf[MAX_PATH];
	GetModuleFileNameA(nullptr, buf, MAX_PATH);
	return buf;

#else
	return std::string();
#endif
}

const char *SystemInfo::getUserName() { return ""; }

unsigned int SystemInfo::getPageSize() { return 1024; }

std::vector<SystemInfo::CPUPackage> SystemInfo::getCPUPackages() {
	std::vector<SystemInfo::CPUPackage> packages(iware::cpu::quantities().packages);

	return packages;
}

unsigned int SystemInfo::getCPUCoreCount() { return iware::cpu::quantities().logical; }

unsigned int SystemInfo::getCPUCacheLine(size_t level) {
	const auto cache = iware::cpu::cache(level);
	return cache.line_size;
}

bool SystemInfo::supportsVibration() { return false; }

unsigned long int SystemInfo::systemMemorySize() {
	const auto memory = iware::system::memory();
	return memory.physical_total;
}

std::string SystemInfo::getCurrentDirectory() { return std::filesystem::current_path(); }

// TODO relocate to system or something, since it they are always exist more of the time.

Ref<IO> &SystemInfo::getStdOut() noexcept {
	static Ref<IO> stdoutRef = Ref<IO>(new FileIO(stdout));
	return stdoutRef;
}
Ref<IO> &SystemInfo::getStdIn() noexcept {
	static Ref<IO> stdinRef = Ref<IO>(new FileIO(stdin));
	return stdinRef;
}
Ref<IO> &SystemInfo::getStdErr() noexcept {
	static Ref<IO> stderrRef = Ref<IO>(new FileIO(stderr));
	return stderrRef;
}
