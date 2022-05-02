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
#ifndef _FRAG_CORE_COLLIDER_H_
#define _FRAG_CORE_COLLIDER_H_ 1
#include <FragCore.h>

namespace fragcore {
	/**
	 *	Collision object.
	 *
	 */
	class FVDECLSPEC Collider {
		friend class DynamicInterface;

	  private:
		Collision() = default;

		virtual ~Collision() = default;

	  public: /*	Public methods.	*/
		virtual Vector3 getCenter() = 0;

		/**
		 *
		 * @param center
		 */
		virtual void setCenter(const Vector3 &center) = 0;
	};
} // namespace fragcore

#endif
