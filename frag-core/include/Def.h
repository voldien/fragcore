/**
    FrameView for rendering shaders in screen space.
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
#ifndef _FRAG_CORE_DEF_H_
#define _FRAG_CORE_DEF_H_ 1
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<cassert>
#include<errno.h>

/*	Vector data types for rendering geometries.	*/
#include<HpmCpp.h>
#include<HCPlane.h>
#include<HCVector2.h>
#include<HCVector4.h>
#include<HCVector3.h>
#include<HCQuaternion.h>
#include<HpmCpp.h>

#define FV_NAME ""
#if defined(FRAG_CORE_INTERNAL_IMP)
#include<zlib.h>
#else

#endif

//TODO relocate.
//TODO rename to none prefix versions.
namespace fragcore {
	typedef HCVector3 PVVector3;
	typedef HCVector4 PVVector4;
	typedef HCVector2 PVVector2;
	typedef HCMatrix4x4 PVMatrix4x4;
	typedef HCMatrix3x3 PVMatrix3x3;
	typedef HCQuaternion PVQuaternion;
	typedef HCPlane PVPlane;
	typedef HCAABB PVAABB;
	typedef HCBoundingSphere PVBoundingSphere;
	typedef HCOBB PVOBB;
	typedef HCRay PVRay;
	typedef HCVector4 PVColor;
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
#elif defined(FV_GNUC)
#define FV_ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(FV_GNUC) || defined(FV_GHS)
#define FV_ALWAYS_INLINE inline __attribute__((always_inline))
#else
	#pragma message("Warning: You'd need to add FV_ALWAYS_INLINE for this compiler.")
#endif


/**
 *	Alignment of data and vectors.
 */
#if defined(FV_GNUC) || defined(FV_CLANG)
#define FV_ALIGN(alignment) __attribute__ ((aligned(alignment)))
#define FV_VECTORALIGN(alignment) __attribute__ ((__vector_size__ (alignment), __may_alias__))
#define FV_VECTORALIGNI(alignment) __attribute__ ((__vector_size__ (alignment)))
#elif defined(FV_VC)
#define FV_ALIGN(alignment) __attribute__ ((aligned(alignment)))
	#define FV_VECTORALIGN(alignment) __attribute__ ((__vector_size__ (alignment), __may_alias__))
	#define FV_VECTORALIGNI(alignment) __attribute__ ((__vector_size__ (alignment)))
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
#if defined(FV_GCC) || defined(FV_CLANG)
	#define likely(x)      __builtin_expect(!!(x), 1) 
	#define unlikely(x)    __builtin_expect(!!(x), 0)  
#else
	#define likely(x)      x 
	#define unlikely(x)    x 
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

/*	*/
typedef float fvvec1f FV_VECTORALIGN(4);
typedef float fvvec2f FV_VECTORALIGN(8);
typedef float fvvec4f FV_VECTORALIGN(16);

/*	*/
typedef double fvvec1d FV_VECTORALIGN(8);
typedef double fvvec2d FV_VECTORALIGN(16);
typedef double fvvec4d FV_VECTORALIGN(32);


#ifdef __cplusplus /*	C++ Environment	*/
}
#endif
// /**
//  * Matrix4x4 float
//  *
//  */
// HPM_ALIGN(32)
// typedef hpmvec4f hpmvec4x4f_t[4];
// typedef struct hpmvec4x4f_s{
//      hpmvecf m11,m21,m31,m41,    /*  column1 */
//              m12,m22,m32,m42,    /*  column2 */
//              m13,m23,m33,m43,    /*  column3 */
//              m14,m24,m34,m44;    /*  column4 */
// }hpmmat4f;
// typedef hpmv4sf hpmvec4x4fi_t[4];
// typedef hpmvec4f hpmvec4x4fp_t;
// HPM_ALIGN(16)
// typedef union {
// 	hpmvec4x4f_t m;
// 	hpmmat4f s;
// 	struct {
// 		hpmvec8f oc[2];
// 	};
// } hpmmat4uf;


