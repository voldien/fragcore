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
#ifndef _FRAGCORE_INET_ADDRESS_H_
#define _FRAGCORE_INET_ADDRESS_H_ 1
#include "../Core/UIDObject.h"

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
		INetAddress() = default;

		virtual NetworkProtocol getNetworkProtocol() const noexcept = 0;

		virtual bool isValid() const noexcept = 0;

		// virtual NetInterface& getInterface() const;

	  private:
	};
} // namespace fragcore

#endif
