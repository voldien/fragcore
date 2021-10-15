
/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_SYSTEMINFO_H_
#define _FRAG_CORE_SYSTEMINFO_H_ 1
#include "../Def.h"
#include "IO/IO.h"
#include "Ref.h"
namespace fragcore {

	/**
	 * @brief 
	 * 
	 */
	class FVDECLSPEC SystemInfo {
	  public:
		enum OperatingSystem {
			Unix = 0x1,				/*	*/
			Linux = (0x2 | Unix),	/*	*/
			Window = 0x4,			/*	*/
			FreeBSD = (0x8 | Unix), /*	*/
			Mac = (0x10 | Unix),	/*	*/
			Android = 0x20,			/*	*/
			IOS = 0x40,				/*	*/
			Unknown = (1 << 31)		/*  */
		};

		enum SIMD {
			NONE = (0 << 0),	/*	No HPM SIMD exention flag.	*/
			MMX = (1 << 1),		/*	MMX. (Yet not supported)	*/
			SSE = (1 << 2),		/*	SSE (Streaming SIMD Extensions).	*/
			SSE2 = (1 << 3),	/*	SSE2 (Streaming SIMD Extensions).	*/
			SSE3 = (1 << 4),	/*	SSE3 (Streaming SIMD Extensions).	*/
			SSSE3 = (1 << 5),	/*	SSSE3 (Streaming SIMD Extensions).	*/
			SSE4_1 = (1 << 6),	/*	SSE4.1 (Streaming SIMD Extensions).	*/
			SSE4_2 = (1 << 7),	/*	SSE4.2 (Streaming SIMD Extensions).	*/
			AVX = (1 << 8),		/*	AVX Version 1 (Advanced Vector Extension).	*/
			AVX2 = (1 << 9),	/*	AVX Version 2 (Advanced Vector Extension).	(Not tested)*/
			AVX512 = (1 << 10), /*	AVX512 (Advanced Vector Extension). (Yet not supported)	*/
			NEON = (1 << 11),	/*	ARM	FPU (floating-point unit) feature.	*/
		};

		enum KernelSystem {
			UnKnownKernel,
			WindowsNT,
			LinuxGNU,
			Darwin,
		};

		enum class Endianness {
			LittleEndian,
			BigEndian,
		};

		static KernelSystem getSystemKernel() noexcept;
		static OperatingSystem getOperatingSystem() noexcept;
		static const std::string &getOperatingSystemName() noexcept;
		static const char *getOperatingSystemName(OperatingSystem os);

		static const char *getCPUName() noexcept;
		static const char *getCPUArchitecture() noexcept;
		static unsigned long getCPUFrequence() noexcept;
		static bool isSupportedInstruction() noexcept;
		static SIMD getSupportedSIMD();

		static Endianness getEndianness() noexcept;

		static unsigned long int systemMemorySize();
		static unsigned int getPageSize();

		static unsigned int getCPUCoreCount();

		static unsigned int getCPUCacheLine();

		static const char *getAppliationName();

		static const char *getUserName();

		static bool supportsVibration();

		static const char *getCurrentDirectory();

		// TODO relocate
		static Ref<IO> &getStdOut() noexcept;
		static Ref<IO> &getStdIn() noexcept;
		static Ref<IO> &getStdErr() noexcept;

	  private:
	};
} // namespace fragcore

#endif
