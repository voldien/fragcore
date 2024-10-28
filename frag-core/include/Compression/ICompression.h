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
#ifndef _FRAGCORE_ICOMPRESSION_H_
#define _FRAGCORE_ICOMPRESSION_H_ 1
#include "../Core/Ref.h"
#include "../FragDef.h"
#include "../IO/IO.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC ICompression : public Object {
	  public:
		virtual void inflate(Ref<IO> io_in, Ref<IO> out) = 0;
		virtual void inflate(const void *data_in, size_t inSize, void *out, size_t size) = 0;

		virtual void deflate(Ref<IO> io_in, Ref<IO> out) = 0;
		virtual void deflate(const void *data_in, size_t inSize, void *out, size_t size) = 0;
	};
} // namespace fragcore

#endif
