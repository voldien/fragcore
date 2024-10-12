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
#ifndef _FRAG_CORE_SYNC_H_
#define _FRAG_CORE_SYNC_H_ 1
#include "RenderObject.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Sync : public RenderObject {
		friend class IRenderer;

	  public:
		enum class SyncStatus {
			Complete = 0x0,
			TimeOutExpired = 0x1,
			Error = 0x2,
		};

		/**
		 * @brief
		 *
		 */
		virtual void fence() = 0;

		/**
		 * @brief
		 *
		 * @param timeout
		 */
		virtual void wait(int timeout = 0) = 0;

		/**
		 * @brief
		 *
		 * @param timeout
		 * @return SyncStatus
		 */
		virtual SyncStatus waitClient(int timeout) = 0; /*  Wait in microseconds.   */
	};
} // namespace fragcore

#endif
