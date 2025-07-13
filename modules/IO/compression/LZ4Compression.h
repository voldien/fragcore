/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_LZ4_COMPRESSION_H_
#define _FRAGCORE_LZ4_COMPRESSION_H_ 1
#include "ICompression.h"

namespace fragcore {
	class FVDECLSPEC LZ4Compression : public ICompression {
	  public:
		LZ4Compression(const size_t block_size = 4096);

		ssize_t inflate(const void *data_in, size_t inSize, void *out, size_t size) override;

		ssize_t deflate(const void *data_in, size_t inSize, void *out, size_t size) override;

		void set_block_size(const size_t block_size);

	  private:
		size_t block_size;
	};
} // namespace fragcore

#endif
