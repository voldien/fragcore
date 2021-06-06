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
#ifndef _FRAG_CORE_IFILESYSTEM_H_
#define _FRAG_CORE_IFILESYSTEM_H_ 1
#include "../../Exception/RuntimeException.h"
#include "ASync.h"
#include <vector>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IFileSystem : public ASync {
	  public:
		virtual IO *openFile(const char *path, IO::Mode mode) = 0; /*  Open based on the filename extension.*/
		virtual ASyncHandle openASyncFile(const char *path, IO::Mode mode) {
			if (!isASyncSupported())
				throw RuntimeException();

			/*	*/
			Ref<IO> io = Ref<IO>(this->openFile(path, mode));
			return this->asyncOpen(io);
		}
		virtual void closeFile(IO *io) = 0;

		virtual void remove(const char *path) = 0;
		virtual void rename(const char *oldPath, const char *newPath) = 0;

		virtual void createFile(const char *path) = 0;
		virtual void createDirectory(const char *path) = 0;

		virtual bool isReadable(const char *path) const = 0;
		virtual bool isWriteable(const char *path) const = 0;
		virtual bool exists(const char *path) const = 0;

		virtual bool isASyncSupported(void) const = 0;
		virtual bool isDirectory(const char *path) = 0;
		virtual bool isFile(const char *path) = 0;

		// virtual FileAccess getFileAccess(const char* path) = 0;
		// virtual DirectoryAccess getDirectoryAccess(const char* path) = 0;

		virtual std::vector<std::string>
		listFiles(const char *directory) const = 0; /*  Display only files in directory.    */
		virtual std::vector<std::string>
		listDirectories(const char *directory) const = 0; /*  Display only directories in directory.  */
		virtual std::vector<std::string> list(const char *directory) const = 0; /*  Display everything in directory. */
	  public:
	};
} // namespace fragcore

#endif
