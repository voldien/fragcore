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
#ifndef _FRAGCORE_IP_ADDRESS_H_
#define _FRAGCORE_IP_ADDRESS_H_ 1
#include "INetAddress.h"
#include "IPInterface.h"
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
		IPAddress();
		IPAddress(void *encoded, const IPAddressType type); // TODO add binary as the address.
		IPAddress(const std::vector<uint8_t> &encoded_address);
		IPAddress(const IPAddress &other) = default;
		IPAddress &operator=(const IPAddress &other) = default;
		IPAddress(IPAddress &&other) = default;
		IPAddress &operator=(IPAddress &&other) = default;

		IPAddress(const std::string &ipAddress,const  IPAddressType type);
		IPAddress(const std::string &hostname);

		bool operator==(const IPAddress &ipAddress) const {
			if (this == &ipAddress) {
				return true;
			}
			if (this->getIPType() == ipAddress.getIPType() && ipAddress.getIP() == this->getIP()) {
				return true;
			}
			return false;
		}
		bool operator!=(const IPAddress &ipAddress) const { return !(*this == ipAddress); }

		NetworkProtocol getNetworkProtocol() const noexcept override { return NetworkProtocol::NetWorkProtocol_IP; }

		const std::string &getIP() const noexcept { return this->ip; }
		const uint8_t *getAddress(IPAddressType addressType) const noexcept;
		const std::string &getHostName() const;
		bool isValid() const noexcept override;
		IPAddressType getIPType() const noexcept { return this->type; }

		virtual NetInterface getInterface();

	  protected:
		static unsigned int getDomain(IPAddressType IPaddressType) noexcept;
		static IPAddressType convertDomain2AddressType(int domain);

	  private:
		std::string ip; /*	Hostname.	*/
		IPAddressType type;
		bool valid;
		union {
			uint8_t field8[16];	 /*	Up to 16 bytes, support from IPV4 to IPV6.	*/
			uint16_t field16[8]; /*	Up to 16 bytes, support from IPV4 to IPV6.	*/
			uint32_t field32[4]; /*	Up to 16 bytes, support from IPV4 to IPV6.	*/
		};
	};
} // namespace fragcore

#endif
