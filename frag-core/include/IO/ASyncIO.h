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
#ifndef _FRAGCORE_ASYNC_H_
#define _FRAGCORE_ASYNC_H_ 1
#include "../TaskScheduler/IScheduler.h"
#include "../Threading/ISemaphore.h"
#include "FragDef.h"
#include "IO.h"
#include <map>

/**
 * @addtogroup libfragcore_io IO
 * @{
 */
namespace fragcore {

	using ASyncHandle = ssize_t;							/*  */
	using AsyncComplete = void (*)(ASyncIO *, ASyncHandle); /*  */

	/**
	 * @brief Asynchronous IO
	 *
	 * Give support for IO being read and written
	 * asynchronous.
	 *
	 */
	class FVDECLSPEC ASyncIO : public SmartReference {
	  public:
		ASyncIO(const Ref<IScheduler> &scheduler);
		ASyncIO(ASyncIO &&other);
		~ASyncIO() override;

		/**
		 * @brief
		 *
		 */
		using IOStatus = struct io_status_t {
			std::atomic_long nbytes; /*  Number of bytes read.   */
			std::atomic_long offset; /*  Current position in bytes from start position.  */
			std::atomic_long status; /*  Status of the termination of the IO operation.  */
		};

		// TODO determine if adding support for dynamic sized buffer read and write.
		// TODO add stragety perhaps for how to schedule it.
		virtual ASyncHandle asyncOpen(Ref<IO> &ioRef);

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
		/**
		 * @brief
		 *
		 * @param handle
		 * @param writeIO
		 * @param complete
		 */
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

		// TODO add C++ Bind function and lamba support
		/**
		 * @brief
		 *
		 * @param handle
		 * @param readIO
		 * @param complete
		 */
		virtual void asyncWriteFile(ASyncHandle handle, Ref<IO> &readIO, AsyncComplete complete);

		/**
		 * @brief
		 *
		 * @param handle
		 */
		virtual void asyncWait(ASyncHandle handle);

		/**
		 * @brief
		 *
		 * @param handle
		 * @param timeout_nanosec
		 * @return true
		 * @return falset
		 */
		virtual bool asyncWait(ASyncHandle handle, long int timeout_nanosec);

		/**
		 * @brief
		 *
		 * @param handle
		 */
		virtual void asyncClose(ASyncHandle handle);

		/**
		 * @brief
		 *
		 * @param handle
		 * @return Ref<IO>
		 */
		virtual Ref<IO> getIO(ASyncHandle handle) const;

		/**
		 * @brief Get current IO Status.
		 *
		 * Will return the current IO status of a specific
		 * asynchronous process.
		 *
		 * @param handle A valid asynchronous handle.
		 * @return const IOStatus&
		 */
		virtual const IOStatus &getIOStatus(ASyncHandle handle) const;

		/**
		 * @brief Get the Scheduler object
		 *
		 * @return Ref<IScheduler>
		 */
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

	  protected:
		/**
		 * @brief
		 *
		 */
		using AsyncObject = struct async_object {
			// TODO be replace with an encapsulated class version.
			ISemaphore *sem;
			Ref<IO> ref;			/*  */
			AsyncComplete callback; /*  */
			Ref<IO> target;			/*  */
			char *buffer;			/*  */
			unsigned int size;		/*  */
			IOStatus status;		/*  */
			void *userData;			/*  */
			void *priv;				/*	*/
		};

		class FVDECLSPEC AsyncTask : public Task {
		  public:
			void Execute() noexcept override {}
			void Complete() noexcept override {}
			AsyncTask(AsyncObject &asyncObj) : asyncObject(asyncObj) {}
			ASyncIO::AsyncObject &asyncObject;
		};

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

/**
 *
 * @}
 */

#endif
