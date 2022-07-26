#include "ModbusRegisterIO.h"
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>

using namespace fragcore;

ModbusRegisterIO::ModbusRegisterIO(ModbusNetSocket &socket, size_t address_start, size_t size)
	: socket(socket), address_start(address_start), size(size) {}
ModbusRegisterIO::~ModbusRegisterIO() {}

void ModbusRegisterIO::open(const char *path, IOMode mode) {}

void ModbusRegisterIO::close() {}

long int ModbusRegisterIO::read(long int nbytes, void *pbuffer) {
	/*	*/
	auto rc = modbus_read_registers((modbus_t *)socket.getModbusContext(),
									static_cast<int>(this->address_start + seek_offset), nbytes, (uint16_t *)pbuffer);
	if (rc == -1) {
		throw RuntimeException("Failed to read register {} - size {} : {}", address_start, nbytes,
							   modbus_strerror(errno));
	}
	return rc;
}

long int ModbusRegisterIO::write(long int nbytes, const void *pbuffer) {
	auto rc = modbus_write_registers((modbus_t *)socket.getModbusContext(),
									 static_cast<int>(this->address_start + seek_offset), nbytes, (uint16_t *)pbuffer);
	if (rc == -1) {
		throw RuntimeException("Failed to write register {} - size {} : {}", address_start, nbytes,
							   modbus_strerror(errno));
	}
	return rc;
}

long int ModbusRegisterIO::peek(long int nBytes, void *pbuffer) { return 0; }

bool ModbusRegisterIO::eof() const { return false; }

long int ModbusRegisterIO::length() { return this->size; }

void ModbusRegisterIO::seek(long int nbytes, Seek seek) {

	switch (seek) {
	case SET:
		seek_offset = nbytes;
		break;
	case CUR:
		seek_offset += nbytes;
		break;
	case END:
		seek_offset += nbytes;
		break;
	default:
		throw InvalidArgumentException("Invalid seek enumerator.");
	}
}

unsigned long ModbusRegisterIO::getPos() { return this->address_start; }

bool ModbusRegisterIO::isWriteable() const { return true; }

bool ModbusRegisterIO::isReadable() const { return true; }

bool ModbusRegisterIO::flush() {

	int rc = modbus_flush(reinterpret_cast<modbus_t *>(this->socket.getModbusContext()));
	if (rc == -1) {
		return false;
	}
	return true;
}