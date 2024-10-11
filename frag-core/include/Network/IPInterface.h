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
#ifndef _FRAG_CORE_IP_INTERFACE_H_
#define _FRAG_CORE_IP_INTERFACE_H_ 1
#include "NetInterface.h"
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IPInterface : public NetInterface {
	  public:
		enum class Type {
			TYPE_NONE = 0,
			TYPE_IPV4 = 1,
			TYPE_IPV6 = 2,
			TYPE_ANY = 3,
		};
		IPInterface(const std::string &ip, Type type);

		const std::string &getIP() const;
		const std::string &getHostName() const;
		Type getIPType() const noexcept;
		const std::string getBroadCastAddress();
		unsigned int getNetMask();

	  protected:
		//		virtual IP_Address _resolve_hostname(const String &p_hostname, Type p_type = TYPE_ANY) = 0;
		static std::vector<IPInterface> getLocalAddresses();
		static std::vector<IPInterface> getLocalInterface();
		//		Array _get_local_addresses() const;
		//		Array _get_local_interfaces() const;
	  private:
		Type type;
		std::string ip;
	};
} // namespace fragcore

#endif
