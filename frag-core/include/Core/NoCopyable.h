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
#ifndef _FV_NO_COPYABLE_H_
#define _FV_NO_COPYABLE_H_ 1
#include "../FragDef.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC NoCopyable {
	  public:
		NoCopyable(const NoCopyable &) = delete;			// non construction-copyable
		NoCopyable &operator=(const NoCopyable &) = delete; // non copyable
		NoCopyable() = default;
	};
} // namespace fragcore

#endif
