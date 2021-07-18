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
#ifndef _FV_BUFFER_H_
#define _FV_BUFFER_H_ 1
#include "../Def.h"
#include "../Prerequisites.h"
#include "RenderObject.h"

namespace fragcore {

	/**
	 * @brief Primitive buffer.
	 *
	 */
	class FVDECLSPEC Buffer : public RenderObject {
		friend class IRenderer;
	  public:
		enum MapTarget {
			eRead = 0x1,		   /*  */
			eWrite = 0x2,		   /*  */
			eReadWrite = 0x4,	   /*  */
			eNoSync = 0x8,		   /*  Disable implicit synchronization.*/
			ePERSISTENT = 0x10,	   /*  */
			eCOHERENT = 0x20,	   /*  */
			eFlushExplicit = 0x40, /*  */
		};

		virtual void bind(void);

		virtual void bind(unsigned int offset, unsigned int size);

		virtual void bindBase(unsigned int base);

		virtual void subData(const void *data, unsigned int offset, unsigned int size);

		virtual void *getData(unsigned int offset, unsigned int size);

		virtual bool isValid(void);

		virtual long int getSize(void);

		virtual void *mapBuffer(MapTarget target);

		virtual void *mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length);

		virtual void flush(unsigned long int offset, unsigned long int length);

		virtual void unMapBuffer(void);

		// virtual void clear(uint32_t i);
		void setName(const std::string &name) override;

		intptr_t getNativePtr(void) const override;
	};
} // namespace fragcore

#endif
