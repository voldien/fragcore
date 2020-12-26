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
#ifndef _FRAG_CORE_ZIP_FILE_IO_H_
#define _FRAG_CORE_ZIP_FILE_IO_H_ 1
#include"../../Def.h"
#include "ZipFileSystem.h"
#include"IO.h"
#include"../Ref.h"
#include<zip.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC ZipFileIO : public IO {
		friend class ZipFileSystem;
	public:
		void open(const char *path, Mode mode) override;

		void close(void) override;

		long read(long int nbytes, void *pbuffer) override;

		long write(long int nbytes, const void *pbuffer) override;

		bool eof(void) const override;

		long length(void) override;

		void seek(long int nbytes, Seek seek) override;

		unsigned long getPos(void) override;

		bool isWriteable(void) const override;

		bool isReadable(void) const override;

		bool flush(void) override;

	private:
		zip_file_t *file;           // Read only
		zip_source_t* zipSource;    // Write only
		zip_int64_t index;          // Index location.
		Ref<ZipFileSystem> zipfile;       // Filesystem reference.
	protected:
		ZipFileIO(zip_file_t *file, zip_int64_t index, Ref<ZipFileSystem> ref);

	public:
		ZipFileIO(void);

		ZipFileIO(const ZipFileIO &other);

		~ZipFileIO(void);

	};
}

#endif