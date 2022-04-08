#include "FileSystemNotify.h"
#include <Core/IO/FileSystem.h>
#include <taskSch.h>

#include <libfswatch/c/libfswatch.h>

using namespace fragcore;

FileSystemNotify::FileSystemNotify(Ref<IScheduler> &sch) {

	if (sch == nullptr)
		throw InvalidArgumentException("Requires scheduler object.");

	/*	Initilize the notification library.	*/
	FSW_STATUS ret = fsw_init_library();
	if (ret != FSW_OK)
		throw RuntimeException("Failed to initialize the Filesystem watch: {}.", ret);
	this->session = fsw_init_session(system_default_monitor_type);
	if (this->session == nullptr)
		throw RuntimeException("Failed to create session for Filesystem watch: {}.", fsw_last_error());
	ret = fsw_set_follow_symlinks(this->session, true);
	if (ret != FSW_OK)
		throw RuntimeException("Failed to enable follow symlinks with Filesystem watch: {}.", ret);
	ret = fsw_set_allow_overflow(this->session, false);
	if (ret != FSW_OK)
		throw RuntimeException("Failed to disable overflow with Filesystem watch: {}.", ret);
	fsw_set_verbose(false);

	/*	*/
	this->scheduler = sch;

	/*  Allocate change events. */
	fileChangeEvents.resize(32);
}

FileSystemNotify::~FileSystemNotify() {
	this->stop();
	fsw_destroy_session(this->session);
	schDeleteThread(this->pthread);
}

void FileSystemNotify::registerAsset(const char *filepath, Object *object) {

	/*  TODO resolve and make part of the filesystem or IO.	*/
	if (access(filepath, R_OK))
		throw InvalidArgumentException("Not a valid file - {}", filepath);

	/*  Get key.    */
	int uid = object->getUID();

	/*  Add watch.  */
	FSW_STATUS ret;
	ret = fsw_add_path(this->session, filepath);
	if (ret != FSW_OK)
		throw RuntimeException("Failed to initialize the Filesystem watch: {}.", ret);
	ret = fsw_set_callback(this->session, FileSystemNotify::callback, this);
	if (ret != FSW_OK)
		throw RuntimeException("Failed to initialize the Filesystem watch: {}.", ret);

	/*  Create file notification entry.  */
	FileNoticationEntry entry;
	entry.assetObject = object;
	entry.filepath = filepath;
	entry.key = object->getUID();
	// entry.type = assetType;

	/*  Add file and path entry.    */
	this->notify[uid] = entry;
	addFilePath(filepath, object);
}

void FileSystemNotify::unregisterAsset(Object *object) {
	std::map<int, FileNoticationEntry> *list = &this->notify;
	std::map<int, FileNoticationEntry>::iterator it;
	int key = object->getUID();

	it = list->find(key);
	if (it != list->end()) {
		FileNoticationEntry *entry = &it->second;
		/*  Remove entry.   */
		removeFilePath(entry->filepath.c_str(), object);
		list->erase(key);
	} else {
		throw RuntimeException("Could not find object with GID: {}", key);
	}
}

void FileSystemNotify::unRegisterAllAsset() {
	std::map<int, FileNoticationEntry>::iterator it = this->notify.begin();

	/*  */
	while (!this->notify.empty()) {
		Object *object = (*this->notify.begin()).second.assetObject;
		unregisterAsset(object);
	}
}

void FileSystemNotify::eventDone(FileNotificationEvent *event) {
	/*  Release resources.    */
	if (event->data)
		free(event->data);
	this->fileChangeEvents.Return(event);
}

void FileSystemNotify::addFilePath(const char *path, Object *object) { objectMap[path] = object; }

Object *FileSystemNotify::getObject(const char *path) { return this->objectMap[path]; }

void FileSystemNotify::removeFilePath(const char *path, Object *object) { this->notify.erase(object->getUID()); }

FileSystemNotify::FileNoticationEntry *FileSystemNotify::getEntry(Object *object) {
	return &this->notify[object->getUID()];
}

