
#include "NetModbusSocket.h"
#include "Core/Network/TCPSocket.h"
#include "Core/Network/TCPUDPAddress.h"
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace fragcore;

ModbusNetSocket::ModbusNetSocket() : ctx(nullptr) {}
ModbusNetSocket::ModbusNetSocket(int socket) : TCPNetSocket(socket), ctx(nullptr) {

	if (this->ctx == nullptr) {
		this->ctx = modbus_new_tcp(nullptr, getPort());
		if (this->ctx == nullptr) {
			throw RuntimeException("Failed to create empty: {}", modbus_strerror(errno));
		}
	}
	/*	*/
	int rcode = modbus_set_socket(static_cast<modbus_t *>(this->ctx), this->getSocket());
	if (rcode == -1) {
		throw RuntimeException("Failed to set Socket: {}", modbus_strerror(errno));
	}

	/*	*/
	rcode = modbus_set_debug(static_cast<modbus_t *>(this->ctx), 0);
	if (rcode == -1) {
		throw RuntimeException("Failed to set Debug: {}", modbus_strerror(errno));
	}

	/*	*/
	if (modbus_set_error_recovery(static_cast<modbus_t *>(this->ctx),
								  static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_PROTOCOL)) == -1) {
		/*	*/
		throw RuntimeException("Failed to set Error Recovery: {}", modbus_strerror(errno));
	}
}
ModbusNetSocket::~ModbusNetSocket() { this->close(); }

ModbusNetSocket::TransportProtocol ModbusNetSocket::getTransportProtocol() const noexcept {
	return ModbusNetSocket::TransportProtocol::TransportProtoclNone;
}

int ModbusNetSocket::close() {

	/*	*/
	if (this->ctx != nullptr) {
		TCPNetSocket::close();
		modbus_free((modbus_t *)this->ctx);
	}
	/*	Reset variables.	*/
	this->socket = 0;
	this->ctx = nullptr;
	netStatus = NetStatus::Status_Disconnected;
	return 0;
}

int ModbusNetSocket::bind(const INetAddress &p_addr) {

	/*	*/
	const TCPUDPAddress &tcpAddress = static_cast<const TCPUDPAddress &>(p_addr);
	if (this->ctx == nullptr) {

		this->ctx = modbus_new_tcp(nullptr, tcpAddress.getPort());

		if (this->ctx == nullptr) {
			throw RuntimeException("{}", modbus_strerror(errno));
		}
	}

	/*	*/
	TCPNetSocket::bind(tcpAddress);

	/*	*/
	int rcode = modbus_set_socket(static_cast<modbus_t *>(this->ctx), this->socket);
	if (rcode == -1) {
		throw RuntimeException("{}", modbus_strerror(errno));
	}

	/*	*/
	rcode = modbus_set_debug(static_cast<modbus_t *>(this->ctx), 0);
	if (rcode == -1) {
		throw RuntimeException("{}", modbus_strerror(errno));
	}

	/*	*/
	if (modbus_set_error_recovery(static_cast<modbus_t *>(this->ctx),
								  static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_PROTOCOL)) == -1) {
		/*	*/
		throw RuntimeException("{}", modbus_strerror(errno));
	}
	return 0;
}

int ModbusNetSocket::listen(unsigned int maxListen) { return TCPNetSocket::listen(maxListen); }

