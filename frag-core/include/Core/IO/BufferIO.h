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
#ifndef _FRAG_CORE_BUFFERIO_H_
#define _FRAG_CORE_BUFFERIO_H_ 1
#include "../../FragDef.h"
#include "IO.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC BufferIO : public IO { // TODO rename so that it understood it is a system memory IO object.
	  public:
		void open(const char *path, IOMode mode) override;

		void close() override;

		long read(long int nbytes, void *pbuffer) override;

		long write(long int nbytes, const void *pbuffer) override;

		long int peek(long int nBytes, void *pbuffer) override;

		bool eof() const override;

		long length() override;

		void seek(long int nbytes, Seek seek) override;

		unsigned long getPos() override;

		bool isWriteable() const override;

		bool isReadable() const override;

		bool flush() override;

		bool isOperationSupported(IOOperation operations) const noexcept override {
			return (operations & OP_ALL) != operations;
		};

	  private:
		// TODO handle memory ownership.
		uint8_t *buffer;
		unsigned long nbytes;
		unsigned long marker;
		bool expandable;
		IOMode ioMode;

	  public:
		BufferIO(const void *pBuffer, unsigned long size);
		BufferIO(void *pBuffer, unsigned long size);
		BufferIO(unsigned long size, bool expandable = false);
		~BufferIO() override;
	};
} // namespace fragcore
#endif
