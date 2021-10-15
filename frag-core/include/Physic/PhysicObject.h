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
#ifndef _FRAG_CORE_PHYSICOBJECT_H_
#define _FRAG_CORE_PHYSICOBJECT_H_ 1
#include "../Core/SmartReference.h"
#include "../Prerequisites.h"
#include "Prerequisites.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC PhysicObject : public SmartReference {
	  public:
		inline PhysicInterface *getPhysicInterface() const { return this->iRenderer; }

		// inline Ref<PhysicObject> getRef() const{

		// }

		inline void *getObject() { return this->pdata; }

		inline void *getObject() const { return this->pdata; }

		virtual intptr_t getNativePtr() const = 0;

	  protected: /*  */
		void *pdata;
		PhysicInterface *iRenderer; // TOOD change to ref.
	};
} // namespace fragcore

#endif
