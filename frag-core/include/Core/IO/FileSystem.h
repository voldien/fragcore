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
#ifndef _FRAG_CORE_FILE_H_
#define _FRAG_CORE_FILE_H_ 1
#include "../../Def.h"
#include "ASyncIO.h"
#include "IFileSystem.h"
#include "IO.h"

namespace fragcore {
	/**
	 * IO operations.
	 */
	class FVDECLSPEC FileSystem : public IFileSystem {
	  public:
		virtual IO *openFile(const char *path, IO::IOMode mode) override; /*  Open based on the filename extension.*/
		virtual void closeFile(IO *io) override;

		virtual void remove(const char *path) override;

		virtual void rename(const char *oldPath, const char *newPath) override;

		virtual void createFile(const char *path) override;

		virtual void createDirectory(const char *path) override;

		virtual bool isReadable(const char *path) const override;

		virtual bool isWriteable(const char *path) const override;

		virtual bool exists(const char *path) const override;

		virtual bool isASyncSupported() const override;
		virtual bool isDirectory(const char *path) override;
		virtual bool isFile(const char *path) override;

		// FileAccess getFileAccess(const char *path) override;

		std::vector<std::string> listFiles(const char *directory) const override;

		std::vector<std::string> listDirectories(const char *directory) const override;

		std::vector<std::string> list(const char *directory) const override;

		// TODO determine how to deal with.
		static const char *getBaseName(const char *path);

		static std::string getAbsolutePath(const char *path);

		static std::string getRelativePath(const char *path);

		static const char *getFileExtension(const char *path);

	  public:
		static FileSystem *getFileSystem();
		static FileSystem *createFileSystem(Ref<IScheduler> &sch);
		static FileSystem *createFileSystem(); /*	Filesystem without async support.	*/

		FileSystem(const FileSystem &&other);
		// TODO add remove function.
	  protected:
		FileSystem(Ref<IScheduler> &sch);
		FileSystem();
		~FileSystem();
	};
} // namespace fragcore

#endif
