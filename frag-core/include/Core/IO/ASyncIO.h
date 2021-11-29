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
#ifndef _FRAG_CORE_ASYNC_H_
#define _FRAG_CORE_ASYNC_H_ 1
#include "../Ref.h"
#include "../RefPtr.h"
#include "../TaskScheduler/IScheduler.h"
#include "../TaskScheduler/ISemaphore.h"
#include "IO.h"
#include <condition_variable>
#include <thread>
#include <map>

namespace fragcore {

	typedef long ASyncHandle;										   /*  */
	typedef void (*AsyncComplete)(ASyncIO *async, ASyncHandle handle); /*  */

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC ASyncIO : public SmartReference {
	  public:
		ASyncIO(const Ref<IScheduler> &scheduler);
		ASyncIO(ASyncIO &&other); // Move semantics
		~ASyncIO();

		/**
		 * @brief
		 *
		 */
		typedef struct io_status_t {
			long int nbytes; /*  Number of bytes read.   */
			long int offset; /*  Current position in bytes from start position.  */
			long int status; /*  Status of the termination of the IO operation.  */
		} IOStatus;

		// TODO determine if adding support for dynamic sized buffer read and write.
		// TODO add stragety perhaps for how to schedule it.
		virtual ASyncHandle asyncOpen(Ref<IO> &io);

		/**
		 * @brief
		 *
		 * @param handle
		 * @param buffer
		 * @param size
		 * @param complete
		 */
		virtual void asyncReadFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete);
		// TODO add C++ Bind function and lamba support
		virtual void asyncReadFile(ASyncHandle handle, Ref<IO> &writeIO, AsyncComplete complete);

		/**
		 * @brief
		 *
		 * @param handle
		 * @param buffer
		 * @param size
		 * @param complete
		 */
		virtual void asyncWriteFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete);
		virtual void asyncWriteFile(ASyncHandle handle, Ref<IO> &readIO, AsyncComplete complete);

		/*  */
		virtual void asyncWait(ASyncHandle handle);
		virtual bool asyncWait(ASyncHandle handle, long int timeout);

		/**
		 * @brief
		 *
		 * @param handle
		 */
		virtual void asyncClose(ASyncHandle handle);

		/*  */
		virtual Ref<IO> getIO(ASyncHandle handle) const;
		virtual const IOStatus &getIOStatus(ASyncHandle handle) const;

		/*  */
		virtual Ref<IScheduler> getScheduler() const;

	  private:
		/*  Static callback functions.  */
		static void async_open(Task *task);
		static void async_read(Task *task);
		static void async_read_io(Task *task);
		static void async_write(Task *task);
		static void async_write_io(Task *task);

	  protected:   /*  */
		ASyncIO(); // TODO make protected since it is required to a have a scheduler attached but inheriet need to be
				   // able to call it.
		ASyncIO(const ASyncIO &other);
		virtual void setScheduleReference(const Ref<IScheduler> &sch);

		typedef struct async_object {
			// TOOD be replace with an encapsulated class version.
			ISemaphore *sem;
			Ref<IO> ref;			/*  */
			AsyncComplete callback; /*  */
			Ref<IO> target;			/*  */
			char *buffer;			/*  */
			unsigned int size;		/*  */
			IOStatus status;		/*  */
			void *userData;			/*  */
			void *priv;				/*	*/
		} AsyncObject;

		/*	*/
		ASyncHandle generateHandle();
		AsyncObject *getObject(ASyncHandle handle);
		const AsyncObject *getObject(ASyncHandle handle) const;
		AsyncObject *createObject(ASyncHandle handle);

		/*  */
		std::map<ASyncHandle, AsyncObject> asyncs;
		UIDGenerator<size_t> uidGenerator;
		Ref<IScheduler> scheduler;
	};
} // namespace fragcore

#endif
