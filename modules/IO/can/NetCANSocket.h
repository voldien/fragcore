/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAG_CORE_CAN_NETSOCKET_H_
#define _FRAG_CORE_CAN_NETSOCKET_H_ 1
#include "CANAddress.h"
#include <Network/NetSocket.h>
#include <sys/socket.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC CANNetSocket : public NetSocket {
	  public:
		CANNetSocket();
		~CANNetSocket() override;

		TransportProtocol getTransportProtocol() const noexcept override;

		int close() override;

		int bind(const INetAddress &p_addr) override;
		int listen(unsigned int maxListen) override;

		int connect(const INetAddress &p_addr) override;
		int poll(int p_type, int timeout) const override;

		int recvfrom(uint8_t *p_buffer, int p_len, int &r_read, INetAddress &addr, bool p_peek = false) override;
		int recv(void *pbuffer, int p_len, int &sent, bool peek = false) override;
		int send(const uint8_t *p_buffer, int p_len, int &r_sent) override;
		int sendto(const uint8_t *p_buffer, int p_len, int &r_sent, const INetAddress &p_ip) override;
		long int send(const void *pbuffer, int p_len, int &sent) override;
		Ref<NetSocket> accept(INetAddress &addr) override;

		NetStatus accept(NetSocket &socket) override;

		int read() override;
		int write() override;
		bool isBlocking() override;
		void setBlocking(bool blocking) override;

		NetStatus getStatus() const noexcept override;

		void setTimeout(long int nanoSeconds) override;
		long int getTimeout() override;

	  public: /*	CAN Socket Specific NetSocket Methods.	*/
		/**
		 * @brief
		 *
		 * @param ID
		 * @param nBytes
		 * @param data
		 * @return long int
		 */
		long int writeFrame(unsigned int ID, size_t nBytes, uint8_t *data);
		long int readFrame(unsigned int &ID, size_t nBytes, uint8_t *data);

		/**
		 * @brief Set the Filter object
		 *
		 * @param ids
		 */
		void setFilter(std::vector<uint32_t> &ids);

		/**
		 * @brief
		 *
		 * @param enable
		 */
		void enableFDFrames(bool enable);

	  protected:
		bool isNetworkLayerSupported(INetAddress::NetworkProtocol protocol);

	  private:
		CANNetSocket(int socket);

		size_t getFrameSize() const noexcept;
		static int getDomain(const INetAddress &address);

		size_t setupAddress(struct sockaddr *addr, const INetAddress &p_addr);

	  private:
		int socket;
		unsigned int ifrIndex = 0;
		NetStatus netStatus;
		// CANAddress connectAddress;
	};
} // namespace fragcore

#endif
