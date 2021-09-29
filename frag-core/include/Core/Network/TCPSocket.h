/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_TCP_NETSOCKET_H_
#define _FRAG_CORE_TCP_NETSOCKET_H_ 1
#include "NetSocket.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC TCPNetSocket : public NetSocket {
	  public:
		TCPNetSocket(const IPInterface &ip);
		virtual ~TCPNetSocket(void);

		// virtual Error open(Type p_type, IP::Type &ip_type) = 0;

		virtual TransportProtocol getTransportProtocol() const noexcept { return TransportProtocol::TCP; }
		virtual void close() override;

		virtual int bind(std::string &addr, unsigned int port) override;
		virtual int listen(unsigned int maxListen) override;
		virtual int connect(std::string &ip, unsigned int port) override;

		// virtual Error bind(IPAddress p_addr, uint16_t p_port) = 0;
		// virtual Error listen(int p_max_pending) = 0;
		// virtual Error connect_to_host(IPAddress p_addr, uint16_t p_port) = 0;
		// virtual Error poll(PollType p_type, int timeout) const = 0;
		// virtual Error recv(uint8_t *p_buffer, int p_len, int &r_read) = 0;
		// virtual Error recvfrom(uint8_t *p_buffer, int p_len, int &r_read, IPAddress &r_ip, uint16_t &r_port,
		// 					   bool p_peek = false) = 0;
		// virtual Error send(const uint8_t *p_buffer, int p_len, int &r_sent) = 0;
		// virtual Error sendto(const uint8_t *p_buffer, int p_len, int &r_sent, IPAddress p_ip, uint16_t p_port) =
		virtual long int send(const void *pbuffer, int p_len, int &sent) override;
		// virtual long int send(Ref<IO> &io, int len, int &sent) override;
		// 0; virtual Ref<NetSocket> accept(IPAddress &r_ip, uint16_t &r_port) = 0;
		virtual Ref<NetSocket> accept(std::string &ip, unsigned int port) override;

		/*virtual int bind(IP adddrr, Type _type);
		virtual int connect(IP& addr);
		virtual int listen(void);
		Status accept(NetSocket &socket);

		virtual int close(void);
		virtual int read(void);
		virtual int write(void);
		virtual bool isBlocking(void);
		virtual void setBlocking(bool blocking);
			*/

		// enum class Status { Done, NotReady, Partial, Disconnected, Error };
	  private:
		int socket;
	};
} // namespace fragcore

#endif
