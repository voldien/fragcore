/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAG_CORE_FRAG_DEF_H_
#define _FRAG_CORE_FRAG_DEF_H_ 1

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <cerrno>

#include <Exception.hpp>

namespace fragcore {

	using RuntimeException = cxxexcept::RuntimeException;
	using DivideByZeroException = cxxexcept::DivideByZeroException ;
	using PermissionDeniedException = cxxexcept::PermissionDeniedException;
	using IOException = cxxexcept::IOException;
	using NotImplementedException = cxxexcept::NotImplementedException ;
	using InvalidArgumentException = cxxexcept::InvalidArgumentException ;
	using NotSupportedException = cxxexcept::NotSupportedException ;
	using InvalidPointerException = cxxexcept::InvalidPointerException ;
	using SystemException = cxxexcept::SystemException;
	using IndexOutOfRangeException = cxxexcept::IndexOutOfRangeException;
}

/**
 *	Compiler version macro.
 */
#define FV_COMPILER_VERSION(major, minor, revision, state) FV_STR(major)FV_TEXT(".")FV_STR(minor)FV_TEXT(".")FV_STR(revision)
#define FV_STR_VERSION(major, minor, revision) FV_COMPILER_VERSION(major, minor, revision, "")

/**
 *    Compiler
 */
#ifdef _MSC_VER 	/*	Visual Studio C++ Compiler.	*/
	#define FV_VC
	
	#define FV_COMPILER 1
	#if _MSC_VER >= 1900
		#define FV_V13 _MSC_VER
	#elif _MSC_VER >= 1800
		#define FV_V12 _MSC_VER
	#elif _MSC_VER >= 1700
		#define FV_VC11 _MSC_VER
	#elif _MSC_VER >= 1600
		#define FV_VC10 _MSC_VER
	#elif _MSC_VER >= 1500
		#define FV_VC9 _MSC_VER
	#elif _MSC_VER >= 1400
		#define FV_VC8 _MSC_VER
	#elif _MSC_VER >= 1300
		#define FV_VC7 _MSC_VER
	#else
		#define FV_VC6 _MSC_VER
	#endif
    	#pragma warning(disable : 4201)
	#define FV_COMPILER_NAME "Visual Studio C++/C"

#elif defined(__clang__)  || defined(__llvm__)           /*  LLVM, clang   */
#define FV_LLVM 1
	#define FV_CLANG 1
	#define FV_COMPILER 5
	#define FV_COMPILER_NAME "LLVM/CLANG"
	#define FV_COMPILER_MAJOR_VERSION __clang_major__
	#define FV_COMPILER_MINOR_VERSION __clang_minor__

#elif defined(__GNUC__) || defined(__SNC__) || defined( __GNUC_MINOR__)	/*  GNU C Compiler*/
#define FV_GNUC 1
#define FV_COMPILER 2
#define FV_COMPILER_NAME "GNU C"
#define FV_COMPILER_MAJOR_VERSION __clang_major__
#define FV_COMPILER_MINOR_VERSION __clang_minor__

#elif defined(__GNUG__) /*  GNU C++ Compiler*/
#define FV_GNUC 2

#elif defined(__ghs__)		/* GHS	*/
	#define FV_GHS 1
	#define FV_COMPILER 3

#elif defined(__HP_cc) || defined(__HP_aCC)			/*	*/

#elif defined(__PGI)			/*	*/

#elif defined(__ICC) || defined(__INTEL_COMPILER) /*  Intel Compiler  */
	#define FV_INTEL
	#define FV_COMPILER 4
	#define FV_COMPILER_NAME "Intel C++"

#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)

#else
	#error Unsupported Compiler.
#endif


#if defined(__GNUC__) && defined(__ARM_NEON__)
/* GCC-compatible compiler, targeting ARM with NEON */
     #include <arm_neon.h>
#endif	/**/


/**
 *	Platform define
 *	Architecture!
 */
