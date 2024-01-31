#include "SerialIO.h"
#include <libserialport.h>
#include <optional>

using namespace fragcore;

static const char *get_flow_control_symbol(SerialIO::FlowControl flowControl) {
	switch (flowControl) {
	case SerialIO::FlowControl::FlowControlNone:
		return "None";
	case SerialIO::FlowControl::FlowControlXonXoff:
		return "XonXoff";
	case SerialIO::FlowControl::FlowControlRtsCts:
		return "RtsCts";
	case SerialIO::FlowControl::FlowControlDtrDsr:
		return "DrtDsr";
	default:
		return "Unknown";
	}
}

static const char *get_parity_symbol(SerialIO::Parity parity) {
	switch (parity) {
	case SerialIO::Parity::ParityNone:
		return "None";
	case SerialIO::Parity::ParityOdd:
		return "Odd";
	case SerialIO::Parity::ParityEven:
		return "Even";
	case SerialIO::Parity::ParityMark:
		return "Mark";
	case SerialIO::Parity::ParitySpace:
		return "Space";
	default:
		return "Unknown";
	}
}

static const char *get_xonxoff_symbol(SerialIO::XonXoff XonXoff) {
	switch (XonXoff) {
	case SerialIO::XonXoff::XonXoffDisable:
		return "Disable";
	case SerialIO::XonXoff::XonXoffIn:
		return "In";
	case SerialIO::XonXoff::XonXoffOut:
		return "Out";
	case SerialIO::XonXoff::XonXoffInOut:
		return "In-Out";
	default:
		return "Unknown";
	}
}

void SerialIO::close() {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	if (serialPort != nullptr) {
		sp_return res = sp_close(serialPort);
		if (res != SP_OK) {
			throw RuntimeException("Failed to close serial: {} ", sp_last_error_message());
		}
	}
}

long int SerialIO::read(long int nbytes, void *pbuffer) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	sp_return res_in_read_bytes = sp_nonblocking_read(serialPort, pbuffer, nbytes);
	return res_in_read_bytes;
}

long int SerialIO::write(long int nbytes, const void *pbuffer) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	sp_return res_in_written_bytes = sp_nonblocking_write(serialPort, pbuffer, nbytes);
	return res_in_written_bytes;
}

long int SerialIO::peek([[maybe_unused]] long int nBytes, [[maybe_unused]] void *pbuffer) { return 0; }

bool SerialIO::eof() const { return false; }
long int SerialIO::length() { return 0; }
void SerialIO::seek([[maybe_unused]] long int nbytes, [[maybe_unused]] const Seek seek) {}
unsigned long SerialIO::getPos() { return 0; }
bool SerialIO::isWriteable() const { return mode & IO::WRITE; }
bool SerialIO::isReadable() const { return mode & IO::READ; }
bool SerialIO::flush() {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	return sp_flush(serialPort, SP_BUF_BOTH) == SP_OK;
}

void SerialIO::setBaudRate(unsigned int baudRate) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}
	res = sp_set_config_baudrate(this->config, baudRate);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set baud Rate {} - {} ({})", baudRate, sp_last_error_message(), res);
	}

	res = sp_set_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set config: {} ({})", sp_last_error_message(), res);
	}
}

SerialIO::BaudRate SerialIO::getBaudRate() const {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	int baudRate;
	res = sp_get_config_baudrate(config, &baudRate);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get baudrate {} - {}", "path", sp_last_error_message());
	}

	return static_cast<SerialIO::BaudRate>(baudRate);
}

void SerialIO::setStopBits(StopBits stopBits) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}
	res = sp_set_config_stopbits(config, static_cast<int>(stopBits));
	if (res != SP_OK) {
		throw RuntimeException("Failed to set StopBits {} - {}", "path", sp_last_error_message());
	}

	res = sp_set_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set config: {} ({})", sp_last_error_message(), res);
	}
}

SerialIO::StopBits SerialIO::getStopBits() const {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	int stopBit;

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	res = sp_get_config_stopbits(config, &stopBit);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get StopBits {} - {}", "path", sp_last_error_message());
	}

	return static_cast<SerialIO::StopBits>(stopBit);
}

void SerialIO::setFlowControl(FlowControl flowControl) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	/*	Convert SerialIO enum to SP enum.	*/
	enum sp_flowcontrol sp_flowcontrol;
	switch (flowControl) {
	case FlowControl::FlowControlNone:
		sp_flowcontrol = SP_FLOWCONTROL_NONE;
		break;
	case FlowControl::FlowControlXonXoff:
		sp_flowcontrol = SP_FLOWCONTROL_XONXOFF;
		break;
	case FlowControl::FlowControlRtsCts:
		sp_flowcontrol = SP_FLOWCONTROL_RTSCTS;
		break;
	case FlowControl::FlowControlDtrDsr:
		sp_flowcontrol = SP_FLOWCONTROL_DTRDSR;
		break;
	default:
		throw InvalidArgumentException("Invalid FlowControl {}", get_flow_control_symbol(flowControl));
	}

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}
	res = sp_set_config_flowcontrol(config, sp_flowcontrol);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set flow control {} - {}", get_flow_control_symbol(flowControl),
							   sp_last_error_message());
	}

	res = sp_set_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set config: {} ({})", sp_last_error_message(), res);
	}
}

SerialIO::FlowControl SerialIO::getFlowControl() const {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {}", sp_last_error_message());
	}
	throw NotImplementedException();
}

