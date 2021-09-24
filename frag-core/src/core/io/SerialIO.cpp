#include "Core/IO/SerialIO.h"
#include "Core/Math.h"
#include <libserialport.h>
#include <optional>

using namespace fragcore;

void SerialIO::close(void) {
	struct sp_port *serialPort = (struct sp_port *)this->port;
	sp_close(serialPort);
	this->port = nullptr;
}
long int SerialIO::read(long int nbytes, void *pbuffer) {
	struct sp_port *serialPort = (struct sp_port *)this->port;
	nbytes = sp_nonblocking_read(serialPort, pbuffer, nbytes);
	return nbytes;
}
long int SerialIO::write(long int nbytes, const void *pbuffer) {
	struct sp_port *serialPort = (struct sp_port *)this->port;
	nbytes = sp_nonblocking_write(serialPort, pbuffer, nbytes);
	return nbytes;
}
bool SerialIO::eof(void) const { return false; }
long int SerialIO::length(void) { return 0; }
void SerialIO::seek(long int nbytes, Seek seek) { return; }
unsigned long SerialIO::getPos(void) { return 0; }
bool SerialIO::isWriteable(void) const { return mode & IO::WRITE; }
bool SerialIO::isReadable(void) const { return mode & IO::READ; }
bool SerialIO::flush(void) { return sp_flush(this->port, SP_BUF_BOTH) == SP_OK; }

void SerialIO::setBaudRate(unsigned int baudRate) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_set_baudrate(serialPort, baudRate);
	if (res != SP_OK)
		throw RuntimeException("Failed to set baud Rate {} - {}", baudRate, sp_last_error_message());
}

SerialIO::BaudRate SerialIO::getBaudRate(void) const {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_get_config(serialPort, config);
	if (res != SP_OK)
		throw RuntimeException("Failed to get baudrate {} - {}", "path", sp_last_error_message());

	int baudRate;
	res = sp_get_config_baudrate(config, &baudRate);
	if (res != SP_OK)
		throw RuntimeException("Failed to get baudrate {} - {}", "path", sp_last_error_message());

	// sp_set_config_stopbits

	return static_cast<SerialIO::BaudRate>(baudRate);
}

void SerialIO::setFlowControl(FlowControl flowControl) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_set_flowcontrol((struct sp_port *)this->port, SP_FLOWCONTROL_NONE);
	// if (res != SP_OK)
	// 	throw RuntimeException("Failed to disable flow control {} - {}", path, sp_last_error_message());
}
void SerialIO::setParity(Parity flowControl) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_set_parity((struct sp_port *)this->port, SP_PARITY_NONE);
	// if (res != SP_OK)
	// 	throw RuntimeException("Failed to disable parity check {} - {}", path, sp_last_error_message());
}
void SerialIO::setXonXoff(XonXoff XonXoff) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	sp_return sp = sp_set_xon_xoff((struct sp_port *)this->port, SP_XONXOFF_DISABLED);
	// if (res != SP_OK)
	// 	throw RuntimeException("Failed to disable xonoff {} - {}", path, sp_last_error_message());
}

SerialIO::SerialIO(const std::string &path, IOMode mode) {
	const int baudRate = 115200;

	/*	Get SP mode from IO::Mode	*/
	sp_mode serial_mode = SP_MODE_READ;
	switch (mode) {
	case IO::IOMode::READ:
		serial_mode = SP_MODE_READ;
		break;
	case IO::IOMode::WRITE:
		serial_mode = SP_MODE_WRITE;
		break;
	case ACCESS:
		serial_mode = SP_MODE_READ_WRITE;
		break;
	default:
		assert(0);
	}

	sp_return res = sp_get_port_by_name(path.c_str(), (struct sp_port **)&this->port);
	if (res != SP_OK)
		throw RuntimeException("Failed get this->port {} - {}", path, sp_last_error_message());
	res = sp_open((struct sp_port *)this->port, SP_MODE_READ_WRITE);
	if (res != SP_OK)
		throw RuntimeException("Failed to open {} - {}", path, sp_last_error_message());

	sp_new_config(&config);

	setBaudRate(baudRate);

	res = sp_set_parity((struct sp_port *)this->port, SP_PARITY_NONE);
	if (res != SP_OK)
		throw RuntimeException("Failed to disable parity check {} - {}", path, sp_last_error_message());
	res = sp_set_flowcontrol((struct sp_port *)this->port, SP_FLOWCONTROL_NONE);
	if (res != SP_OK)
		throw RuntimeException("Failed to disable flow control {} - {}", path, sp_last_error_message());
	res = sp_set_xon_xoff((struct sp_port *)this->port, SP_XONXOFF_DISABLED);
	if (res != SP_OK)
		throw RuntimeException("Failed to disable xonoff {} - {}", path, sp_last_error_message());
}

SerialIO ::~SerialIO(void) {}

bool SerialIO::supportedBaudRate(unsigned int baudRate) {
	return baudRate == 110 || Math::IsPowerOfTwo(baudRate % (300));
}

std::optional<std::vector<std::string>> SerialIO::getSerialPorts(void) noexcept {
	int i;
	struct sp_port **ports;
	std::vector<std::string> list;
	sp_return error = sp_list_ports(&ports);
	if (error == SP_OK) {

		for (i = 0; ports[i] != nullptr; i++) {
			list.push_back(sp_get_port_name(ports[i]));
		}
		sp_free_port_list(ports);
		return {list};
	} else {
		return {};
	}
}