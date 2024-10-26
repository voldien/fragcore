#include "Core/SystemInfo.h"
#include "IO/FileIO.h"
#include <cstdlib>
#include <filesystem>
#include <infoware.hpp>
#include <magic_enum.hpp>
#if defined(PLATFORM_POSIX) || defined(__linux__) // check defines for your setup
#include <pwd.h>
#endif

using namespace fragcore;

static iware::system::OS_info_t info;

SystemInfo::KernelSystem SystemInfo::getSystemKernel() noexcept {
	switch (iware::system::kernel_info().variant) {
	case iware::system::kernel_t::darwin:
		return SystemInfo::KernelSystem::Darwin;
	case iware::system::kernel_t::linux:
		return SystemInfo::KernelSystem::LinuxGNU;
	case iware::system::kernel_t::windows_nt:
		return SystemInfo::KernelSystem::WindowsNT;
	default:
		return SystemInfo::KernelSystem::UnKnownKernel;
	}
}

SystemInfo::OperatingSystem SystemInfo::getOperatingSystem() noexcept {

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

std::string SystemInfo::getOperatingSystemName() noexcept {
	if (info.major == 0) {
		info = iware::system::OS_info();
	}
	return info.full_name;
}

std::string SystemInfo::getOperatingSystemName(SystemInfo::OperatingSystem operatingSystem) {
	return std::string(magic_enum::enum_name(operatingSystem));
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
	case SystemInfo::SIMD::S3DNOW:
		_instruction = iware::cpu::instruction_set_t::s3d_now;
		break;
	case SystemInfo::SIMD::SSE:
		_instruction = iware::cpu::instruction_set_t::sse;
		break;
	case SystemInfo::SIMD::SSE2:
		_instruction = iware::cpu::instruction_set_t::sse2;
		break;
	case SystemInfo::SIMD::SSE3:
		_instruction = iware::cpu::instruction_set_t::sse3;
		break;
	case SystemInfo::SIMD::SSSE3:
		_instruction = iware::cpu::instruction_set_t::ssse3;
		break;
	case SystemInfo::SIMD::SSE4_1:
		_instruction = iware::cpu::instruction_set_t::sse41;
		break;
	case SystemInfo::SIMD::SSE4_2:
		_instruction = iware::cpu::instruction_set_t::sse42;
		break;
	case SystemInfo::SIMD::AVX:
		_instruction = iware::cpu::instruction_set_t::avx;
		break;
	case SystemInfo::SIMD::AVX2:
		_instruction = iware::cpu::instruction_set_t::avx2;
		break;
	case SystemInfo::SIMD::AVX512:
		_instruction = iware::cpu::instruction_set_t::avx_512;
		break;
	case SystemInfo::SIMD::NEON:
		_instruction = iware::cpu::instruction_set_t::neon;
		break;
	default:
		return false;
	}
	return iware::cpu::instruction_set_supported(_instruction);
}

std::vector<SystemInfo::SIMD> SystemInfo::getSupportedSIMD() {
	std::vector<SystemInfo::SIMD> supportedSIMDS;

	std::vector<iware::cpu::instruction_set_t> supported = iware::cpu::supported_instruction_sets();

	for (size_t i = 0; i < supported.size(); i++) {
		switch (supported[i]) {
		case iware::cpu::instruction_set_t::s3d_now:
			supportedSIMDS.push_back(SystemInfo::SIMD::S3DNOW);
			break;
		case iware::cpu::instruction_set_t::mmx:
			supportedSIMDS.push_back(SystemInfo::SIMD::MMX);
			break;
		case iware::cpu::instruction_set_t::sse:
			supportedSIMDS.push_back(SystemInfo::SIMD::SSE);
			break;
		case iware::cpu::instruction_set_t::sse2:
			supportedSIMDS.push_back(SystemInfo::SIMD::SSE2);
			break;
		case iware::cpu::instruction_set_t::sse3:
			supportedSIMDS.push_back(SystemInfo::SIMD::SSE3);
			break;
		case iware::cpu::instruction_set_t::ssse3:
			supportedSIMDS.push_back(SystemInfo::SIMD::SSSE3);
			break;
		case iware::cpu::instruction_set_t::sse41:
			supportedSIMDS.push_back(SystemInfo::SIMD::SSE4_1);
			break;
		case iware::cpu::instruction_set_t::sse42:
			supportedSIMDS.push_back(SystemInfo::SIMD::SSE4_2);
			break;
		case iware::cpu::instruction_set_t::avx:
			supportedSIMDS.push_back(SystemInfo::SIMD::AVX);
			break;
		case iware::cpu::instruction_set_t::avx2:
			supportedSIMDS.push_back(SystemInfo::SIMD::AVX2);
			break;
		case iware::cpu::instruction_set_t::avx_512:
			supportedSIMDS.push_back(SystemInfo::SIMD::AVX512);
			break;
		case iware::cpu::instruction_set_t::neon:
			supportedSIMDS.push_back(SystemInfo::SIMD::NEON);
		default:
			break;
		}
	}

	return supportedSIMDS;
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
		gpuDevices[i].max_frequency = devices[i].max_frequency;
	}

	return gpuDevices;
}

std::string SystemInfo::getApplicationName() {
#if defined(PLATFORM_POSIX) || defined(__linux__) // check defines for your setup

	std::string app_name;
	std::ifstream("/proc/self/comm") >> app_name;
	return app_name;

#elif defined(_WIN32)

	char buf[MAX_PATH];
	GetModuleFileNameA(nullptr, buf, MAX_PATH);
	return buf;

#else
	return std::string();
#endif
}

const char *SystemInfo::getUserName() {
#if defined(PLATFORM_POSIX) || defined(__linux__) // check defines for your setup
	uid_t uid = geteuid();
	struct passwd *passw = getpwuid(uid);
	if (passw) {
		return passw->pw_name;
	}
#else

#endif
	return "";
}

unsigned int SystemInfo::getPageSize() {
#if defined(PLATFORM_POSIX) || defined(__linux__) // check defines for your setup
	long int pagesize = sysconf(_SC_PAGE_SIZE);
	return pagesize;
#else
	return 1024;
#endif
}

std::vector<SystemInfo::CPUPackage> SystemInfo::getCPUPackages() {
	std::vector<SystemInfo::CPUPackage> packages(iware::cpu::quantities().packages);
	return packages;
}

unsigned int SystemInfo::getCPUCoreCount() { return iware::cpu::quantities().logical; }

unsigned int SystemInfo::getCPUCacheLine(const size_t level) {
	const auto cache = iware::cpu::cache(level);
	return cache.line_size;
}

unsigned long int SystemInfo::systemMemorySize() noexcept {
	const auto memory = iware::system::memory();
	return memory.physical_total;
}

std::string SystemInfo::getCurrentDirectory() { return std::filesystem::current_path(); }

Ref<IO> &SystemInfo::getStdOut() {
	static Ref<IO> stdoutRef = Ref<IO>(new FileIO(stdout));
	return stdoutRef;
}
Ref<IO> &SystemInfo::getStdIn() {
	static Ref<IO> stdinRef = Ref<IO>(new FileIO(stdin));
	return stdinRef;
}
Ref<IO> &SystemInfo::getStdErr() {
	static Ref<IO> stderrRef = Ref<IO>(new FileIO(stderr));
	return stderrRef;
}
