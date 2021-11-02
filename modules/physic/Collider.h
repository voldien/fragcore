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
#ifndef _FRAG_CORE_COLLIDER_H_
#define _FRAG_CORE_COLLIDER_H_ 1
#include <FragCore.h>

namespace fragcore {
	/**
	 *	Collision object.
	 *
	 */
	class FVDECLSPEC Collision {
		friend class DynamicInterface;

	  private:
		inline Collision() {}

		virtual ~Collision();

	  public: /*	Public methods.	*/
		/**
		 * Get center.
		 * @return
		 */
		inline Vector3 getCenter() {
			// return getCenter(getMappedObject());
		}

		/**
		 *
		 * @param center
		 */
		inline void setCenter(const Vector3 &center) {
			// setCenter(getMappedObject(), center);
		}

	  protected: /*	Protected virtual methods.	*/
		virtual Vector3 getCenter(void *pobj);

		virtual void setCenter(void *pobj, const Vector3 &center);
	};
} // namespace fragcore

#endif
