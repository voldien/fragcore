/**
	Simple physic simulation with a server-client model support.
	Copyright (C) 2016  Valdemar Lindberg

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
#ifndef _PHYSIC_INTERPRETOR_H_
#define _PHYSIC_INTERPRETOR_H_ 1
#include"ProtocolHeaders.h"
#include"network/Connection.h"

/**
 *	Responsible for interpreting
 *	physic interface packets.
 */
class PVDECLSPEC PhysicInterpreter{
public:
	PhysicInterpreter(void);
	PhysicInterpreter(const PhysicInterpreter& other);
	~PhysicInterpreter(void);


	/**
	 *
	 * @param interface
	 * @param header
	 */
	void interpret(PhysicInterpreter* interface, PacketHeader* header);

};

#endif
