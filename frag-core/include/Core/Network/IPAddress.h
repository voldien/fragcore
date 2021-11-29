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
#ifndef _FRAG_CORE_IP_ADDRESS_H_
#define _FRAG_CORE_IP_ADDRESS_H_ 1
#include "INetAddress.h"
#include <string>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IPAddress : public INetAddress {
	  public:
		enum class IPAddressType {

			IPAddress_Type_NONE = 0,
			IPAddress_Type_IPV4 = 1,
			IPAddress_Type_IPV6 = 2,
			IPAddress_Type_ANY = 3,
		};
		IPAddress(); // TODO add binary as the address.
		//IPAddress(const std::vector<uint8_t>& encoded_address);
		IPAddress(const IPAddress &other) = default;
		IPAddress &operator=(const IPAddress &other) = default;
		IPAddress(IPAddress &&other) = default;
		IPAddress &operator=(IPAddress &&other) = default;

		IPAddress(const std::string &ip, IPAddressType type);
		IPAddress(const std::string &hostname);

		bool operator==(const IPAddress &ipAddress) const {
			if (this == &ipAddress)
				return true;
			if (this->getIPType() == ipAddress.getIPType() && ipAddress.getIP() == this->getIP())
				return true;
			return false;
		}
		bool operator!=(const IPAddress &ipAddress) const { return !(*this == ipAddress); }

		virtual NetworkProtocol getNetworkProtocol() const noexcept override {
			return NetworkProtocol::NetWorkProtocol_IP;
		}

		const std::string &getIP() const noexcept { return this->ip; }
		const uint8_t *getAddress(IPAddressType addressType) const noexcept;
		const std::string &getHostName() const;
		virtual bool isValid() const noexcept override;
		IPAddressType getIPType() const noexcept { return this->type; }

	  protected:
		static unsigned int getDomain(IPAddressType IPaddressType) noexcept;
		static IPAddressType convertDomain2AddressType(int domain);

	  private:
		std::string ip;
		IPAddressType type;
		bool valid;
		union {
			uint8_t field8[16];
			uint16_t field16[8];
			uint32_t field32[4];
		};
	};
} // namespace fragcore

#endif
