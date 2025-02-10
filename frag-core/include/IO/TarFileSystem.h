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
#ifndef _FRAGCORE_TAR_FILESYSTEM_H_
#define _FRAGCORE_TAR_FILESYSTEM_H_ 1
#include "../FragDef.h"
#include "../Prerequisites.h"
#include "IFileSystem.h"
#include <vector>

namespace fragcore {

	/**
	 * Zip file container.
	 */
	class FVDECLSPEC TarFileSystem : public IFileSystem {
		friend class ZipFileIO;

	  public:
		~TarFileSystem() override;

	  public:
		IO *openFile(const char *path, IO::IOMode mode) override;
		IO *openFile(unsigned int index);

		std::vector<std::string> listFiles(const char *path) const override;
		std::vector<std::string> listDirectories(const char *path) const override;
		std::vector<std::string> list(const char *path) const override;

		void closeFile(IO *io) override;

		void remove(const char *path) override;

		void rename(const char *oldPath, const char *newPath) override;

		void createFile(const char *path) override;

		void createDirectory(const char *path) override;

		bool isReadable(const char *path) const override;

		bool isWriteable(const char *path) const override;

		bool exists(const char *path) const override;

		bool isASyncSupported() const override;

		bool isDirectory(const char *path) const override;
		bool isFile(const char *path) const override;

		// FileAccess getFileAccess(const char *path) override;

		/**
		 * Release
		 */
		virtual void release();

	  public:
		// TODO add auth to allow to read encrypted files.
		//	  void authenicate();
	  private:		/*	*/
		void *pzip; /**/

		virtual void *getZipObject() const;

	  public: /*	Factory method.	*/
		/*  Create zip filesystem object.   */
		static TarFileSystem *createZipFileObject(Ref<IO> &ioRef, Ref<IScheduler> ref = Ref<IScheduler>(nullptr));
		static TarFileSystem *createZipFileObject(void *source, int size,
												  Ref<IScheduler> ref = Ref<IScheduler>(nullptr));
		static TarFileSystem *
		createZipFileObject(const char *cfilename,
							Ref<IScheduler> ref = Ref<IScheduler>(nullptr)); /*	Will call the IO method */

	  protected: /*	Prevent one from creating an instance of this class.	*/
		TarFileSystem(Ref<IScheduler> ref);
		TarFileSystem(const TarFileSystem &other);
	};
} // namespace fragcore

#endif
