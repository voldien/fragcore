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
#ifndef _FRAG_CORE_ASYNC_H_
#define _FRAG_CORE_ASYNC_H_ 1
#include"../RefPtr.h"
#include"../Ref.h"
#include"../TaskScheduler/IScheduler.h"
#include"IO.h"
#include<condition_variable>
#include<thread>
#if defined(FRAG_CORE_INTERNAL_IMP)	//TODO resolve to a single file or something later
#include<taskSch.h>
#endif
#include<map>

namespace fragcore {

	typedef long ASyncHandle;											/*  */
	typedef void (*AsyncComplete)(ASync* async, ASyncHandle handle);	/*  */

	/**
	 *
	 */
	//TODO rename so that it include the IO name.
	class FVDECLSPEC ASync : public SmartReference {
	public:
		ASync(Ref<IScheduler> &scheduler);
		ASync(const ASync &&other); // Move semantics
		~ASync(void);

		typedef struct io_status_t {
			unsigned int nbytes;    /*  Number of bytes read.   */
			unsigned int offset;    /*  Current position in bytes from start position.  */
			unsigned int status;    /*  Status of the termination of the IO operation.  */
		} IOStatus;

		//TODO determine if adding support for dynamic sized buffer read and write.
		//TODO add stragety perhaps for how to schedule it.
		virtual ASyncHandle asyncOpen(Ref<IO> &io);
		virtual void asyncReadFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete);
		virtual void asyncReadFile(ASyncHandle handle, Ref<IO>& writeIO, AsyncComplete complete);
		virtual void asyncWriteFile(ASyncHandle handle, char *buffer, unsigned int size, AsyncComplete complete);
		virtual void asyncWriteFile(ASyncHandle handle, Ref<IO>& readIO, AsyncComplete complete);

		/*  */
		virtual void asyncWait(ASyncHandle handle);
		virtual bool asyncWait(ASyncHandle handle, long int timeout);
		virtual void asyncClose(ASyncHandle handle);

		/*  */
		virtual Ref<IO> getIO(ASyncHandle handle) const;
		virtual const IOStatus& getIOStatus(ASyncHandle handle) const;

		/*  */
		virtual Ref<IScheduler> getScheduler(void) const;

	private:
		/*  Static callback functions.  */
		static void async_open(Task *task);
		static void async_read(Task *task);
		static void async_read_io(Task *task);
		static void async_write(Task *task);
		static void async_write_io(Task *task);

	protected:  /*  */
		ASync(void); //TODO make protected since it is required to a have a scheduler attached but inheriet need to be able to call it.
		ASync(const ASync& other);
		virtual void setScheduleReference(Ref<IScheduler>& sch);

		typedef struct async_object {
			#if defined(FRAG_CORE_INTERNAL_IMP)	//TODO resolve to a single file or something later
			schSemaphore* semaphore;    /*  */
			#else
			void* data;
			#endif
			Ref<IO> ref;                /*  */
			AsyncComplete callback;     /*  */
			Ref<IO> target;             /*  */
			char *buffer;               /*  */
			unsigned int size;          /*  */
			IOStatus status;            /*  */
			void *userData;             /*  */
			void *priv;					/*	*/
		} AsyncObject;

		/*	*/
		AsyncObject* getObject(ASyncHandle handle);
		const AsyncObject* getObject(ASyncHandle handle) const;
		AsyncObject* createObject(ASyncHandle handle);

		/*  */
		std::map<ASyncHandle, AsyncObject> asyncs;
		UIDGenerator uidGenerator;
		Ref<IScheduler> scheduler;
	};
}

#endif
