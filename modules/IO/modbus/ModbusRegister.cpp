#include "ModbusRegisterIO.h"
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>

using namespace fragcore;

ModbusRegisterIO::ModbusRegisterIO(ModbusNetSocket &socket, const size_t read_address_start,
								   const size_t read_size_bytes, const size_t write_address_start,
								   const size_t write_size_bytes)
	: socket(socket), address_write_start(write_address_start), write_size(write_size_bytes),
	  read_size(read_size_bytes), address_read_start(read_address_start) {}

ModbusRegisterIO::~ModbusRegisterIO() {}

void ModbusRegisterIO::open(const char *path, IOMode mode) {}

void ModbusRegisterIO::close() {}

long int ModbusRegisterIO::read(long int nbytes, void *pbuffer) {
	/*	*/
	auto rcode = modbus_read_registers((modbus_t *)socket.getModbusContext(),
									   static_cast<int>(this->address_read_start + seek_read_offset), nbytes,
									   (uint16_t *)pbuffer);
	if (rcode == -1) {
		throw RuntimeException("Failed to read register {} - size {} : {}", address_write_start, nbytes,
							   modbus_strerror(errno));
	}
	return rcode;
}

long int ModbusRegisterIO::write(long int nbytes, const void *pbuffer) {
	auto rcode = modbus_write_registers((modbus_t *)socket.getModbusContext(),
										static_cast<int>(this->address_write_start + seek_write_offset), nbytes,
										(uint16_t *)pbuffer);
	if (rcode == -1) {
		throw RuntimeException("Failed to write register {} - size {} : {}", address_write_start, nbytes,
							   modbus_strerror(errno));
	}
	return rcode;
}

long int ModbusRegisterIO::peek(long int nBytes, void *pbuffer) { return 0; }

bool ModbusRegisterIO::eof() const { return false; }

long int ModbusRegisterIO::length() { return this->write_size; }

void ModbusRegisterIO::seek(long int nbytes, const Seek seek) {

	switch (seek) {
	case SET:
		seek_write_offset = nbytes;
		break;
	case CUR:
		seek_write_offset += nbytes;
		break;
	case END:
		seek_write_offset += nbytes;
		break;
	default:
		throw InvalidArgumentException("Invalid seek enumerator.");
	}
}

unsigned long ModbusRegisterIO::getPos() { return this->address_write_start; }

bool ModbusRegisterIO::isWriteable() const { return true; }

bool ModbusRegisterIO::isReadable() const { return true; }

bool ModbusRegisterIO::flush() {

	int rcode = modbus_flush(reinterpret_cast<modbus_t *>(this->socket.getModbusContext()));
	if (rcode == -1) {
		return false;
	}
	return true;
}