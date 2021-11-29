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
#ifndef _FRAG_CORE_SERIAL_FILE_IO_H_
#define _FRAG_CORE_SERIAL_FILE_IO_H_ 1
#include <Core/IO/IO.h>
#include <libserialport.h>
#include <optional>
#include <vector>

// TODO move to a module.
namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class SerialIO : public IO {
	  public:
		SerialIO(const std::string &path, IOMode mode);
		virtual ~SerialIO();

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
			const IOOperation supportedIO = static_cast<IOOperation>(OP_READ | OP_WRITE | OP_FLUSH);
			return (operations & supportedIO) != operations;
		};

	  public:
		enum class FlowControl {
			FlowControlNone = 0,
			FlowControlXonXoff = 1,
			FlowControlRtsCts = 2,
			FlowControlDtrDsr = 3
		};

		enum class Parity { ParityNone, ParityOdd, ParityEven, ParityMark, ParitySpace };

		enum class StopBits { ONEBIT = 1, TWOBIT = 2 };

		enum class XonXoff {
			XonXoffDisable,
			XonXoffIn,
			XonXoffOut,
			XonXoffInOut,
		};

		enum class BaudRate {
			BAUDRATE_110 = 110,
			BAUDRATE_300 = 300,
			BAUDRATE_600 = 600,
			BAUDRATE_1200 = 1200,
			BAUDRATE_2400 = 2400,
			BAUDRATE_4800 = 4800,
			BAUDRATE_9600 = 9600,
			BAUDRATE_14400 = 14400,
			BAUDRATE_19200 = 19200,
			BAUDRATE_38400 = 38400,
			BAUDRATE_57600 = 57600,
			BAUDRATE_115200 = 115200,
		};

	  public:
		void setBaudRate(unsigned int baudRate);
		BaudRate getBaudRate() const;

		void setStopBits(StopBits stopBits);
		StopBits getStopBits() const;

		void setFlowControl(FlowControl flowControl);
		FlowControl getFlowControl() const;

		void setParity(Parity flowControl);
		Parity getParity() const;

		void setXonXoff(XonXoff XonXoff);
		XonXoff getXonXoff();

		void setPayloadBits(unsigned int nrBits);
		int getPayloadBits() const;

	  public:
		static bool supportedBaudRate(unsigned int baudRate);
		/**
		 *
		 */
		static std::optional<std::vector<std::string>> getSerialPorts() noexcept;

	  private:
		virtual void open([[maybe_unused]] const char *path, [[maybe_unused]]IOMode mode) override {}

		struct sp_port *port;
		struct sp_port_config *config;
		IOMode mode;
	};
} // namespace fragcore
#endif