int ModbusNetSocket::connect(const INetAddress &addr) {
	uint32_t old_response_to_sec;
	uint32_t old_response_to_usec;

	/*	*/
	if (this->ctx == nullptr) {
		this->ctx = modbus_new_tcp(nullptr, 0);
		if (this->ctx == nullptr) {
			throw RuntimeException("Failed to create ModbusTCP Context: {}", modbus_strerror(errno));
		}
	}

	/*	Connect over the transport layer.	*/
	TCPNetSocket::connect(addr);

	/*	*/
	int rcode = modbus_set_socket(static_cast<modbus_t *>(this->ctx), this->socket);
	if (rcode == -1) {
		throw RuntimeException("Failed to set Socket: {}", modbus_strerror(errno));
	}

	/*	*/
	rcode =
		modbus_get_response_timeout(static_cast<modbus_t *>(this->ctx), &old_response_to_sec, &old_response_to_usec);
	if (rcode == -1) {
		throw RuntimeException("Failed to set response timeout Mode: {}", modbus_strerror(errno));
	}

	/*	*/
	rcode = modbus_set_error_recovery(static_cast<modbus_t *>(this->ctx),
									  static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_PROTOCOL));
	if (rcode == -1) {
		throw RuntimeException("Failed to set Recovery Mode: {}", modbus_strerror(errno));
	}

	/*	*/
	this->netStatus = NetStatus::Status_Done;
	return 0;
}

int ModbusNetSocket::poll(int p_type, int timeout) const { return 0; }
int ModbusNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) {

	int res = 0; //= recvfrom(this->socket, p_buffer, p_len, flag, connection->intaddr, &r_read);
	return res;
}

int ModbusNetSocket::recv(void *pbuffer, int p_len, int &sent, bool peek) {

	int res = modbus_read_registers((modbus_t *)this->ctx, 0, p_len, (uint16_t *)pbuffer);
	if (res == -1) {
		throw RuntimeException(" Failed to read Register {}", modbus_strerror(errno));
	}
	return res;
}

int ModbusNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {

	int res = modbus_write_registers((modbus_t *)this->ctx, 0, p_len, (const uint16_t *)p_buffer);
	if (res == -1) {
		throw RuntimeException(" Failed to write Register {}", modbus_strerror(errno));
	}
	return res;
}

int ModbusNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) {
	int res = 0; //=  sendto(this->socket, p_buffer, p_len, flag, connection->extaddr, connection->sclen);
	return res;
}

long int ModbusNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	long int res = ::send(this->socket, pbuffer, p_len, flag);
	return res;
}

Ref<NetSocket> ModbusNetSocket::accept(INetAddress &r_ip) {
	Ref<NetSocket> base_socket = TCPNetSocket::accept(r_ip);

	TCPNetSocket *tcp_net_socket = static_cast<TCPNetSocket *>(*base_socket);

	ModbusNetSocket *_newsocket = new ModbusNetSocket(tcp_net_socket->getSocket());
	/*	*/
	return Ref<NetSocket>(_newsocket);
}

ModbusNetSocket::NetStatus ModbusNetSocket::accept(NetSocket &socket) { return TCPNetSocket::accept(socket); }
int ModbusNetSocket::read() { return 0; }
int ModbusNetSocket::write() { return 0; }
bool ModbusNetSocket::isBlocking() { /*	*/
	return TCPNetSocket::isBlocking();
}

void ModbusNetSocket::setBlocking(bool blocking) { /*	*/
	TCPNetSocket::setBlocking(blocking);
}

int ModbusNetSocket::writeRegister(unsigned int address, unsigned int nWords, void *pdata) {
	auto rcode = modbus_write_registers((modbus_t *)this->getModbusContext(), static_cast<int>(address), nWords,
										(uint16_t *)pdata);
	if (rcode == -1) {
		throw RuntimeException(" Failed to write Register {}", modbus_strerror(errno));
	}
	return rcode;
}
int ModbusNetSocket::readRegister(unsigned int address, unsigned int nWords, void *pdata) {
	auto rcode = modbus_read_registers((modbus_t *)this->getModbusContext(), static_cast<int>(address), nWords,
									   (uint16_t *)pdata);
	if (rcode == -1) {
		throw RuntimeException(" Failed to read Register {}", modbus_strerror(errno));
	}
	return rcode;
}

ModbusNetSocket::NetStatus ModbusNetSocket::getStatus() const noexcept { return this->netStatus; }
