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
#ifndef _FRAG_CORE_LOGGER_H_
#define _FRAG_CORE_LOGGER_H_ 1
#include "IO/IO.h"
#include "Object.h"
#include "Ref.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */

	class FVDECLSPEC ILogger : public Object {
	  public:
		/**
		 *	Verbosity level of the program.
		 */
		enum class Verbosity {
			Quite = 0,	 /*  */
			Error = 1,	 /*  */
			Verbose = 2, /*  */
			Warning = 3, /*  */
			Debug = 16,	 /*  */
		};

		/**
		 *
		 * @param verbosity
		 * @param format
		 * @param ...
		 */
		virtual size_t log(Verbosity verbosity, const char *format, ...) = 0;

		ILogger() = default;
	};
} // namespace fragcore

#endif
