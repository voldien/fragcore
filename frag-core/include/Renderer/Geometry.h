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
#ifndef _FRAG_CORE_GEOMETRY_H_
#define _FRAG_CORE_GEOMETRY_H_ 1
#include"IRenderer.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Geometry : public RenderObject {
		friend class IRenderer;
	public:

		//virtual unsigned int getVertexStride(void* pobj);
		//virtual unsigned int getIndicecStride(void* pobj);
		virtual unsigned int getVertexCount(void);

		virtual unsigned int getIndicesCount(void);


		//virtual BufferObject* verticesbuffer(void* pobj);
		//virtual BufferObject* indicesBuffer(void* pobj);


		intptr_t getNativePtr(void) const override;
	};
}
#endif