#ifdef FV_VC
#if defined(_M_IX86) || defined(_WIN32)
		#define FV_X86                          /**/
		#define FV_X32                          /**/
		#define FV_WIN32                        /**/
		#define FV_WINDOWS                      /**/
	#elif defined(_M_X64) || defined(_WIN64)
		#define FV_X64                          /**/
		#define FV_WIN64                        /**/
		#define FV_WINDOWS                      /**/
	#elif defined(_M_PPC)
		#define FV_PPC                          /**/
		#define FV_X360                         /**/
		#define FV_VMX                          /**/
	#elif defined(_M_ARM)
		#define FV_ARM                          /**/
		#define FV_ARM_NEON                     /**/
	#endif
#elif defined(FV_GNUC) || defined(FV_CLANG)
#ifdef __CELLOS_LV2__   /**/
#define FV_PS3                          /*	playstation 3*/
#elif defined(__arm__)	/**/
#define FV_ARM
        #define FV_PSP2                         /*	playstation portable 2*/
        #define FV_RAS_PI                       /*	rasberry pi	*/
#endif
#if defined(_WIN32) /**  Window*/
#define FV_X86
		#define FV_WINDOWS                      /**/
#endif
#if ( defined(__linux__) || defined(__linux) || defined(linux) ) && (!(__ANDROID__) || !(ANDROID))/* Linux */
#define FV_LINUX 1                       /**/
#if defined(__amd64) || defined(__x86_64__) || defined(__i386__)
#define FV_X86 1
#define FV_X86_64 1
#endif
#if defined(__arm__)
#define EX_ARM 1
#endif

#elif defined (ANDROID) || defined(__ANDROID__) || __ANDROID_API__ > 9  /** Android */
#include<jni.h>
		#define FV_ANDROID 1
		/*  android Architecture*/
        #if defined(__arm__)
			#define FV_ARM 1
		  #if defined(__ARM_ARCH_7A__)
			#if defined(__ARM_NEON__)
			  #if defined(__ARM_PCS_VFP)
				#define ABI "armeabi-v7a/NEON (hard-float)"
			  #else
				#define ABI "armeabi-v7a/NEON"
			  #endif
			#else
			  #if defined(__ARM_PCS_VFP)
				#define ABI "armeabi-v7a (hard-float)"
			  #else
				#define ABI "armeabi-v7a"
			  #endif
			#endif
		  #else
		   #define ABI "armeabi"
		  #endif
		#elif defined(__i386__)
		   #define ABI "x86"
		#elif defined(__x86_64__)
		   #define ABI "x86_64"
		#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
		   #define ABI "mips64"
		#elif defined(__mips__)
		   #define ABI "mips"
		#elif defined(__aarch64__)
		   #define ABI "arm64-v8a"
		#else
		   #define ABI "unknown"
		#endif

	#elif defined (__APPLE__)   /*  Apple product   */
		#define FV_APPLE 1
		#if defined(__arm__)
			#define FV_APPLE_IOS    /*  Apple iphone/ipad OS    */
		#elif defined(MACOSX) || defined(macintosh) || defined(Macintosh)
			#define EX_MAC 1
		#endif
	#elif defined(__CYGWIN) 	/**/
		#define FV_CYGWIN 1
		#define FV_LINUX 1
	#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)   /*  BSD*/
		#define FV_BSD
    	#elif defined(__llvm__) || defined(__clang__)   	/*  llvm    */
        	#define FV_LLVM 1
#endif

#elif defined(__ICC) || defined(__INTEL_COMPILER)


#else
	#error  Unsupported architecture!   /*  No architecture support implicitly. remove this line to compile anyway*/
#endif

#if defined(__native_client__)	/*	nacl google	*/
#define FV_NACL 1
#endif
#if defined(__pnacl__)          /* portable nacl google */
#define FV_PNACL 1
#endif
#if defined(__unix__) || defined(__unix) || defined(unix)	/*  Unix    */
#   define FV_UNIX 1
#endif

/**
 *	Calling function convention.
 */
#ifdef FV_WINDOWS	        /** Windows Calling Convention.*/
#define FVAPIENTRY     __cdecl
	#define FVAPIFASTENTRY __fastcall
	#define FVAPITHISENTRY __thiscall
	#define FVAPISTDENTRY  __stdcall
#elif defined(FV_ANDROID)   /** Android Calling Convention	*/
#define FVAPIENTRY JNICALL
    #define FVAPIFASTENTRY JNICALL
    #define FVAPITHISENTRY JNICALL
    #define FVAPISTDENTRY JNICALL
