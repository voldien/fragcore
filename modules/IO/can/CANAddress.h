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
#ifndef _FRAGCORE_CAN_ADDRESS_H_
#define _FRAGCORE_CAN_ADDRESS_H_ 1
#include <Network/INetAddress.h>
#include <string>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC CANAddress : public INetAddress {
	  public:
		enum class CANStandard { CANRaw, CANBCM, CANISOTP, CANJ1939 };

		CANAddress(const std::string &interface, unsigned int ID);
		CANAddress(unsigned int ID);

		bool operator==(const CANAddress &address) const {
			if (this == &address) {
				return true;
			}
			if (this->getID() == address.getID()) {
				return true;
			}
			return false;
		}
		bool operator!=(const CANAddress &address) const { return !(*this == address); }

		NetworkProtocol getNetworkProtocol() const noexcept override { return NetworkProtocol::NetWorkProtocol_CAN; }

		bool isValid() const noexcept override { return this->valid; }
		CANStandard getStandard() const noexcept { return CANStandard::CANRaw; }
		unsigned int getID() const noexcept { return this->id; }
		const std::string getInterface() const;

	  protected:
	  private:
		bool valid;
		unsigned int id;
		unsigned int ifrIndex;
	};
} // namespace fragcore

#endif
