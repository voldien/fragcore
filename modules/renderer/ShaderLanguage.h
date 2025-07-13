/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_SHADER_LANGUAGE_H_
#define _FRAGCORE_SHADER_LANGUAGE_H_ 1
#include <ImageFormat.h>
#include <cstdint>

namespace fragcore {
	/**
	 *  Support languages.
	 */
	enum ShaderLanguage : uint32_t {
		NONE = 0,
		GLSL = 0x1,			   /*  OpenGL GLSL.    */
		SPIRV = 0x2,		   /*  SPIRV.  */
		HLSL = 0x4,			   /*  High Level Shading Language.    */
		CLC = 0x8,			   /*  OpenCL C language.  */
		CG = 0x10,			   /*  Legacy. */
		unKnownLanguage = 0x0, /*	*/
	};
} // namespace fragcore
#endif