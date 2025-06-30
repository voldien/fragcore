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
#ifndef _FRAGCORE_BUFFER_H_
#define _FRAGCORE_BUFFER_H_ 1
#include "FragDef.h"
#include "RenderObject.h"
#include "RenderPrerequisites.h"

namespace fragcore {

	/**
	 * @brief Primitive buffer.
	 *
	 */
	class FVDECLSPEC Buffer : public RenderObject {
		friend class IRenderer;

	  public:
		enum MapTarget : uint32_t {
			Read = 0x1,			  /*  */
			Write = 0x2,		  /*  */
			ReadWrite = 0x4,	  /*  */
			NoSync = 0x8,		  /*  Disable implicit synchronization.*/
			PERSISTENT = 0x10,	  /*  */
			COHERENT = 0x20,	  /*  */
			FlushExplicit = 0x40, /*  */
		};

		~Buffer() override = default;

		virtual void bind() = 0;

		virtual void bind(unsigned int offset, unsigned int size) = 0;

		virtual void bindBase(unsigned int base) = 0;

		virtual void subData(const void *data, unsigned int offset, unsigned int size) = 0;

		virtual void *getData(unsigned int offset, unsigned int size) = 0;

		virtual bool isValid() = 0;

		virtual long int getSize() = 0;

		/**
		 * @brief
		 *
		 * @param target
		 * @return void*
		 */
		virtual void *mapBuffer(MapTarget target) = 0;

		/**
		 * @brief
		 *
		 * @param target
		 * @param offset
		 * @param length
		 * @return void*
		 */
		virtual void *mapBuffer(MapTarget target, unsigned long int offset, unsigned long int length) = 0;

		/**
		 * @brief
		 *
		 * @param offset
		 * @param length
		 */
		virtual void flush(unsigned long int offset, unsigned long int length) = 0;

		/**
		 * @brief
		 *
		 */
		virtual void unMapBuffer() = 0;
	};
} // namespace fragcore

#endif
