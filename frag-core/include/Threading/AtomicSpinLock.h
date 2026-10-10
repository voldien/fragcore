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
#ifndef _FRAGCORE_ATOMIC_SPIN_LOCK_H_
#define _FRAGCORE_ATOMIC_SPIN_LOCK_H_ 1
#include "ISpinLock.h"

namespace fragcore {
	/**
	 * @brief
	 */
	class AtomicSpinlock : public ISpinLock {
	  private:
		std::atomic_flag atomic_flag = ATOMIC_FLAG_INIT;

	  public:
		void lock() override {
			while (atomic_flag.test_and_set(std::memory_order_acquire)) {
			}
		}
		void unlock() override { atomic_flag.clear(std::memory_order_release); }

		intptr_t getNativePtr() const override { return static_cast<intptr_t>(this->atomic_flag._M_i); }
	};
} // namespace fragcore
#endif