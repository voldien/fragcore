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
#include"../Def.h"

namespace fragcore {
	/**
	 *	Collision object.
	 *
	 */
	class FVDECLSPEC Collision {
		friend class DynamicInterface;

	private:
		inline Collision(void) {}

		virtual ~Collision(void);

	public:    /*	Public methods.	*/

		/**
		 * Get center.
		 * @return
		 */
		inline PVVector3 getCenter(void) {
			//return getCenter(getMappedObject());
		}

		/**
		 *
		 * @param center
		 */
		inline void setCenter(const PVVector3 &center) {
			//setCenter(getMappedObject(), center);
		}


	protected:    /*	Protected virtual methods.	*/

		virtual PVVector3 getCenter(void *pobj);

		virtual void setCenter(void *pobj, const PVVector3 &center);

	};
}

#endif
