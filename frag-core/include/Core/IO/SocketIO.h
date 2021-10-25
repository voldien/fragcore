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
#ifndef _FRAG_CORE_SOCKET_IO_H_
#define _FRAG_CORE_SOCKET_IO_H_ 1
#include "../Network/NetSocket.h"
#include "../Ref.h"
#include "IO.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC SocketIO : public IO {
	  public:
		virtual void open(const char *path, IOMode mode) override;

		virtual void close() override;

		virtual long int read(long int nbytes, void *pbuffer) override;

		virtual long int write(long int nbytes, const void *pbuffer) override;

		virtual long int peek(long int nBytes, void *pbuffer) override;

		virtual bool eof() const override;

		virtual long int length() override;

		virtual void seek(long int nbytes, Seek seek) override;

		virtual unsigned long getPos() override;

		virtual bool isWriteable() const override;

		virtual bool isReadable() const override;

		virtual bool flush() override;

		virtual bool isOperationSupported(IOOperation operations) const noexcept override {
			const IOOperation supportedIO = static_cast<IOOperation>(OP_READ | OP_WRITE | OP_PEEK);
			return (operations & supportedIO) != operations;
		}

	  public:
		SocketIO(Ref<NetSocket> &socket);

	  private:
		SocketIO() = default;

	  protected:
		Ref<NetSocket> netSocket;
	};
} // namespace fragcore

#endif
