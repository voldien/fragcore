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
#ifndef _FRAG_CORE_TCP_UDP_ADDRESS_H_
#define _FRAG_CORE_TCP_UDP_ADDRESS_H_ 1
#include "IPAddress.h"
#include <string>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC TCPUDPAddress : public INetAddress {
	  public:
		TCPUDPAddress() {}
		TCPUDPAddress(const TCPUDPAddress &other) = default;
		TCPUDPAddress(const IPAddress &ipaddress, unsigned int port);
		~TCPUDPAddress() = default;
		TCPUDPAddress &operator=(const TCPUDPAddress &other) = default;
		TCPUDPAddress(TCPUDPAddress &&other) = default;
		TCPUDPAddress &operator=(TCPUDPAddress &&other) = default;

		bool operator==(const TCPUDPAddress &ipAddress) const {
			if (this == &ipAddress) {
				return true;
			}
			if (this->getPort() == ipAddress.getPort() && ipAddress.getIPAddress() == getIPAddress()) {
				return true;
			}
			return false;
		}
		bool operator!=(const TCPUDPAddress &ipAddress) const { return !(*this == ipAddress); }

		NetworkProtocol getNetworkProtocol() const noexcept override {
			return NetworkProtocol::NetWorkProtocol_TCP_UDP;
		}

		const IPAddress &getIPAddress() const noexcept { return ipAddress; }
		unsigned int getPort() const noexcept { return this->port; }
		bool isValid() const noexcept override;

	  private:
		unsigned int port;
		IPAddress ipAddress;
	};
} // namespace fragcore

#endif
