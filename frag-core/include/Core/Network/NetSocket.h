/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
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
		NetSocket(NetSocket &&other) = delete;
		~NetSocket() override;

		NetSocket &operator=(NetSocket &&other) = delete;

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

		virtual TransportProtocol getTransportProtocol() const noexcept = 0;

		/**
		 * @brief
		 *
		 * @return int
		 */
		virtual int close() = 0;

		/**
		 * @brief
		 *
		 * @param pAddr
		 * @return int
		 */
		virtual int bind(const INetAddress &pAddr) = 0;
		virtual int listen(unsigned int maxListen) = 0;
		virtual int connect(const INetAddress &pAddr) = 0;
		virtual int poll(int p_type, int timeout) const = 0;

		/**
		 * @brief
		 *
		 * @param p_buffer
		 * @param p_len
		 * @param r_read
		 * @param addr
		 * @param p_peek
		 * @return int
		 */
		virtual int recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &addr, bool p_peek = false) = 0;

		/**
		 * @brief
		 *
		 * @param pbuffer
		 * @param p_len
		 * @param sent
		 * @param peek
		 * @return int
		 */
		virtual int recv(void *pbuffer, int p_len, int &sent, bool peek = false) = 0;

		/**
		 * @brief
		 *
		 * @param p_buffer
		 * @param p_len
		 * @param r_sent
		 * @return int
		 */
		virtual int send(const uint8_t *p_buffer, int p_len, int &r_sent) = 0;
		virtual int sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) = 0;
		virtual long int send(const void *pbuffer, int p_len, int &sent) = 0;
		virtual Ref<NetSocket> accept(INetAddress &addr) = 0;
		virtual NetStatus accept(NetSocket &socket) = 0;

		virtual int read() = 0;
		virtual int write() = 0;
		virtual bool isBlocking() = 0;
		virtual void setBlocking(bool blocking) = 0;

		virtual NetStatus getStatus() const noexcept = 0;

		virtual void setTimeout(long int microsec) = 0;
		virtual long int getTimeout() = 0;

		// TODO get interaces assocated

	  protected:
		NetSocket() = default;

	  public:
		static constexpr const char *getTransportProtocolSymbol(TransportProtocol transportProtocol) noexcept;
		static constexpr const char *getNetStatusSymbol(NetStatus status) noexcept;
	};
} // namespace fragcore

#endif
