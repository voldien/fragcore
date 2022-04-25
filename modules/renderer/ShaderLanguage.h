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
#ifndef _FRAG_CORE_SHADER_LANGUAGE_H_
#define _FRAG_CORE_SHADER_LANGUAGE_H_ 1
#include "GraphicFormat.h"
#include "RenderPrerequisites.h"
#include <ImageFormat.h>

namespace fragcore {
	/**
	 *  Support languages.
	 */
	enum ShaderLanguage {
		GLSL = 0x1,			   /*  OpenGL GLSL.    */
		SPIRV = 0x2,		   /*  SPIRV.  */
		HLSL = 0x4,			   /*  High Level Shading Language.    */
		CLC = 0x8,			   /*  OpenCL C language.  */
		CG = 0x10,			   /*  Legacy. */
		unKnownLanguage = 0x0, /*	*/
	};
} // namespace fragcore
#endif