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
#ifndef _FRAG_CORE_ISCHEDULER_H_
#define _FRAG_CORE_ISCHEDULER_H_ 1
#include "../Ref.h"
#include "../SmartReference.h"

namespace fragcore {
	/**
	 *
	 */
	class IScheduler;
	/**
	 * @brief 
	 * 
	 */
	class FVDECLSPEC Task : UIDObject {
	  public:
		typedef void (*TaskCallBack)(Task *task);
		TaskCallBack callback;
		void *userData;

		virtual void Execute() noexcept = 0;
		virtual void Complete() noexcept = 0;

	  private:
		Ref<IScheduler> scheduler;
	};

	// TODO rename so that it includes the name task.
	class FVDECLSPEC IScheduler : public SmartReference {
	  public:
		virtual void addTask(Task *task) = 0;
		virtual void setUserData(const void *data) = 0;
		virtual const void *getUserData() = 0;
		virtual void run() = 0;
		virtual void terminate() = 0;
		virtual void wait() = 0;
		virtual void lock() = 0;
		virtual void unLock() = 0;

	  private:
	};
} // namespace fragcore
#endif
