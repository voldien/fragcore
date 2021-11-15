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
#include "../Ref.h"
#include "../RefPtr.h"
#include "IScheduler.h"
#include <map>

// TODO relocate to its own module.
namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC TaskScheduler : public IScheduler {
	  public:
		TaskScheduler();
		TaskScheduler(int cores, unsigned int maxPackagesPool);
		virtual ~TaskScheduler();

		virtual void addTask(Task *task) override;
		virtual void setUserData(const void *data) override;
		virtual const void *getUserData() override;
		virtual void run() override;
		virtual void terminate() override;
		virtual void wait() override;

		virtual void lock() override;
		virtual void unLock() override;

	  private:
		void *sch;
		std::map<int, Task *> tasks;
	};
} // namespace fragcore
#endif