void SerialIO::setParity(Parity parity) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	enum sp_parity sp_parity = SP_PARITY_NONE;
	switch (parity) {
	case Parity::ParityNone:
		sp_parity = SP_PARITY_NONE;
		break;
	case Parity::ParityOdd:
		sp_parity = SP_PARITY_ODD;
		break;
	case Parity::ParityEven:
		sp_parity = SP_PARITY_EVEN;
		break;
	case Parity::ParityMark:
		sp_parity = SP_PARITY_MARK;
		break;
	case Parity::ParitySpace:
		sp_parity = SP_PARITY_SPACE;
		break;
	default:
		throw InvalidArgumentException("Invalid Parity: {}", get_parity_symbol(parity));
	}

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	res = sp_set_config_parity(config, sp_parity);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set parity: {} ({})", sp_last_error_message(), res);
	}

	res = sp_set_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set config: {} ({})", sp_last_error_message(), res);
	}
}
SerialIO::Parity SerialIO::getParity() const {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	sp_parity parity;
	res = sp_get_config_parity(config, &parity);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get parity {} ({})", sp_last_error_message(), res);
	}

	return static_cast<SerialIO::Parity>(parity);
}

void SerialIO::setXonXoff(XonXoff XonXoff) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	enum sp_xonxoff sp_xonoxoff;
	switch (XonXoff) {
	case XonXoff::XonXoffDisable:
		sp_xonoxoff = SP_XONXOFF_DISABLED;
		break;
	case XonXoff::XonXoffIn:
		sp_xonoxoff = SP_XONXOFF_IN;
		break;
	case XonXoff::XonXoffOut:
		sp_xonoxoff = SP_XONXOFF_OUT;
		break;
	case XonXoff::XonXoffInOut:
		sp_xonoxoff = SP_XONXOFF_INOUT;
		break;
	default:
		throw InvalidArgumentException("Invalid XonXoff: {}", get_xonxoff_symbol(XonXoff));
	}

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	res = sp_set_config_xon_xoff(config, sp_xonoxoff);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set xonoff {} - {}", "path", sp_last_error_message());
	}

	res = sp_set_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set config: {} ({})", sp_last_error_message(), res);
	}
}

SerialIO::XonXoff SerialIO::getXonXoff() {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	sp_xonxoff xonxoff;
	res = sp_get_config_xon_xoff(config, &xonxoff);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config xon xoff: {}", sp_last_error_message());
	}

	return static_cast<SerialIO::XonXoff>(xonxoff);
}

void SerialIO::setPayloadBits(unsigned int nrBits) {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);

	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	res = sp_set_config_bits(config, nrBits);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set payload: {}", sp_last_error_message());
	}

	res = sp_set_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to set config: {} ({})", sp_last_error_message(), res);
	}
}

int SerialIO::getPayloadBits() const {
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	sp_return res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}
	int nrBits;
	res = sp_get_config_bits(config, &nrBits);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config payload bits: {}", sp_last_error_message());
	}

	return nrBits;
}

SerialIO::SerialIO(const std::string &path, IOMode mode) : port(nullptr) {

	/*	Get SP mode from IO::Mode	*/
	sp_mode serial_mode = SP_MODE_READ;
	switch (mode) {
	case IO::IOMode::READ:
		serial_mode = SP_MODE_READ;
		break;
	case IO::IOMode::WRITE:
		serial_mode = SP_MODE_WRITE;
		break;
	case IO::IOMode::ACCESS:
		serial_mode = SP_MODE_READ_WRITE;
		break;
	default:
		assert(0);
		throw InvalidArgumentException("Invalid IO Mode: {} - {}", path, mode);
	}

	/*	*/
	struct sp_port *serialPort = static_cast<struct sp_port *>(this->port);
	sp_return res = sp_get_port_by_name(path.c_str(), &serialPort);
	if (res != SP_OK) {
		this->close();
		throw RuntimeException("Failed get this->port {} - {}  ({})", path, sp_last_error_message(), res);
	}
	this->port = serialPort;

	/*	*/
	res = sp_open(serialPort, serial_mode);
	if (res != SP_OK) {
		throw RuntimeException("Failed to open {} in mode: {} - {}  ({})", path, (int)mode, sp_last_error_message(),
							   res);
	}

	res = sp_set_baudrate(serialPort, 4800);
	res = sp_set_bits(serialPort, 8);
	res = sp_set_parity(serialPort, SP_PARITY_NONE);
	res = sp_set_stopbits(serialPort, 1);
	res = sp_set_flowcontrol(serialPort, SP_FLOWCONTROL_NONE);

	res = sp_new_config(&this->config);
	if (res != SP_OK) {
		this->close();
		throw RuntimeException("Failed to create config: {}  ({})", sp_last_error_message(), res);
	}

	res = sp_get_config(serialPort, this->config);
	if (res != SP_OK) {
		throw RuntimeException("Failed to get config: {} ({})", sp_last_error_message(), res);
	}

	res = sp_set_config(serialPort, this->config);
	if (res != SP_OK) {
		this->close();
		throw RuntimeException("Failed to set config: {} ({})", sp_last_error_message(), res);
	}

	this->mode = mode;
}

SerialIO ::~SerialIO() {
	this->close();
	sp_free_config(this->config);
	this->config = nullptr;
	if (this->port) {
		sp_free_port(this->port);
	}
}

std::optional<std::vector<std::string>> SerialIO::getSerialPorts() {

	struct sp_port **ports;
	std::vector<std::string> list;
	sp_return error = sp_list_ports(&ports);

	if (error == SP_OK) {

		for (int i = 0; ports[i] != nullptr; i++) {
			list.emplace_back(sp_get_port_name(ports[i]));
		}
		sp_free_port_list(ports);
		return {list};
	}
	return {};
}