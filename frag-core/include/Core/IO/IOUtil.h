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
#ifndef _FRAG_CORE_IOUTIL_H_
#define _FRAG_CORE_IOUTIL_H_ 1
#include "../Ref.h"
#include "IO.h"

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC IOUtil {
	  public:

		/**
		 * @brief
		 *
		 * @param io
		 * @param data
		 * @return long int
		 */
		static long int loadFileMem(Ref<IO> &io, char **data) noexcept(noexcept(data != nullptr));
		static long int loadFile(Ref<IO> &in, Ref<IO> &out);

		/**
		 * @brief
		 *
		 * @param io
		 * @param data
		 * @return long int
		 */
		static long int loadStringMem(Ref<IO> &io, char **data) noexcept(noexcept(data != nullptr));
		static long int loadString(Ref<IO> &in, Ref<IO> &out);

		/**
		 * @brief 
		 * 
		 * @param io 
		 * @param vformat 
		 * @param ... 
		 * @return long int 
		 */
		static long int format(Ref<IO> &io, const char *vformat, ...) noexcept(noexcept(vformat != nullptr));
	};
} // namespace fragcore

#endif
