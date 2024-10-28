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
#ifndef _FRAGCORE_SERIAL_FILE_IO_H_
#define _FRAGCORE_SERIAL_FILE_IO_H_ 1
#include <IO/IO.h>
#include <libserialport.h>
#include <optional>
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC SerialIO : public IO {
	  public:
		SerialIO(const std::string &path, IOMode mode);
		~SerialIO() override;

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
			const IOOperation supportedIO = static_cast<IOOperation>(OP_READ | OP_WRITE | OP_FLUSH);
			return (operations & supportedIO) != operations;
		};

	  public:
		/**
		 * @brief
		 *
		 */
		enum class FlowControl {
			FlowControlNone = 0,
			FlowControlXonXoff = 1,
			FlowControlRtsCts = 2,
			FlowControlDtrDsr = 3
		};

		/**
		 * @brief
		 *
		 */
		enum class Parity { ParityNone, ParityOdd, ParityEven, ParityMark, ParitySpace };

		/**
		 * @brief
		 *
		 */
		enum class StopBits { ONEBIT = 1, TWOBIT = 2 };

		/**
		 * @brief
		 *
		 */
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

		void setParity(Parity parity);
		Parity getParity() const;

		void setXonXoff(XonXoff XonXoff);
		XonXoff getXonXoff();

		/**
		 * @brief Set the number of bits for each payload.
		 *
		 * @param nrBits
		 */
		void setPayloadBits(unsigned int nrBits);

		/**
		 * @brief Get the number of bits for each payload.
		 *
		 * @return int
		 */
		int getPayloadBits() const;

	  public:
		/**
		 * @brief
		 *
		 * @param baudRate
		 * @return true
		 * @return false
		 */
		static bool supportedBaudRate(unsigned int baudRate);

		/**
		 * @brief Get the Serial Ports object
		 *
		 * @return std::optional<std::vector<std::string>>
		 */
		static std::optional<std::vector<std::string>> getSerialPorts();

	  private:
		void open([[maybe_unused]] const char *path, [[maybe_unused]] IOMode mode) override {}

		struct sp_port *port;
		struct sp_port_config *config;
		IOMode mode;
	};
} // namespace fragcore
#endif
