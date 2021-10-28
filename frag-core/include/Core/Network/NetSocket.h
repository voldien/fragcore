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
#ifndef _FRAG_CORE_NETSOCKET_H_
#define _FRAG_CORE_NETSOCKET_H_ 1
#include "../IO/IO.h"
#include "../Ref.h"
#include "../SmartReference.h"
#include "INetAddress.h"
#include "NetInterface.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC NetSocket : public SmartReference {
	  public:
		NetSocket(const IPInterface &ip);
		virtual ~NetSocket();

		/**
		 * @brief
		 *
		 */
		enum class TransportProtocol {
			TransportProtoclNone,
			TransportProtocolTCP,
			TransportProtocolUDP,
			TransportProtocolCAN,
			MaxTransportProtocol
		};
		enum class NetStatus { Status_Done, Status_NotReady, Status_Partial, Status_Disconnected, Status_Error };

		virtual int open(int p_type, int ip_type) = 0;

		virtual TransportProtocol getTransportProtocol() const noexcept = 0;

		virtual int close() = 0;
		virtual int bind(std::string &addr, unsigned int port) = 0;
		virtual int bind(const INetAddress &p_addr, uint16_t p_port) = 0;
		virtual int listen(unsigned int maxListen) = 0;
		virtual int connect(std::string &ip, unsigned int port) = 0;
		virtual int connect(const INetAddress &p_addr, uint16_t p_port) = 0;
		virtual int poll(int p_type, int timeout) const = 0;

		virtual int recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &r_ip, uint16_t &r_port,
							 bool p_peek = false) = 0;
		virtual int recv(const void *pbuffer, int p_len, int &sent) = 0;
		virtual int send(const uint8_t *p_buffer, int p_len, int &r_sent) = 0;
		virtual int sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip,
						   uint16_t p_port) = 0;
		virtual long int send(const void *pbuffer, int p_len, int &sent) = 0;
		virtual Ref<NetSocket> accept(INetAddress &r_ip, uint16_t &r_port) = 0;
		virtual Ref<NetSocket> accept(std::string &ip, unsigned int port) = 0;
		virtual NetStatus accept(NetSocket &socket) = 0;

		virtual int read() = 0;
		virtual int write() = 0;
		virtual bool isBlocking() = 0;
		virtual void setBlocking(bool blocking) = 0;

		virtual NetStatus getStatus() const noexcept = 0;

		// virtual Error listen(int p_max_pending) = 0;
		// virtual Error connect_to_host(IPAddress p_addr, uint16_t p_port) = 0;
		// virtual Error poll(PollType p_type, int timeout) const = 0;
		// virtual Error recv(uint8_t *p_buffer, int p_len, int &r_read) = 0;

		/*virtual int bind(IP adddrr, Type _type);
		virtual int connect(IP& addr);
		virtual int listen();

			*/

	  protected:
		NetSocket() = default;

	  public:
		static constexpr const char *getTransportProtocolSymbol(TransportProtocol transportProtocol) noexcept;
		static constexpr const char *getNetStatusSymbol(NetStatus status) noexcept;
	};
} // namespace fragcore

#endif
