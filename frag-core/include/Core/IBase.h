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
#ifndef _FRAGCORE_IBASE_H_
#define _FRAGCORE_IBASE_H_ 1
#include "IO/IO.h"
#include "Ref.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IBase {
	  public:
		virtual ~IBase() = default;
		virtual void encode(Ref<IO> &raw, Ref<IO> &encoded) = 0;
		virtual void decode(Ref<IO> &encoded, Ref<IO> &raw) = 0;
	};
} // namespace fragcore
#endif
