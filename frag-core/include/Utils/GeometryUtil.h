/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2020 Valdemar Lindberg

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
#ifndef _FRAG_CORE_GEOMETRYUTIL_H_
#define _FRAG_CORE_GEOMETRYUTIL_H_ 1
#include "../Def.h"

namespace fragcore {

	/**
	 *
	 */
	// TODO name class to match the file name.
	class FVDECLSPEC GeometryUtility {
	  public:
		static bool TestPlanesAABB(const Plane &plane, const AABB &bound);
	};
} // namespace fragcore
#endif
