
#include "NetModbusSocket.h"
#include "Core/Network/TCPUDPAddress.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus-tcp.h>
#include <modbus/modbus.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using namespace fragcore;

ModbusNetSocket::ModbusNetSocket() {}
ModbusNetSocket::ModbusNetSocket(int socket) : TCPNetSocket(socket) {}
ModbusNetSocket::~ModbusNetSocket() { this->close(); }

ModbusNetSocket::TransportProtocol ModbusNetSocket::getTransportProtocol() const noexcept {
	return ModbusNetSocket::TransportProtocol::TransportProtoclNone;
}

int ModbusNetSocket::close() {
	if (this->ctx) {
		modbus_close((modbus_t *)this->ctx);
		modbus_free((modbus_t *)this->ctx);
	}
	this->socket = 0;
	netStatus = NetStatus::Status_Disconnected;
}

// int ModbusNetSocket::bind(std::string &IPaddr, unsigned int port) {
// 	if (ctx == nullptr)
// 		ctx = modbus_new_tcp(IPaddr.c_str(), port);
// }

int ModbusNetSocket::bind(const INetAddress &p_addr) {
	// if (this->ctx == nullptr)
	// 	this->ctx = modbus_new_tcp(IPaddr.c_str(), p_port);

	// modbus_set_debug(ctx, 1);
	if (modbus_set_error_recovery((modbus_t *)this->ctx,
								  static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_PROTOCOL)) == -1) {
	}
	this->socket = modbus_get_socket((modbus_t *)this->ctx);
}

int ModbusNetSocket::listen(unsigned int maxListen) {
	if (modbus_tcp_listen((modbus_t *)this->ctx, maxListen) < 0) {
		SystemException ex(errno, std::system_category(), "TCP socket: Failed to set listen {} ", maxListen);
	}
	return 0;
}
// int ModbusNetSocket::connect(std::string &ip, unsigned int port) {
// 	// return connect(IPAddress(ip, IPAddress::IPAddressType::IPAddress_Type_IPV4), port);
// }

int ModbusNetSocket::connect(const INetAddress &p_addr) {
	uint32_t old_response_to_sec;
	uint32_t old_response_to_usec;

	const TCPUDPAddress &tcpAddress = static_cast<const TCPUDPAddress &>(p_addr);
	if (this->ctx == nullptr) {
		this->ctx = modbus_new_tcp(nullptr, tcpAddress.getPort());
		if (this->ctx == nullptr)
			throw RuntimeException("{}", modbus_strerror(errno));
	}
	modbus_get_response_timeout((modbus_t *)this->ctx, &old_response_to_sec, &old_response_to_usec);

	if (modbus_set_error_recovery((modbus_t *)this->ctx,
								  static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_PROTOCOL)) == -1) {
		throw RuntimeException("Failed to set Recovery Mode {}", modbus_strerror(errno));
	}

	TCPNetSocket::connect(p_addr);
	this->socket = modbus_set_socket((modbus_t *)this->ctx, this->socket);

	this->netStatus = NetStatus::Status_Done;
}

int ModbusNetSocket::poll(int p_type, int timeout) const {}
int ModbusNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, bool p_peek) {
	int flag = 0;
	int res; //= recvfrom(this->socket, p_buffer, p_len, flag, connection->intaddr, &r_read);
	return res;
}
int ModbusNetSocket::recv( void *pbuffer, int p_len, int &sent, bool peek) {
	int flag = 0;
	int res = modbus_read_registers((modbus_t *)this->ctx, 0, p_len, (uint16_t *)pbuffer);
	if (res != p_len) {
		throw RuntimeException(" Failed to read Register {}", modbus_strerror(errno));
	}
	return res;
}
int ModbusNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {
	int flag = 0;
	int res = modbus_write_registers((modbus_t *)this->ctx, 0, p_len, (const uint16_t *)p_buffer);
	if (res != p_len) {
		throw RuntimeException(" Failed to write Register {}", modbus_strerror(errno));
	}
	return res;
}

int ModbusNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) {
	int res; //=  sendto(this->socket, p_buffer, p_len, flag, connection->extaddr, connection->sclen);
	return res;
}

long int ModbusNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	long int res = ::send(this->socket, pbuffer, p_len, flag);
	return res;
}

Ref<NetSocket> ModbusNetSocket::accept(INetAddress &r_ip) { return TCPNetSocket::accept(r_ip); }

ModbusNetSocket::NetStatus ModbusNetSocket::accept(NetSocket &socket) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
	}
}
int ModbusNetSocket::read() {}
int ModbusNetSocket::write() {}
bool ModbusNetSocket::isBlocking() { /*	*/
}
void ModbusNetSocket::setBlocking(bool blocking) { /*	*/
												   // modbus_set_response_timeout()
}
ModbusNetSocket::NetStatus ModbusNetSocket::getStatus() const noexcept { return this->netStatus; }
