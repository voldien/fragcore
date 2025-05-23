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
#ifndef _FRAGCORE_IFILESYSTEM_H_
#define _FRAGCORE_IFILESYSTEM_H_ 1
#include "ASyncIO.h"
#include <vector>

namespace fragcore {

	/**
	 * @brief FileSystem Interface.
	 *
	 */
	class FVDECLSPEC IFileSystem : public ASyncIO {
	  public:
		virtual IO *openFile(const char *path, IO::IOMode mode) = 0; /*  Open based on the filename extension.*/
		virtual ASyncHandle openASyncFile(const char *path, IO::IOMode mode) {
			if (!isASyncSupported()) {
				throw RuntimeException("No Task Scheduler associated with the AsyncIO");
			}

			/*	*/
			Ref<IO> io = Ref<IO>(this->openFile(path, mode));
			return this->asyncOpen(io);
		}
		virtual void closeFile(IO *io) = 0;

		/**
		 * @brief
		 *
		 * @param path
		 */
		virtual void remove(const char *path) = 0;

		/**
		 * @brief
		 *
		 * @param oldPath
		 * @param newPath
		 */
		virtual void rename(const char *oldPath, const char *newPath) = 0;

		/**
		 * @brief Create a File object
		 *
		 * @param path
		 */
		virtual void createFile(const char *path) = 0;

		/**
		 * @brief Create a Directory object
		 *
		 * @param path
		 */
		virtual void createDirectory(const char *path) = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @return true
		 * @return false
		 */
		virtual bool isReadable(const char *path) const = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @return true
		 * @return false
		 */
		virtual bool isWriteable(const char *path) const = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @return true
		 * @return false
		 */
		virtual bool exists(const char *path) const = 0;

		virtual bool isASyncSupported() const = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @return true
		 * @return false
		 */
		virtual bool isDirectory(const char *path) const = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @return true
		 * @return false
		 */
		virtual bool isFile(const char *path) const = 0;

		// virtual FileAccess getFileAccess(const std::string& path) = 0;
		// virtual DirectoryAccess getDirectoryAccess(const std::string& path) = 0;

		/**
		 * @brief
		 *
		 * @param directory
		 * @return std::vector<std::string>
		 */
		virtual std::vector<std::string>
		listFiles(const char *directory) const = 0; /*  Display only files in directory.    */

		/**
		 * @brief
		 *
		 * @param directory
		 * @return std::vector<std::string>
		 */
		virtual std::vector<std::string>
		listDirectories(const char *directory) const = 0; /*  Display only directories in directory.  */

		/**
		 * @brief
		 *
		 * @param directory
		 * @return std::vector<std::string>
		 */
		virtual std::vector<std::string> list(const char *directory) const = 0; /*  Display everything in directory. */
		
	  public:
		virtual std::string getBaseName(const char *path) { return path; }

		virtual std::string getAbsolutePath(const char *path) { return path; }

		virtual std::string getRelativePath(const char *path) { return path; }

		virtual std::string getFileExtension(const char *path) { return path; }
	};
} // namespace fragcore

#endif
