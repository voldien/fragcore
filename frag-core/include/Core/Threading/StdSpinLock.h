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
#ifndef _FRAG_CORE_STD_SPIN_LOCK_H_
#define _FRAG_CORE_STD_SPIN_LOCK_H_ 1
#include "ISpinLock.h"
#include <taskSch.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class StdSpinLock : public ISpinLock {
	  public:
		StdSpinLock();
		virtual ~StdSpinLock();

		virtual void lock() = 0;
		virtual void unlock() = 0;
		virtual void wait(long int nanoTimeout = -1) = 0;

		virtual intptr_t getNativePtr() const = 0;

	  private:
		schSpinLock *spinlock; /*  */
	};
} // namespace fragcore

#endif
