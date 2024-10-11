/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FV_GL_BUFFER_H_
#define _FV_GL_BUFFER_H_ 1
#include "../Buffer.h"
#include "internal_object_type.h"

namespace fragcore {
	/**
	 *	Primitive buffer.
	 */
	class FVDECLSPEC GLBuffer : public Buffer {
		friend class GLRendererInterface;

	  public:
		GLBuffer() = default;

		virtual ~GLBuffer() = default;

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
		unsigned int target;
		unsigned int buffer;
		unsigned int base;
	};
} // namespace fragcore

#endif
