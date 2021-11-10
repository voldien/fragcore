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
#include <fmt/format.h>
#include <string>

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
		static long int loadFileMem(Ref<IO> &io, char **data);
		static long int loadFile(Ref<IO> &in, Ref<IO> &out);

		/**
		 * @brief
		 *
		 * @param io
		 * @param data
		 * @return long int
		 */
		static long int loadStringMem(Ref<IO> &io, char **data);
		static long int loadString(Ref<IO> &in, Ref<IO> &out);

		/**
		 * @brief
		 *
		 * @param io
		 * @param vformat
		 * @param ...
		 * @return long int
		 */
		template <typename... Args>
		static inline long int format(Ref<IO> &io, const std::string &format, Args &&... args) {
			std::string formatted = fmt::format(format, args...);
			// TODO allow to perform chunk if needed
			return io->write(formatted.length(), formatted.data());
		}
	};
} // namespace fragcore

#endif
