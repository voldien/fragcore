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
#ifndef _FRAG_CORE_PHYSIC_INTERPRETOR_H_
#define _FRAG_CORE_PHYSIC_INTERPRETOR_H_ 1
#include "../PhysicInterface.h"
#include "ProtocolHeaders.h"

namespace fragcore {
	/**
	 *	Responsible for interpreting
	 *	physic interface packets.
	 */
	class FVDECLSPEC RPCPhysicInterpreter {
	  public:
		RPCPhysicInterpreter();
		RPCPhysicInterpreter(const RPCPhysicInterpreter &other);
		virtual ~RPCPhysicInterpreter();

		/**
		 *
		 * @param interface
		 * @param header
		 */
		void interpret(RPCPhysicInterpreter *interface, PacketHeader *header);
	};
} // namespace fragcore

#endif
