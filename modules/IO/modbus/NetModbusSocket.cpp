
#include "NetModbusSocket.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
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

ModbusNetSocket::ModbusNetSocket() : netStatus(NetStatus::Status_Disconnected), socket(0) {}
ModbusNetSocket::ModbusNetSocket(int socket) : socket(socket) {}
ModbusNetSocket::~ModbusNetSocket() { this->close(); }

ModbusNetSocket::TransportProtocol ModbusNetSocket::getTransportProtocol() const noexcept {
	return ModbusNetSocket::TransportProtocol::TransportProtoclNone;
}

int ModbusNetSocket::close() {
	modbus_close((modbus_t *)ctx);
	modbus_free((modbus_t *)ctx);
	int status = ::close(this->socket);
	if (status != 0)
		throw RuntimeException("{}", strerror(errno));
	this->socket = 0;
	netStatus = NetStatus::Status_Disconnected;
}

int ModbusNetSocket::bind(std::string &IPaddr, unsigned int port) {
	if (ctx == nullptr)
		ctx = modbus_new_tcp(IPaddr.c_str(), port);
}

int ModbusNetSocket::bind(const INetAddress &p_addr, uint16_t p_port) {
	// if (this->ctx == nullptr)
	// 	this->ctx = modbus_new_tcp(IPaddr.c_str(), p_port);

	// modbus_set_debug(ctx, 1);
	if (modbus_set_error_recovery((modbus_t *)this->ctx,
								  static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_PROTOCOL)) == -1) {
	}
}

int ModbusNetSocket::listen(unsigned int maxListen) {
	if (::listen(this->socket, maxListen) < 0) {
		SystemException ex(errno, "TCP socket: Failed to set listen {} - error: {}", maxListen, strerror(errno));
	}
	return 0;
}
int ModbusNetSocket::connect(std::string &ip, unsigned int port) {
	// return connect(IPAddress(ip, IPAddress::IPAddressType::IPAddress_Type_IPV4), port);
}

int ModbusNetSocket::connect(const INetAddress &p_addr, uint16_t p_port) {

	// if (ctx == nullptr)
	// 	ctx = modbus_new_tcp(p_addr. c_str(), p_port);

	if (modbus_connect((modbus_t *)ctx) != 0) {
	};
	this->netStatus = NetStatus::Status_Done;
}

int ModbusNetSocket::open(int p_type, int ip_type) {}

int ModbusNetSocket::poll(int p_type, int timeout) const {}
int ModbusNetSocket::recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, uint16_t &r_port,
							  bool p_peek) {
	int flag = 0;
	int res; //= recvfrom(this->socket, p_buffer, p_len, flag, connection->intaddr, &r_read);
	return res;
}
int ModbusNetSocket::recv(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	int res = ::recv(this->socket, (void *)pbuffer, p_len, flag);
	return res;
}
int ModbusNetSocket::send(const uint8_t *p_buffer, int p_len, int &r_sent) {
	int flag = 0;
	int res = ::send(this->socket, p_buffer, p_len, flag);
	return res;
}
int ModbusNetSocket::sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip, uint16_t p_port) {
	int res; //=  sendto(this->socket, p_buffer, p_len, flag, connection->extaddr, connection->sclen);
	return res;
}

long int ModbusNetSocket::send(const void *pbuffer, int p_len, int &sent) {
	int flag = 0;
	long int res = ::send(this->socket, pbuffer, p_len, flag);
	return res;
}

Ref<NetSocket> ModbusNetSocket::accept(INetAddress &r_ip, uint16_t &r_port) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		// close();
	}
	ModbusNetSocket *_newsocket = new ModbusNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}

Ref<NetSocket> ModbusNetSocket::accept(std::string &ip, unsigned int port) {
	struct sockaddr tobuffer; /*	*/
	socklen_t aclen = 0;	  /*	*/
	int aaccept_socket = ::accept(this->socket, &tobuffer, &aclen);
	if (aaccept_socket < 0) {
		// close();
	}
	ModbusNetSocket *_newsocket = new ModbusNetSocket(aaccept_socket);
	return Ref<NetSocket>(_newsocket);
}
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
}
ModbusNetSocket::NetStatus ModbusNetSocket::getStatus() const noexcept { return this->netStatus; }

// int ModbusNetSocket::getDomain(const INetAddress &address) {

// 	int domain = 0; // TODO be override by the NetAddress!
// 	switch (address.getNetworkProtocol()) {
// 	case INetAddress::NetworkProtocol::NetWorkProtocol_IP: {
// 		const IPAddress &ipAddress = static_cast<const IPAddress &>(address);
// 		if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV4)
// 			return AF_INET;
// 		else if (ipAddress.getIPType() == IPAddress::IPAddressType::IPAddress_Type_IPV6)
// 			return AF_INET6;
// 	} break;
// 	case INetAddress::NetworkProtocol::NetWorkProtocol_CAN:
// 		return AF_CAN;
// 		break;
// 	default:
// 		throw RuntimeException("Non Supported Network Protocol: {}", address.getNetworkProtocol());
// 	}
// }