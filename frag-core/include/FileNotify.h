/**
    FrameView for rendering shaders in screen space.
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
#ifndef _FRAG_CORE_FILE_NOTIFY_H_
#define _FRAG_CORE_FILE_NOTIFY_H_ 1
#include"Prerequisites.h"
#include"FileChangeEvent.h"
#include"IFileNotify.h"
#include"Core/Object.h"
#include"Core/TaskScheduler/IThreading.h"
#include"Core/TaskScheduler/TaskScheduler.h"
#include"Core/dataStructure/PoolAllocator.h"
//TODO resolve include path and other solution.
#include<libfswatch/c/libfswatch_types.h>
#include<libfswatch/c/cevent.h>
#include<map>
#include<vector>

namespace fragcore {

	typedef void *NotifyHandle;
//	typedef void (*AsyncComplete)(ASync *async, ASyncHandle handle); /*  */
	/**
	 *
	 */
	//TODO make it more generic in order to reduce coupling and increase cohession!
	class FVDECLSPEC FileNotify : public IFileNotify {
	public:

		/**
		 * Start the file notification process.
		 */
		void start(void);
		void stop(void);
		//void restart(void);

	protected:

		/**
		 *
		 */
		class FVDECLSPEC FileNoticationEntry {
		public:
			int key;                /*  */
			std::string filepath;   /*  */
			Object *assetObject;    /*  */
			void *userdata;
			//TOOD change! 
			//AssetType type;         /*  */

		};

	public:

		//TODO relocate to another class.
		void registerAsset(const char *filepath, Object *object);
		void registerAsset(Ref<IO> &io);
		void unregisterAsset(Object *notify);
		void unregisterAsset(Ref<IO> &io);
		void unRegisterAllAsset(void);

		void eventDone(FileNotificationEvent *event);

		//TODO improve
		bool pollEvent(void);
		void eventDone(const std::vector<FileNotificationEvent> &events) const;
		bool releaseEventBuffer(void);

	private:

		/**
		 *
		 * @param path
		 */
		void addFilePath(const char *path, Object *object);

		/**
		 *
		 * @param path
		 */
		void removeFilePath(const char *path, Object *object);

		/**
		 *
		 * @param path
		 * @return
		 */
		Object *getObject(const char *path);

		/**
		 *
		 * @param object
		 * @return
		 */
		FileNoticationEntry *getEntry(Object *object);

		/**
		 *
		 * @param events
		 * @param event_num
		 * @param data
		 */
		static void callback(fsw_cevent const *const events,
		                     const unsigned int event_num,
		                     void *data);

		static void fileFetchTask(Task *package);

		/**
		 * Start fswatch internal process.
		 */
		static void *fswatch(const void *psession);

	private:

		std::map<int, FileNoticationEntry> notify;  /*  List of update-able assets.  */
		std::map<std::string, Object *> objectMap;  /*  */
		FSW_HANDLE session;                         /*  */

		void *pthread;      /*  Thread for async operations. */
		Ref<IThreading> thread;
		Ref<IScheduler> scheduler;
		PoolAllocator<FileNotificationEvent> fileChangeEvents;

	public: /*  */

		FileNotify(Ref<IScheduler>& sch);

		~FileNotify(void);
	};
}

#endif