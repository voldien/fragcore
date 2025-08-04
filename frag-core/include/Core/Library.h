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
#ifndef _FRAGCORE_LIBRARY_H_
#define _FRAGCORE_LIBRARY_H_ 1
#include "../FragDef.h"
#include "SmartReference.h"
#include <string>

namespace fragcore {

	/**
	 *	Load dynamic library in runtime.
	 */
	class FVDECLSPEC Library : public SmartReference {
	  public:
		Library();

		Library(const char *clibrary);
		Library(const std::string &clibrary);
		Library(const Library &other);
		Library(Library &&other);

		Library &operator=(Library &&other);

		Library &operator=(const Library &other);

		~Library() override;

		/**
		 *	Open library.
		 *
		 *	@return true if successfully.
		 */
		bool open(const char *clibrary);

		/**
		 *	Close library.
		 *	This will unload all function pointer loaded
		 *	from this library object.
		 */
		void close();

		/**
		 *	Determine if loaded library is valid.
		 *
		 *	@return true if valid library.
		 */
		bool isValid() const noexcept;

		/**
		 * @brief Get the Path object
		 */
		std::string getPath() const noexcept { return this->path; }

		/**
		 *	Get function pointer.
		 *
		 *	@return function pointer if successfully.
		 */
		void *getfunc(const char *cfunctionName);

		/**
		 *	Get function pointer.
		 *
		 *	@return function pointer if successfully.
		 */
		template <typename T> T getfunc(const char *cfunctionName) {
			return reinterpret_cast<T>(this->getfunc(cfunctionName));
		}

	  private: /*	Attributes.	*/
		void *mlib;
		std::string path;
	};
} // namespace fragcore

#endif
