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
#ifndef _FRAG_CORE_CL_BUFFER_H_
#define _FRAG_CORE_CL_BUFFER_H_ 1
#include "../Buffer.h"
#include "internal_object_type.h"

namespace fragcore {
	/**
	 *	Primitive buffer.
	 */
	class FVDECLSPEC CLBuffer : public Buffer {

	  public:
		CLBuffer() = default;

		virtual ~CLBuffer() = default;

		virtual void bind() override;

		virtual void bind(unsigned int offset, unsigned int size) override;

		virtual void bindBase(unsigned int base) override;

		virtual void subData(const void *data, unsigned int offset, unsigned int size) override;

		virtual void *getData(unsigned int offset, unsigned int size) override;

		virtual bool isValid() override;

		virtual long int getSize() override;

		virtual void *mapBuffer(MapTarget target) override;

		virtual void *mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) override;

		virtual void flush(unsigned long int offset, unsigned long int length) override;

		virtual void unMapBuffer() override;

		// virtual void clear(uint32_t i);
		void setName(const std::string &name) override;

		intptr_t getNativePtr() const override;

	  protected:
		BufferDesc desc;
		cl_mem target;
		unsigned int buffer;
		unsigned int base;
		Buffer *interp;
	};
} // namespace fragcore

#endif