#else
#   if !defined(__cdecl) && ( defined(FV_GNUC)  || defined(FV_CLANG) )
#define __cdecl  __attribute__ ((__cdecl__))
#define __stdcall  __attribute__ ((stdcall))
#define __fastcall __attribute__((fastcall))
#   endif
#define FVAPIENTRY     __cdecl
#define FVAPISTDENTRY  __stdcall
#define FVAPIFASTENTRY __fastcall
#endif


/**
 *	restrict declaration.
 */
#ifndef FV_RESTRICT
#if defined(FV_GNUC)
#define FV_RESTRICT __restrict__
#elif defined(FV_VC) || defined(FV_CLANG)
#define FV_RESTRICT __restrict
#else
#define FV_RESTRICT __declspec(restrict)
#endif
#endif

/*
 *	force inline.
 */
#if defined(FV_MSVC)
#define FV_ALWAYS_INLINE __forceinline
#elif defined(FV_GNUC) || defined(FV_CLANG)
#define FV_ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(FV_GNUC) || defined(FV_GHS)
#define FV_ALWAYS_INLINE inline __attribute__((always_inline))
#else
	#pragma message("Warning: You'd need to add FV_ALWAYS_INLINE for this compiler.")
#endif


/**
 *	Alignment of data and vectors.
 */
#if defined(FV_GNUC) || defined(FV_CLANG)	// GNU & clang compiler
	#define FV_ALIGN(alignment) __attribute__ ((aligned(alignment)))
	#define FV_VECTORALIGN(alignment) __attribute__ ((__vector_size__ (alignment), __may_alias__))
	#define FV_VECTORALIGNI(alignment) __attribute__ ((__vector_size__ (alignment)))

	#define FV_ALIGNED16  FV_ALIGN(16)
	#define FV_ALIGNED64   FV_ALIGN(64)
	#define FV_ALIGNED128  FV_ALIGN(128)

#elif defined(FV_VC)	// Visual Compiler
	#define FV_ALIGN(alignment) __declspec(align(alignment))
	#define FV_VECTORALIGN(alignment) __declspec(align(alignment))
	#define FV_VECTORALIGNI(alignment) __declspec(align(alignment))

	#define FV_ALIGNED16  FV_ALIGN(16)
	#define FV_ALIGNED64   FV_ALIGN(64)
	#define FV_ALIGNED128  FV_ALIGN(128)
#else
	#pragma message("Warning: You'd need to add FV_ALIGN, FV_VECTORALIGN, FV_VECTORALIGNI for this compiler.")
#endif

/**
 *	library declaration.
 */
#if defined(FV_GNUC) || defined(FV_CLANG)
#if defined(FV_UNIX)
#define FVDECLSPEC	 __attribute__((__visibility__ ("default")))
#else
#define FVDECLSPEC
#endif
#elif defined(FV_VC)
#if FV_INTERNAL
		#define FVDECLSPEC __declspec(dllexport)
	#else
		#define FVDECLSPEC __declspec(dllimport)
	#endif
#endif

#if defined(FV_ARM) || defined(FV_I386)

#endif

/*	Optimized branch predictions.	*/
#if defined(FV_GCC) || defined(FV_CLANG)	// GCC or Clang compiler
	#define likely(x)      __builtin_expect(!!(x), 1)
	#define unlikely(x)    __builtin_expect(!!(x), 0)
#else
	#define likely(x)      x
	#define unlikely(x)    x
#endif


#if defined(FV_GCC) || defined(FV_CLANG)
	#define FV_PACKED( __Declaration__ ) __Declaration__ __attribute__ ((__packed__))
#elif defined(FV_VC)
	#define FV_PACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#else
	#define FV_PACKED( __Declaration__ ) __Declaration__
#endif

/**
 *	String macros.
 */
#define FV_STR_HELPER(x) #x                                         /*	String helper macro.*/
#define FV_STR(x) FV_STR_HELPER(x)                                  /*	Convert input to a double quoate string.	*/
#define FV_TEXT(quote) quote                                        /*	*/

//TODO relocate
#if FV_UNIX
	#include <unistd.h>
#endif

#ifdef __cplusplus /*	C++ Environment	*/
extern "C" {
#endif



#ifdef __cplusplus /*	C++ Environment	*/
}
#endif

#endif
