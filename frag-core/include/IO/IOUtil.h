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
#ifndef _FRAGCORE_IOUTIL_H_
#define _FRAGCORE_IOUTIL_H_ 1
#include "../Core/Ref.h"
#include "IO.h"
#include "IO/IFileSystem.h"
#include <fmt/format.h>
#include <string>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IOUtil {
	  public:
		static long int loadFileMem(Ref<IO> &ioRef, char **data);

		static long int loadFile(Ref<IO> &in_io, Ref<IO> &out);

		static long int loadStringMem(Ref<IO> &in_io, char **data);

		static long int loadString(Ref<IO> &in, Ref<IO> &out);

		static long int saveFileMem(Ref<IO> &io, char *data, size_t size);

		template <typename... Args> static long int format(Ref<IO> &io, const std::string &format, Args &&...args) {
			std::string formatted = fmt::format(format, args...);
			return io->write(formatted.length(), formatted.data());
		}

		template <typename T> static std::vector<T> readFileData(Ref<IO> &io) {

			const size_t fileSize = static_cast<size_t>(io->length());
			std::vector<T> buffer(fileSize / sizeof(T));

			io->read(fileSize, buffer.data());

			return buffer;
		}

		template <typename T> static std::vector<T> readString(Ref<IO> &io) {

			const size_t fileSize = static_cast<size_t>(io->length());
			std::vector<T> buffer((fileSize / sizeof(T)) + sizeof(T));

			io->read(fileSize, buffer.data());
			buffer.data()[fileSize] = '\0';

			return buffer;
		}

		template <typename T>
		static std::vector<T> readFileString(const std::string &filename, IFileSystem *filesystem) {

			assert(filesystem);

			Ref<IO> ref = Ref<IO>(filesystem->openFile(filename.c_str(), IO::IOMode::READ));
			std::vector<char> string = fragcore::IOUtil::readString<T>(ref);
			ref->close();
			return string;
		}

		template <typename T> static std::vector<T> readFileData(const std::string &filename, IFileSystem *filesystem) {

			assert(filesystem);

			Ref<IO> ref = Ref<IO>(filesystem->openFile(filename.c_str(), IO::IOMode::READ));
			std::vector<T> buffer = fragcore::IOUtil::readFileData<T>(ref);
			ref->close();
			return buffer;
		}
	};
} // namespace fragcore

#endif
