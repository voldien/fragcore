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
#ifndef _FRAG_CORE_MODBUS_REGISTER_IO_H_
#define _FRAG_CORE_MODBUS_REGISTER_IO_H_ 1
#include "Core/IO/IO.h"
#include "NetModbusSocket.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC ModbusRegisterIO : public IO {
	  public:
		// TODO add shared pointer.
		ModbusRegisterIO(ModbusNetSocket &socket, const size_t read_address_start, const size_t read_size_bytes,
						 const size_t write_address_start, const size_t write_size_bytes);
		~ModbusRegisterIO() override;

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

		bool isOperationSupported(IOOperation operations) const noexcept override {
			const IOOperation supportedIO =
				static_cast<IOOperation>(OP_READ | OP_WRITE | OP_SEEK | OP_FLUSH | OP_LENGTH);
			return (operations & supportedIO) == operations;
		}

	  private:
		ModbusNetSocket &socket;
		size_t address_write_start;
		size_t address_read_start;
		size_t write_size;
		size_t read_size;
		size_t seek_write_offset = 0;
		size_t seek_read_offset = 0;
	};
} // namespace fragcore

#endif