void FileSystemNotify::fileFetchTask(Task *package) {
	// TODO reolcate to the application specific.

	// std::string *path = (std::string *) package->puser;
	// FileNotify *fileNotify = (FileNotify *) package->begin;
	// FileNoticationEntry *entry = (FileNoticationEntry *) package->end;

	// /*  */
	// FileNotificationEvent *fileEvent = fileNotify->fileChangeEvents.obtain();
	// fileEvent->object = entry->assetObject;
	// fileEvent->path = path->c_str();
	// fileEvent->type = entry->type;
	// fileEvent->size = 0;
	// fileEvent->data = nullptr;
	// fileEvent->timestamp = SDL_GetPerformanceCounter();

	// /*  */
	// SDL_Event event = {};
	// event.type = SDL_USEREVENT;
	// event.user.data1 = fileEvent;
	// event.user.code = ASSET_UPDATE;

	// /*  Load file.  */
	// try {
	// 	void *data;
	// 	IO* io = FileSystem::getFileSystem()->openFile(path->c_str(), IO::READ);
	// 	fileEvent->size = FileSystem::getFileSystem()->loadFile(io, (char **) &data);
	// 	fileEvent->data = data;
	// 	SDL_PushEvent(&event);
	// 	delete io;
	// } catch (RuntimeException &ex) {
	// 	fileEvent->data = nullptr;
	// 	fileEvent->size = 0;
	// 	fileNotify->eventDone(fileEvent);
	// }
}

class FVDECLSPEC FileNotifyTask : public Task {
  public:
	virtual void Execute() noexcept override {}
	virtual void Complete() noexcept override {}
};

void FileSystemNotify::callback(fsw_cevent const *const events, const unsigned int event_num, void *data) {

	/*  */
	FileSystemNotify *notify = (FileSystemNotify *)data;

	/*  */
	for (unsigned int i = 0; i < event_num; i++) {
		fsw_cevent const *const event = &events[i];

		for (int j = 0; j < event->flags_num; j++) {
			fsw_event_flag flag = event->flags[j];

			/*  Check event filter mask.    */
			if (!(flag & (Updated | Removed | Renamed | MovedFrom | MovedTo | OwnerModified | AttributeModified)))
				continue;

			/*  Get object associated with the reference.   */
			Object *object = notify->getObject(event->path);
			assert(object);

			/*  */
			FileNoticationEntry *entry = notify->getEntry(object);

			assert(entry->filepath == event->path);

			/*  FileSystem updated.   */
			if (flag & Updated) {

				FileNotifyTask task;
				task.callback = FileSystemNotify::fileFetchTask;
				task.userData = entry;
				// package.puser = &entry->filepath;
				// package.begin = notify;
				// package.end = entry;
				//				package.callback = FileNotify::fileFetchTask;

				notify->scheduler->addTask(&task);
				notify->scheduler->wait();
				// int status = schSubmitTask(notify->sch, &package, nullptr);
				// if (status != SCH_OK) {

				// }
				// schWaitTask(notify->sch);
			}

			/*  */
			if (flag & Removed) {
				notify->unregisterAsset(entry->assetObject);
			}

			/*  */
			if (flag & (Renamed | MovedFrom | MovedTo)) {
				/*  Update filepath in entries.    */
				entry->filepath = event->path;
			}

			/*  Check if file is still accessiable. */
			if (flag & (OwnerModified | AttributeModified)) {
				if (!FileSystem::getFileSystem()->isReadable(event->path)) {
				}
			}
		}
	}
}

void *FileSystemNotify::fswatch(const void *psession) {
	const FSW_HANDLE session = (const FSW_HANDLE)psession;

	FSW_STATUS ret = fsw_start_monitor(session);
	if (ret != FSW_OK)
		throw RuntimeException("Error on starting monitoring with the Filesystem watch: {}.", ret);
	return nullptr;
}

void FileSystemNotify::start() {
	/*  Create monitoring thread.   */
	if (!fsw_is_running(this->session)) {
		// TODO make use of the thread wrapper method.
		this->pthread = schCreateThread(-1, (schFunc)FileSystemNotify::fswatch, this->session);
	}
}

void FileSystemNotify::stop() {
	FSW_STATUS ret;
	if (fsw_is_running(this->session)) {
		ret = fsw_stop_monitor(this->session);
		if (ret != FSW_OK)
			throw RuntimeException("Error on stopping monitoring with the Filesystem watch: {}.", ret);
	}
}
