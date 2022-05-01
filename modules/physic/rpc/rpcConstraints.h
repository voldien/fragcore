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
#ifndef _FRAG_CORE_RPC_CONSTRAINTS_H_
#define _FRAG_CORE_RPC_CONSTRAINTS_H_ 1
#include "../Constraints.h"
#include <FragCore.h>

namespace fragcore {
	class RigidBody;
	/**
	 *
	 */
	class FVDECLSPEC RPCConstraints : public Constraints {
		friend class DynamicInterface;

	  public:
		RPCConstraints() = default;
		virtual ~RPCConstraints() = default;

	  public:
		virtual void attachRigidbody(RigidBody *r1, RigidBody *r2) override;
	};
} // namespace fragcore
#endif
