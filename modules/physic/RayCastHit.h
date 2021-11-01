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
#ifndef _FRAG_CORE_RAY_CAST_HIT_H_
#define _FRAG_CORE_RAY_CAST_HIT_H_ 1
#include"PhysicInterface.h"

namespace fragcore {
	/**
	 *	RayCastHit.
	 */
	class FVDECLSPEC RayCastHit {
	public:
	//TODO add parmaters and methods.
		inline RayCastHit(){}
		~RayCastHit(){

		}

		//Collider*  collider() const;

		//RigidBody*  rigidBody() const;

	//	VDGameObject*  gameObject() const;

		//VDTransform*  transform() const;

		//Vector3& point() const;

		//Vector3 &normal() const;

		//float distance() const;

		//PVVector2& barycentric() const;
	protected:
		void *hitdata;
	};
}

#endif
