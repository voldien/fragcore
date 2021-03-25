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
#ifndef _FRAG_CORE_RENDEROBJECT_H_
#define _FRAG_CORE_RENDEROBJECT_H_ 1
#include "../Core/Ref.h"
#include "../Core/SmartReference.h"
#include "../Prerequisites.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC RenderObject : public SmartReference {
		friend class IRenderer;

	  public:
		RenderObject(void) {}
		RenderObject(IRenderer *renderer) { setRenderInterface(renderer); }

		inline IRenderer *getRenderer(void) const { return this->iRenderer; }
		// inline Ref<IRenderer> getRef(void) const{

		// }

		inline void *getObject(void) { return this->pdata; }

		inline void *getObject(void) const { return this->pdata; }

		virtual intptr_t getNativePtr(void) const = 0;

		virtual void setRenderInterface(IRenderer *interface) { this->iRenderer = interface; }

	  protected: /*  */
		void *pdata;
		// Ref<IRenderer> irenderer;
		IRenderer *iRenderer; // TOOD change to ref.
	};
} // namespace fragcore

#endif
