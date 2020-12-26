/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2018  Valdemar Lindberg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_SYNC_H_
#define _FRAG_CORE_SYNC_H_ 1
#include"RenderObject.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Sync : public RenderObject {
		friend class IRenderer;

	public:

		enum SyncStatus {
			Complete = 0x0,
			TimeOutExpired = 0x1,
			Error = 0x2,
		};

		virtual void fence(void);

		virtual void wait(int timeout = 0);

		virtual SyncStatus waitClient(int timeout); /*  Wait in microseconds.   */

		virtual intptr_t getNativePtr(void) const;
	};
}

#endif