#include "Core/IO/SerialIO.h"
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
	sp_drain(serialPort);
	return nbytes;
}
bool SerialIO::eof(void) const { return false; }
long int SerialIO::length(void) { return 0; }
void SerialIO::seek(long int nbytes, Seek seek) { return; }
unsigned long SerialIO::getPos(void) { return 0; }
bool SerialIO::isWriteable(void) const { return true; }
bool SerialIO::isReadable(void) const { return true; }
bool SerialIO::flush(void) { return sp_flush(this->port, SP_BUF_BOTH) == SP_OK; }

void SerialIO::setBaudRate(unsigned int rate) {}
void SerialIO::setFlowControl(void) {}
void SerialIO::setParity(void) {}
void SerialIO::setXonXoff(void) {}

SerialIO::SerialIO(const std::string &path, Mode mode) {
	const int baudRate = 115200;

	sp_return res = sp_get_port_by_name(path.c_str(), (struct sp_port **)&this->port);
	if (res != SP_OK)
		throw RuntimeException("Failed get this->port {} - {}", path, sp_last_error_message());
	res = sp_open((struct sp_port *)this->port, SP_MODE_READ_WRITE);
	if (res != SP_OK)
		throw RuntimeException("Failed to open {} - {}", path, sp_last_error_message());
	res = sp_set_baudrate((struct sp_port *)this->port, baudRate);
	if (res != SP_OK)
		throw RuntimeException("Failed to set baud Rate {} - {}", baudRate, sp_last_error_message());
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