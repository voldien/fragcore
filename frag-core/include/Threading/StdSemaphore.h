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
#ifndef _FRAGCORE_STD_SEMPAHORE_H_
#define _FRAGCORE_STD_SEMPAHORE_H_ 1
#include "ISemaphore.h"

#include <taskSch.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class stdSemaphore : public ISemaphore {
	  public:
		stdSemaphore();
		~stdSemaphore() override;

		void lock() override;
		void unlock() override;
		void wait(long int nanoTimeout = -1) override;

		intptr_t getNativePtr() const override;

	  private:
		schSemaphore *semaphore; /*  */
	};
} // namespace fragcore

#endif
