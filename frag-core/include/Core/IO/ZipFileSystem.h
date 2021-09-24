/**
	FrameView for rendering shaders in screen space.
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
#ifndef _FRAG_CORE_ZIP_FILE_H_
#define _FRAG_CORE_ZIP_FILE_H_ 1
#include "../../Def.h"
#include "../../Prerequisites.h"
#include "../RefPtr.h"
#include "IFileSystem.h"
#include <vector>

namespace fragcore {
	/**
	 *	Zip file container.
	 */
	class FVDECLSPEC ZipFileSystem : public IFileSystem {
		friend class ZipFileIO;

	  public:
		virtual ~ZipFileSystem(void);

		IO *openFile(const char *path, IO::IOMode mode);
		IO *openFile(unsigned int index);

		virtual std::vector<std::string> listFiles(const char *path) const;
		virtual std::vector<std::string> listDirectories(const char *path) const;
		virtual std::vector<std::string> list(const char *path) const;

		void closeFile(IO *io) override;

		void remove(const char *path) override;

		void rename(const char *oldPath, const char *newPath) override;

		void createFile(const char *path) override;

		void createDirectory(const char *path) override;

		bool isReadable(const char *path) const override;

		bool isWriteable(const char *path) const override;

		bool exists(const char *path) const override;

		bool isASyncSupported(void) const override;

		virtual bool isDirectory(const char *path);
		virtual bool isFile(const char *path);
		// FileAccess getFileAccess(const char *path) override;

		/**
		 * Release
		 */
		virtual void release(void);

	  private:		/*	*/
		void *pzip; /**/

		virtual void *getZipObject(void) const;

	  public: /*	Factory method.	*/
		/*  Create zip filesystem object.   */
		static ZipFileSystem *createZipFileObject(Ref<IO> &ioRef, Ref<IScheduler> ref = Ref<IScheduler>(nullptr));
		static ZipFileSystem *createZipFileObject(void *source, int size,
												  Ref<IScheduler> ref = Ref<IScheduler>(nullptr));
		static ZipFileSystem *
		createZipFileObject(const char *cfilename,
							Ref<IScheduler> ref = Ref<IScheduler>(nullptr)); /*	Will call the IO method */

	  protected: /*	Prevent one from creating an instance of this class.	*/
		ZipFileSystem(Ref<IScheduler> ref);
		ZipFileSystem(const ZipFileSystem &other);
	};
} // namespace fragcore

#endif
