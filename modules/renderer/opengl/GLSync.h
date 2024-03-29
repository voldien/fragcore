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
#ifndef _FRAG_CORE_GL_SYNC_H_
#define _FRAG_CORE_GL_SYNC_H_ 1
#include "../Sync.h"
#include <GL/glew.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC GLSync : public Sync {
		friend class IRenderer;

	  public:
		/**
		 * @brief
		 *
		 */
		virtual void fence() override;

		/**
		 * @brief
		 *
		 * @param timeout
		 */
		virtual void wait(int timeout = 0) override;

		/**
		 * @brief
		 *
		 * @param timeout
		 * @return SyncStatus
		 */
		virtual SyncStatus waitClient(int timeout) override; /*  Wait in microseconds.   */

		virtual intptr_t getNativePtr() const override;

	  private:
		GLsync sync;
	};
} // namespace fragcore

#endif
