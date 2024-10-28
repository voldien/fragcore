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
#ifndef _FRAGCORE_VK_SYNC_H_
#define _FRAGCORE_VK_SYNC_H_ 1
#include "../Sync.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VKSync : public Sync {
		friend class IRenderer;

	  public:

		virtual void fence()override;

		virtual void wait(int timeout = 0) override;

		virtual SyncStatus waitClient(int timeout) override; /*  Wait in microseconds.   */

		virtual intptr_t getNativePtr() const override;
	};
} // namespace fragcore

#endif
