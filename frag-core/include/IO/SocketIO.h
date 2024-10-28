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
#ifndef _FRAGCORE_SOCKET_IO_H_
#define _FRAGCORE_SOCKET_IO_H_ 1
#include "../Network/NetSocket.h"
#include "../Core/Ref.h"
#include "IO.h"

namespace fragcore {

	/**
	 * @brief Encapsulate a socket as an IO object.
	 *
	 */
	class FVDECLSPEC SocketIO : public IO {
	  public:
		void open(const char *path, IOMode mode) override;

		void close() override;

		long int read(long int nbytes, void *pbuffer) override;

		long int write(long int nbytes, const void *pbuffer) override;

		long int peek(long int nBytes, void *pbuffer) override;

		bool eof() const override;

		long int length() override;

		void seek(long int nbytes, const Seek seek) override;

		unsigned long getPos() override;

		bool isWriteable() const override;

		bool isReadable() const override;

		bool flush() override;

		bool isOperationSupported(const IOOperation operations) const noexcept override {
			const IOOperation supportedIO = static_cast<IOOperation>(OP_READ | OP_WRITE | OP_PEEK | OP_FILEACCESS |
																	 OP_FILEACCESS | OP_FLUSH | OP_EOF);
			return (operations & supportedIO) == operations;
		}

	  public:
		SocketIO(Ref<NetSocket> &socket);
		SocketIO(SocketIO &&other);

	  private:
		SocketIO() = default;

	  protected:
		Ref<NetSocket> netSocket;
		IOMode iomode;
	};
} // namespace fragcore

#endif
