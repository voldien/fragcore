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
#ifndef _FRAG_CORE_ISCHEDULER_H_
#define _FRAG_CORE_ISCHEDULER_H_ 1
#include "../Ref.h"
#include "../SmartReference.h"

namespace fragcore {
	class IScheduler;
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Task : public Object {
	  public:
		Task() = default;
		template <class Function, class... Args> explicit Task(Function &&f, Args &&... args);
		virtual ~Task() = default;

		typedef void (*TaskCallBack)(Task *task);
		TaskCallBack callback;
		void *userData;

		virtual void Execute() noexcept = 0;
		virtual void Complete() noexcept = 0;

		template <typename... Args> void succeed(Args &&... args) {}
		template <typename... Args> void precede(Args &&... args) {}

	  private:
		Ref<IScheduler> scheduler;
	};

	// TODO rename so that it includes the name task.
	class FVDECLSPEC IScheduler : public SmartReference {
	  public:
		virtual ~IScheduler() = default;

		/**
		 * @brief
		 *
		 * @param task
		 */
		virtual void addTask(Task *task) = 0;

		template <class Function, class... Args> void addTask(Function &&f, Args &&... args) {}

		/**
		 * @brief Set the User Data object
		 *
		 * @param data
		 */
		virtual void setUserData(const void *data) = 0;

		/**
		 * @brief Get the User Data object
		 *
		 * @return const void*
		 */
		virtual const void *getUserData() = 0;

		/**
		 * @brief
		 *
		 */
		virtual void run() = 0;

		/**
		 * @brief
		 *
		 */
		virtual void terminate() = 0;

		/**
		 * @brief
		 *
		 */
		virtual void wait() = 0;
		// virtual void wait(Task *task);

		/**
		 * @brief
		 *
		 */
		virtual void lock() = 0;

		/**
		 * @brief
		 *
		 */
		virtual void unLock() = 0;

	  private:
	};
} // namespace fragcore
#endif
