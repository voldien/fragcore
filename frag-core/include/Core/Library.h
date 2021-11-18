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
#ifndef _FRAG_CORE_LIBRARY_H_
#define _FRAG_CORE_LIBRARY_H_ 1
#include "../Def.h"
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
		Library(const Library &other);
		Library(Library &&other);

		~Library();

		/**
		 *	Open library.
		 *
		 *	@Return true if successfully.
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
		 *	@Return true if valid library.
		 */
		bool isValid() const;

		/**
		 *	Get function pointer.
		 *
		 *	@Return function pointer if successfully.
		 */
		void *getfunc(const char *cfunctionName);

		template <typename T> T getfunc(const char *cfunctionName) {
			return reinterpret_cast<T>(this->getfunc(cfunctionName));
		}

	  private: /*	Attributes.	*/
		void *mlib;
		std::string name;
	};
} // namespace fragcore

#endif