// /**
//  * Matrix4x4 double.
//  *
//  */
// typedef hpmvec4d hpmvec4x4d_t[4];
// typedef struct hpmvec4x4d_s{
//     hpmvecd m11,m21,m31,m41,    /*  column1 */
//             m12,m22,m32,m42,    /*  column2 */
//             m13,m23,m33,m43,    /*  column3 */
//             m14,m24,m34,m44;    /*  column4 */
// } hpmmat4d;
// HPM_ALIGN(16)
// typedef union {
// 	hpmvec4x4d_t m;
// 	struct hpmvec4x4d_s s;
// 	hpmvec2d t[2][2];
// } hpmmat4ud;


// /**
//  * Matrix2x2 float
//  *
//  */
// HPM_ALIGN(32)
// typedef hpmvec2f hpmvec2x2f_t[2];
// typedef struct hpmvec2x2f_s{hpmvecf m11,m21,m12,m22;}hpmmat2f;
// typedef hpmvec2f hpmvec2x2fp_t;
// HPM_ALIGN(16)
// typedef union {
// 	hpmvec2x2f_t m;
// } hpmmat2uf;

// /**
//  *	Single component vector data type.
//  */
// typedef float hpmvecfv HPM_VECTORALIGN(4);      /*	*/
// typedef double hpmvecdv HPM_VECTORALIGN(8);     /*	*/
// typedef int hpmveciv HPM_VECTORALIGN(4);        /*	*/

// /**
//  *	Two component vector data type.
//  */
// typedef hpmveci hpmvec2i HPM_VECTORALIGN(8);    /*	*/
// typedef hpmvecf hpmvec2f HPM_VECTORALIGN(8);    /*	*/
// typedef hpmvecd hpmvec2d HPM_VECTORALIGN(16);   /*	*/
// typedef struct hpm_vec2uf_t{
// 	union{
// 		hpmvec2f v;
// 		struct{hpmvecf x, y;};
// 	};
// };

// /**
//  *	SSE 128 bit data types.
//  */
// typedef hpmveci hpmvec3i HPM_VECTORALIGN(16);   /*	*/
// typedef hpmvecf hpmvec3f HPM_VECTORALIGN(16);   /*	*/
// typedef hpmvecd hpmvec3d HPM_VECTORALIGN(32);   /*	*/
// typedef struct hpm_vec3uf_t {
// 	union {
// 		hpmvec3f v;
// 		struct {
// 			hpmvecf x, y, z;
// 		};
// 	};
// };


// /**
//  *	SSE 128 bits data types.
//  */
// typedef hpmveci hpmvec4i HPM_VECTORALIGN(16);   /*	*/
// typedef hpmvecf hpmvec4f HPM_VECTORALIGN(16);   /*	*/
// typedef struct hpm_vec4uf_t {
// 	union {
// 		hpmvec4f v;
// 		struct {
// 			hpmvecf x, y, z, w;
// 		};
// 	};
// };

// /**
//  * 	Internal SSE 128 bit  data types
//  *	for implementing the intrinsics.
//  */
// typedef hpmveci hpmv4si HPM_VECTORALIGNI(16);
// typedef hpmvecf hpmv4sf HPM_VECTORALIGNI(16);

// /**
//  * 	AVX	256 bits data types.
//  */
// typedef hpmveci hpmvec8i HPM_VECTORALIGN(32);
// typedef hpmvecf hpmvec8f HPM_VECTORALIGN(32);
// typedef hpmvecd hpmvec4d HPM_VECTORALIGN(32);

// /**
//  *	Union data types
//  */
// HPM_ALIGN(16)
// typedef struct hpmvec8fu_t {
// 	union {
// 		hpmvec4f d2[2];
// 		hpmvec8f d;
// 	};
// } hpmvec8fu;
// HPM_ALIGN(16)
// typedef struct hpmvec4du_t {
// 	union {
// 		hpmvec2d d2[2];
// 		hpmvec4d d;
// 	};
// } hpmvec4du;

// /**
//  *	AVX 512 bits data types.
//  */
// typedef hpmveci hpmvec16i HPM_VECTORALIGN(64);
// typedef hpmvecf hpmvec16f HPM_VECTORALIGN(64);
// typedef hpmvecd hpmvec8d HPM_VECTORALIGN(64);
// HPM_ALIGN(16)
// typedef struct hpmvec8du_t {
// 	union {
// 		hpmvec4d d2[2];
// 		hpmvec8d d;
// 	};
// } hpmvec8du;


#endif