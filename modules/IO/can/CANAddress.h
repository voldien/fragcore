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
#ifndef _FRAG_CORE_CAN_ADDRESS_H_
#define _FRAG_CORE_CAN_ADDRESS_H_ 1
#include "Core/Network/INetAddress.h"
#include <string>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC CANAddress : public INetAddress {
	  public:
		CANAddress(unsigned int ID)
			: INetAddress(NetworkProtocol::NetWorkProtocol_CAN), valid(false), id(ID), ifrIndex(0) {}

		bool operator==(const CANAddress &address) const {
			if (this == &address)
				return true;
			if (this->getID() == address.getID())
				return true;
			return false;
		}
		bool operator!=(const CANAddress &address) const { return !(*this == address); }

		virtual NetworkProtocol getNetworkProtocol() const noexcept override {
			return NetworkProtocol::NetWorkProtocol_CAN;
		}

		virtual bool isValid() const noexcept override { return this->valid; }
		unsigned int getID() const noexcept { return this->id; }

	  protected:
	  private:
		bool valid;
		unsigned int id;
		unsigned int ifrIndex;
	};
} // namespace fragcore

#endif
