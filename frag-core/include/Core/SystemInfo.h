/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_SYSTEMINFO_H_
#define _FRAG_CORE_SYSTEMINFO_H_ 1
#include "../FragDef.h"
#include "IO/IO.h"
#include "Ref.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC SystemInfo {
	  public:
		enum class OperatingSystem {
			Unix = 0x1,				/*	*/
			Linux = (0x2 | Unix),	/*	*/
			Window = 0x4,			/*	*/
			FreeBSD = (0x8 | Unix), /*	*/
			Mac = (0x10 | Unix),	/*	*/
			Android = 0x20,			/*	*/
			IOS = 0x40,				/*	*/
			Unknown = (1 << 31)		/*  */
		};

		enum class CPUArchicture {
			UnKnown,
			x86,   /**/
			AMD64, /**/
			Arm32, /**/
			Arm64  /**/
		};

		enum class SIMD : unsigned int {
			NONE,	/*	No SIMD exention flag.	*/
			MMX,	/*	MMX. (Yet not supported)	*/
			S3DNOW, /*	3DNOW. (Yet not supported)	*/
			SSE,	/*	SSE (Streaming SIMD Extensions).	*/
			SSE2,	/*	SSE2 (Streaming SIMD Extensions).	*/
			SSE3,	/*	SSE3 (Streaming SIMD Extensions).	*/
			SSSE3,	/*	SSSE3 (Streaming SIMD Extensions).	*/
			SSE4_1, /*	SSE4.1 (Streaming SIMD Extensions).	*/
			SSE4_2, /*	SSE4.2 (Streaming SIMD Extensions).	*/
			AVX,	/*	AVX Version 1 (Advanced Vector Extension).	*/
			AVX2,	/*	AVX Version 2 (Advanced Vector Extension).	(Not tested)*/
			AVX512, /*	AVX512 (Advanced Vector Extension). (Yet not supported)	*/
			NEON,	/*	ARM	FPU (floating-point unit) feature.	*/
		};

		/*	The kernel of the Operating system.	*/
		enum class KernelSystem {
			UnKnownKernel, /*		*/
			WindowsNT,	   /*		*/
			LinuxGNU,	   /*		*/
			Darwin,		   /*		*/
		};

		enum class Endianness {
			UnKnown,
			LittleEndian, /*	*/
			BigEndian,	  /*	*/
		};

		static KernelSystem getSystemKernel() noexcept;
		static OperatingSystem getOperatingSystem() noexcept;
		static std::string getOperatingSystemName() noexcept;
		static std::string getOperatingSystemName(OperatingSystem operatingSystem);

	  public: /*	System CPU Information.	*/
		// TODO add support for NUMA.
		static std::string getCPUName() noexcept;
		static CPUArchicture getCPUArchitecture2() noexcept;
		static const char *getCPUArchitecture() noexcept;
		static unsigned long getCPUFrequency() noexcept;
		static bool isSupportedInstruction(SIMD instruction) noexcept;
		static std::vector<SIMD> getSupportedSIMD();

		// TODO add package and physical cores.
		typedef struct cpu_package_t {
			uint32_t physical_cores;
			uint32_t logical_cores;
		} CPUPackage;

		/**
		 * @brief
		 *
		 * @return std::vector<CPUPackage>
		 */
		static std::vector<CPUPackage> getCPUPackages();

		/**
		 * @brief
		 *
		 * @return unsigned int
		 */
		static unsigned int getCPUCoreCount();

		/**
		 * @brief
		 *
		 * @return unsigned int
		 */
		static unsigned int getCPUCacheLine(size_t level = 2);

		/**
		 * @brief Get the Endianness object
		 *
		 * @return Endianness
		 */
		static Endianness getEndianness() noexcept;

	  public: /*	GPU Information	*/
		typedef struct gpu_information_t {
			std::string name;
			size_t memorySize;
		} GPUInformation;

		static std::vector<GPUInformation> getGPUDevices() noexcept;

	  public: /*	System Memory Information.	*/
		/**
		 * @brief
		 *
		 * @return unsigned long int
		 */
		static unsigned long int systemMemorySize();

		/**
		 * @brief Get the Page Size object
		 *
		 * @return unsigned int
		 */
		static unsigned int getPageSize();

	  public: /*	User Space Information.	*/
		/**
		 * @brief Get the Appliation Name object
		 *
		 * @return const char*
		 */
		static std::string getApplicationName();

		/**
		 * @brief Get the name of the current user.
		 *
		 * @return const char*
		 */
		static const char *getUserName();

		/**
		 * @brief Get the Current Directory object
		 *
		 * @return const char*
		 */
		static std::string getCurrentDirectory();

		/**
		 * @brief
		 *
		 * @return true
		 * @return false
		 */
		static bool supportsVibration();

		// TODO relocate
		static Ref<IO> &getStdOut() noexcept;
		static Ref<IO> &getStdIn() noexcept;
		static Ref<IO> &getStdErr() noexcept;

	  private:
	};
} // namespace fragcore

#endif
