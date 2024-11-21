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
#ifndef _FRAGCORE_IOUTIL_H_
#define _FRAGCORE_IOUTIL_H_ 1
#include "../Core/Ref.h"
#include "IO.h"
#include <fmt/format.h>
#include <string>

namespace fragcore {
	/**
	 * @brief
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
		static long int loadFileMem(Ref<IO> &ioRef, char **data);

		/**
		 * @brief
		 *
		 * @param in
		 * @param out
		 * @return long int
		 */
		static long int loadFile(Ref<IO> &in_io, Ref<IO> &out);

		/**
		 * @brief
		 *
		 * @param io
		 * @param data
		 * @return long int
		 */
		static long int loadStringMem(Ref<IO> &in_io, char **data);

		/**
		 * @brief
		 *
		 * @param in
		 * @param out
		 * @return long int
		 */
		static long int loadString(Ref<IO> &in, Ref<IO> &out);

		static long int saveFileMem(Ref<IO> &io, char *data, size_t size);

		/**
		 * @brief
		 *
		 * @param io
		 * @param vformat
		 * @param ...
		 * @return long int
		 */
		template <typename... Args>
		static inline long int format(Ref<IO> &io, const std::string &format, Args &&...args) {
			std::string formatted = fmt::format(format, args...);
			// TODO allow to perform chunk if needed
			return io->write(formatted.length(), formatted.data());
		}

		/**
		 * @brief
		 *
		 * @tparam T
		 * @param io
		 * @return std::vector<T>
		 */
		template <typename T> static std::vector<T> readFileData(Ref<IO> &io) {

			const size_t fileSize = static_cast<size_t>(io->length());
			std::vector<T> buffer(fileSize / sizeof(T));

			io->read(fileSize, buffer.data());

			return buffer;
		}

		/**
		 * @brief
		 *
		 * @tparam T
		 * @param io
		 * @return std::vector<T>
		 */
		template <typename T> static std::vector<T> readString(Ref<IO> &io) {

			const size_t fileSize = static_cast<size_t>(io->length());
			std::vector<T> buffer(fileSize / sizeof(T) + 1);

			io->read(fileSize, buffer.data());
			buffer.data()[fileSize] = '\0';

			return buffer;
		}
	};
} // namespace fragcore

#endif
