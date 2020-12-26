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
#ifndef _FRAG_CORE_NETSOCKET_H_
#define _FRAG_CORE_NETSOCKET_H_ 1
#include "../SmartReference.h"
#include "IP.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC NetSocket : public SmartReference {
	public:
		NetSocket(void);
		~NetSocket(void);
		enum TransportProtocol {
			UDP,	/**/
			TCP,	/*	*/
		};

		/*virtual int bind(IP adddrr, Type _type);
		virtual int connect(IP& addr);
		virtual int listen(void);
		Status accept(NetSocket &socket);

		virtual int close(void);
		virtual int read(void);
		virtual int write(void);
		virtual bool isBlocking(void);
		virtual void setBlocking(bool blocking);

	enum class Status {
		Done,
		NotReady,
		Partial,
		Disconnected,
		Error
	};
		
*/
	};
}

#endif