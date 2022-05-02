#include "ModbusRegisterIO.h"
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>

using namespace fragcore;

ModbusRegisterIO::ModbusRegisterIO(ModbusNetSocket &socket, unsigned int address, unsigned int size)
	: socket(socket), address(address), size(size) {}
ModbusRegisterIO::~ModbusRegisterIO() {}

void ModbusRegisterIO::open(const char *path, IOMode mode) {}

void ModbusRegisterIO::close() {}

long int ModbusRegisterIO::read(long int nbytes, void *pbuffer) {
	auto rc = modbus_read_registers((modbus_t *)socket.getModbusContext(), static_cast<int>(this->address), nbytes,
									(uint16_t *)pbuffer);
	return rc;
}

long int ModbusRegisterIO::write(long int nbytes, const void *pbuffer) {
	auto rc = modbus_write_registers((modbus_t *)socket.getModbusContext(), static_cast<int>(this->address), nbytes,
									 (uint16_t *)pbuffer);
	return rc;
}

long int ModbusRegisterIO::peek(long int nBytes, void *pbuffer) { return 0; }

bool ModbusRegisterIO::eof() const { return false; }

long int ModbusRegisterIO::length() { return size; }

void ModbusRegisterIO::seek(long int nbytes, Seek seek) {}

unsigned long ModbusRegisterIO::getPos() { return this->address; }

bool ModbusRegisterIO::isWriteable() const { return true; }

bool ModbusRegisterIO::isReadable() const { return true; }

bool ModbusRegisterIO::flush() { return true; }