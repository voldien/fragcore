/*
 *	FragCore - Fragment Core - Engine Core
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

#ifndef _FRAG_CORE_FILE_NOTIFY_H_
#define _FRAG_CORE_FILE_NOTIFY_H_ 1
#include "IO/IO.h"
#include <Core/Object.h>
#include <DataStructure/PoolAllocator.h>
#include <TaskScheduler/IScheduler.h>
#include <TaskScheduler/IThreading.h>
#include <map>
#include <vector>

namespace fragcore {

	class IFileNotify;
	class FileNotificationEntry;

	using FileWatchEvent = void (*)(IFileNotify *, const FileNotificationEntry *); /*  */

	class FVDECLSPEC FileNotificationEntry {
	  public:
		int key;			  /*  */
		std::string filepath; /*  */
		void *userdata;
		Ref<IO> refIO;
		FileWatchEvent callback;
	};

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC IFileNotify : public SmartReference {
	  public:
		/**
		 *
		 */

		~IFileNotify() override {}

		virtual void start() = 0;
		virtual void stop() = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @param object
		 */
		virtual void addFilePath(const char *path, FileWatchEvent event, void *object = nullptr) = 0;

		/**
		 * @brief
		 *
		 * @param path
		 * @param object
		 */
		virtual void removeFilePath(const char *path) = 0;

		/**
		 * @brief Get the Object object
		 *
		 * @param path
		 * @return Object*
		 */
		virtual void *getObject(const char *path) = 0;

		/**
		 * @brief Get the Entry object
		 *
		 * @param object
		 * @return FileNoticationEntry*
		 */
		virtual FileNotificationEntry *getEntry(const char *path);
	};

	using NotifyHandle = void *;
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC FileSystemNotify : public IFileNotify {
	  public:
		/**
		 * Start the file notification process.
		 */
		void start() override;
		void stop() override;

	  private:
		void eventDone(FileNotificationEntry *event);

		// TODO improve
		bool pollEvent();
		void eventDone(const std::vector<FileNotificationEntry> &events) const;
		bool releaseEventBuffer();

	  public:
		/**
		 *
		 * @param path
		 */
		void addFilePath(const char *path, FileWatchEvent event, void *object = nullptr) override;

		/**
		 *
		 * @param path
		 */
		void removeFilePath(const char *path) override;

		/**
		 *
		 * @param path
		 * @return
		 */
		void *getObject(const char *path) override;

		/**
		 *
		 * @param object
		 * @return
		 */
		FileNotificationEntry *getEntry(const char *path) override;

	  protected:
		/**
		 *
		 * @param events
		 * @param event_num
		 * @param data
		 */

		static void fileFetchTask(Task *package);

		/**
		 * Start fswatch internal process.
		 */
		static void *fswatch(const void *psession);

	  private:
		std::map<int, FileNotificationEntry> notify; /*  List of update-able assets.  */
		std::map<std::string, void *> objectMap;	 /*  */
		NotifyHandle session;						 /*  */

		void *pthread; /*  Thread for async operations. */
		Ref<IThreading> thread;
		Ref<IScheduler> scheduler;
		PoolAllocator<FileNotificationEntry> fileChangeEvents;

	  public: /*  */
		FileSystemNotify(Ref<IScheduler> &sch);

		~FileSystemNotify() override;
	};
} // namespace fragcore

#endif
