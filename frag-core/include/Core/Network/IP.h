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
#ifndef _FRAG_CORE_IP_H_
#define _FRAG_CORE_IP_H_ 1
#include "../Object.h"
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IP : public Object {
	  public:
		enum Type {

			TYPE_NONE = 0,
			TYPE_IPV4 = 1,
			TYPE_IPV6 = 2,
			TYPE_ANY = 3,
		};
		IP(const std::string &ip, Type type);

		const std::string &getIP(void) const;
		const std::string &getHostName(void) const;

	  protected:
		//		virtual IP_Address _resolve_hostname(const String &p_hostname, Type p_type = TYPE_ANY) = 0;
		static std::vector<IP> getLocalAddresses(void);
		static std::vector<IP> getLocalInterface(void);
		//		Array _get_local_addresses() const;
		//		Array _get_local_interfaces() const;
	  private:
		Type type;
		std::string ip;
	};
} // namespace fragcore

#endif