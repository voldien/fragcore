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
#ifndef _FRAG_CORE_TASK_SCHEDULER_H_
#define _FRAG_CORE_TASK_SCHEDULER_H_ 1
#include "../RefPtr.h"
#include"IScheduler.h"
#include "../Ref.h"
#if defined(FRAGVIEW_CORE_INTERNAL_IMP)
#include<taskSch.h>
#endif
#include <map>

namespace fragcore {
	/**
	 * 
	 */
	class FVDECLSPEC TaskScheduler : public IScheduler {
	public:
		TaskScheduler(void);
		TaskScheduler(int cores, unsigned int maxPackagesPool);
		~TaskScheduler(void);

		virtual void AddTask(Task *task) override;
		virtual void setUserData(const void *data) override;
		virtual const void *getUserData(void) override;
		virtual void run(void) override;
		virtual void terminate(void) override;
		virtual void wait(void) override;
		virtual void Lock(void) override;
		virtual void UnLock(void) override;

	private:
		#ifdef FRAGVIEW_CORE_INTERNAL_IMP
		schTaskSch *sch;
		#else
		void *sch;
		#endif
		std::map<int, Task*> tasks;
	};
}
#endif