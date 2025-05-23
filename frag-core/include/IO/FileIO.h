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
#ifndef _FRAGCORE_FILE_IO_H_
#define _FRAGCORE_FILE_IO_H_ 1
#include "IO.h"
#include <cstdio>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC FileIO : public IO {
	  public:
		long read(long int nbytes, void *pbuffer) override;

		long write(long int nbytes, const void *pbuffer) override;

		long int peek(long int nBytes, void *pbuffer) override;

		void close() override;

		long length() override;

		unsigned long getPos() override;

		bool eof() const override;

		void seek(long int nbytes, const Seek seek) override;

		bool isWriteable() const override;

		bool isReadable() const override;

		void open(const char *path, IOMode mode) override;

		bool flush() override;

		bool isOperationSupported(const IOOperation operations) const noexcept override {
			return (operations & OP_ALL) == operations;
		};

	  public: /*	object specific methods.	*/
		int getFileDescriptor() { return fileno(this->file); }

		void setBlocking(bool blocking);

	  protected:
		FILE *file;
		IOMode mode;

		FileIO();

	  public:
		FileIO(const char *path, IOMode mode);
		FileIO(const std::string &path, IOMode mode);
		FileIO(int file_d, IOMode mode);
		~FileIO() override;

		FileIO(FILE *file); // TODO perhaps add IOMOde for overriding.
		FileIO(FileIO &&other);
	};
} // namespace fragcore
#endif
