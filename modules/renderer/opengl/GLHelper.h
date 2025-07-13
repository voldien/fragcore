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
#ifndef _FRAGCORE_GL_HELPER_H_
#define _FRAGCORE_GL_HELPER_H_ 1
#include <FragCore.h>
#include <GL/glew.h>

#if _NDEBUG
#define FVALIDATE_GL_CALL(x)                                                                                           \
	{                                                                                                                  \
		x;                                                                                                             \
		GLenum gl_error = glGetError();                                                                                \
		if (gl_error != GL_NO_ERROR) {                                                                                 \
			throw cxxexcept::RuntimeException("{} {} {} - {}", __FILE__, __LINE__, _err, gluErrorString(gl_error));    \
		}                                                                                                              \
	}
#else
#define FVALIDATE_GL_CALL(x) x
#endif

namespace fragcore {

	extern FVDECLSPEC void checkError();
	extern FVDECLSPEC void resetErrorFlag();
	extern FVDECLSPEC bool validateExistingProgram();
	extern FVDECLSPEC std::string getProgramValidateString();
} // namespace fragcore

#endif