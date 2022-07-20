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
#ifndef _FRAG_CORE_INET_ADDRESS_H_
#define _FRAG_CORE_INET_ADDRESS_H_ 1
#include "../UIDObject.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC INetAddress : public UIDObject {
	  public:
		enum class NetworkProtocol : unsigned int {
			NetWorkProtocol_NONE = 0,
			NetWorkProtocol_IP = 1,
			NetWorkProtocol_TCP_UDP = 2,
			NetWorkProtocol_CAN = 3,
			NetWorkProtocol_ModBus = 4,
			NetworkProtocol_Unknown = 1000,
			NetWorkProtocol_ANY = 0xFFFFFFFF,
		};
		INetAddress(NetworkProtocol type) : networkProtocol(type) {}

		virtual NetworkProtocol getNetworkProtocol() const noexcept = 0;

		virtual bool isValid() const noexcept = 0;

		// virtual NetInterface& getInterface() const;

	  private:
		NetworkProtocol networkProtocol;
	};
} // namespace fragcore

#endif
