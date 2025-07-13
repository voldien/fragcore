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
#ifndef _FRAGCORE_LZ4_FILEIO_H_
#define _FRAGCORE_LZ4_FILEIO_H_ 1
#include "LZ4Compression.h"
#include <IO/FileIO.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC LZ4IO : public FileIO {
	  public:
		long read(long int nbytes, void *pbuffer) override;

		long write(long int nbytes, const void *pbuffer) override;

		long length() override;

		bool eof() const override;

		void seek(long int nbytes, const Seek seek) override;

		unsigned long getPos() override;

		bool isWriteable() const override;

		bool isReadable() const override;

		bool flush() override;

		void close() override;

		void open(const char *path, IOMode mode) override;

		bool isOperationSupported(IOOperation operations) const noexcept override {
			return (operations & (OP_READ | OP_WRITE | OP_FLUSH | OP_FILEACCESS | OP_PEEK)) != operations;
		};

	  protected:
		LZ4Compression compressor;

	  public:
		LZ4IO(const char *path, IOMode mode);
	};
} // namespace fragcore

#endif
