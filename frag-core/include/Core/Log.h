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
#ifndef _FRAG_CORE_LOG_H_
#define _FRAG_CORE_LOG_H_ 1
#include "IO/IO.h"
#include "Object.h"
#include "Ref.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	// TODO object container.
	// TOOD remove from being static - using spdlog.
	class FVDECLSPEC Log : public Object {
	  public:
		/**
		 *	Verbosity level of the program.
		 */
		enum VERBOSITY {
			Quite = 0,	 /*  */
			Error = 1,	 /*  */
			Verbose = 2, /*  */
			Warning = 3, /*  */
			Debug = 16,	 /*  */
		};

		/**
		 *
		 */
		enum LogMappingFlag {
			fQuite = (1 << VERBOSITY::Quite),
			fError = (1 << VERBOSITY::Error),
			fVerbose = (1 << VERBOSITY::Verbose),
			fWarning = (1 << VERBOSITY::Warning),
			fDebug = (1 << VERBOSITY::Debug),
		};

		/**
		 * Set verbosity
		 * @param verbosity
		 */
		static void setVerbosity(VERBOSITY verbosity);

		/**
		 *
		 * @return
		 */
		static VERBOSITY getVerbosity();

		/**
		 *
		 * @param verbosity
		 * @param format
		 * @param ...
		 */
		static int log(VERBOSITY verbosity, const char *format, ...);

		/**
		 *
		 * @param format
		 * @param ...
		 * @return
		 */
		static int log(const char *format, ...);

		/**
		 *
		 * @param format
		 * @param ...
		 * @return
		 */
		static int error(const char *format, ...);
		static int debug(const char *format, ...);

		/*  TOOD improve the design.    */
		// TODO add support.
		static void addIOOutput(Ref<IO> &io, const VERBOSITY mapping);
		static void removeIOOutPut(Ref<IO> &io);
		static void clearIO();

	  private:
		static int logv(VERBOSITY verbosity, const char *format, va_list va);

		Log() = default;
	};
} // namespace fragcore

#endif
