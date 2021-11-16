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
#ifndef _FRAG_CORE_UDP_NETSOCKET_H_
#define _FRAG_CORE_UDP_NETSOCKET_H_ 1
#include "NetSocket.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC UDPNetSocket : public NetSocket {
	  public:
		UDPNetSocket();
		virtual ~UDPNetSocket();

		virtual TransportProtocol getTransportProtocol() const noexcept override;

		virtual int close() override;

		virtual int bind(const INetAddress &p_addr) override;
		virtual int listen(unsigned int maxListen) override;
		virtual int connect(const INetAddress &p_addr) override;

		virtual int poll(int p_type, int timeout) const override;

		virtual int recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip,
							 bool p_peek = false) override;
		virtual int recv(void *pbuffer, int p_len, int &sent, bool peek = false) override;
		virtual int send(const uint8_t *p_buffer, int p_len, int &r_sent) override;
		virtual int sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) override;
		virtual long int send(const void *pbuffer, int p_len, int &sent) override;
		virtual Ref<NetSocket> accept(INetAddress &r_ip) override;
		virtual NetStatus accept(NetSocket &socket) override;

		virtual int read() override;
		virtual int write() override;
		virtual bool isBlocking() override;
		virtual void setBlocking(bool blocking) override;

		virtual NetStatus getStatus() const noexcept override;

	  protected:
		UDPNetSocket(int socket);

		bool isNetworkLayerSupported(INetAddress::NetworkProtocol protocol);
		bool isValidNetworkAddress(const INetAddress &address);

		void setTimeout(long int nanoSeconds);
		static int getDomain(const INetAddress &address);
		// static size_t setupIPAddress(sockaddr *addr, const INetAddress &p_addr, uint16_t p_port);

	  private:
		int socket;
		NetStatus netStatus;
	};
} // namespace fragcore

#endif
