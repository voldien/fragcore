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
#ifndef _FRAG_CORE_ZIP_FILE_IO_H_
#define _FRAG_CORE_ZIP_FILE_IO_H_ 1
#include "../../FragDef.h"
#include "../Ref.h"
#include "IO.h"
#include "ZipFileSystem.h"
#include <zip.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC ZipFileIO : public IO {
		friend class ZipFileSystem;

	  public:
		void open(const char *path, IOMode mode) override;

		void close() override;

		long read(long int nbytes, void *pbuffer) override;

		long write(long int nbytes, const void *pbuffer) override;

		long int peek(long int nBytes, void *pbuffer) override;

		bool eof() const override;

		long length() override;

		void seek(long int nbytes, const Seek seek) override;

		unsigned long getPos() override;

		bool isWriteable() const override;

		bool isReadable() const override;

		bool flush() override;

		bool isOperationSupported(IOOperation operations) const noexcept override {
			return (operations & OP_ALL) != operations;
		};

	  private:
		zip_file_t *file;			// Read only
		zip_source_t *zipSource;	// Write only
		zip_int64_t index;			// Index location.
		Ref<ZipFileSystem> zipfile; // Filesystem reference.
	  protected:
		ZipFileIO(zip_file_t *file, zip_int64_t index, Ref<ZipFileSystem> ref);

	  public:
		ZipFileIO();

		ZipFileIO(const ZipFileIO &other);
		ZipFileIO(const ZipFileIO &&other);
		~ZipFileIO() override;
	};
} // namespace fragcore

#endif
