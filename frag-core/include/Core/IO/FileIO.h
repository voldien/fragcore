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
#ifndef _FRAG_CORE_FILE_IO_H_
#define _FRAG_CORE_FILE_IO_H_ 1
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

		bool isOperationSupported(IOOperation operations) const noexcept override {
			return (operations & OP_ALL) == operations;
		};

	  public: /*	object specific methods.	*/
		int getFileDescriptor() { return 0; }

	  protected:
		FILE *file;
		IOMode mode;

		FileIO();

	  public:
		FileIO(const char *path, IOMode mode);
		FileIO(const std::string &path, IOMode mode);
		FileIO(int fd, IOMode mode);
		~FileIO() override;

		FileIO(FILE *file); // TODO perhaps add IOMOde for overriding.
		FileIO(FileIO &&other);
	};
} // namespace fragcore
#endif
