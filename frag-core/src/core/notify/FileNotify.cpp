#include "FileNotify.h"
#include "Core/IO/FileSystem.h"
#include "Exception/InvalidArgumentException.h"
#include "Exception/RuntimeException.h"
#include "Utils/StringUtil.h"
#include <libfswatch/c/libfswatch.h>

//#include<event.h>

using namespace fragcore;

FileNotify::FileNotify(Ref<IScheduler> &sch) {

	if (sch == NULL)
		throw InvalidArgumentException("Requires scheduler object.");

	/*	Initilize the notification library.	*/
	FSW_STATUS ret = fsw_init_library();
	if (ret != FSW_OK)
		throw RuntimeException(fmt::format("Failed to initialize the Filesystem watch: %d.", ret));
	this->session = fsw_init_session(system_default_monitor_type);
	if (this->session == NULL)
		throw RuntimeException(fmt::format("Failed to create session for Filesystem watch: %d.", fsw_last_error()));
	ret = fsw_set_follow_symlinks(this->session, true);
	if (ret != FSW_OK)
		throw RuntimeException(fmt::format("Failed to enable follow symlinks with Filesystem watch: %d.", ret));
	ret = fsw_set_allow_overflow(this->session, false);
	if (ret != FSW_OK)
		throw RuntimeException(fmt::format("Failed to disable overflow with Filesystem watch: %d.", ret));
	fsw_set_verbose(false);

	/*	*/
	this->scheduler = sch;

	/*  Allocate change events. */
	fileChangeEvents.resize(32);
}

FileNotify::~FileNotify(void) {
	this->stop();
	fsw_destroy_session(this->session);
	schDeleteThread(this->pthread);
}

void FileNotify::registerAsset(const char *filepath, Object *object) {

	/*  TODO resolve and make part of the filesystem or IO.	*/
	if (access(filepath, R_OK))
		throw InvalidArgumentException(fmt::format("Not a valid file - %s", filepath));

	/*  Get key.    */
	int uid = object->getUID();

	/*  Add watch.  */
	FSW_STATUS ret;
	ret = fsw_add_path(this->session, filepath);
	if (ret != FSW_OK)
		throw RuntimeException(fmt::format("Failed to initialize the Filesystem watch: %d.", ret));
	ret = fsw_set_callback(this->session, FileNotify::callback, this);
	if (ret != FSW_OK)
		throw RuntimeException(fmt::format("Failed to initialize the Filesystem watch: %d.", ret));

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

void FileNotify::unregisterAsset(Object *object) {
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
		throw RuntimeException(fmt::format("Could not find object with GID: %d", key));
	}
}

void FileNotify::unRegisterAllAsset(void) {
	std::map<int, FileNoticationEntry>::iterator it = this->notify.begin();

	/*  */
	while (!this->notify.empty()) {
		Object *object = (*this->notify.begin()).second.assetObject;
		unregisterAsset(object);
	}
}

void FileNotify::eventDone(FileNotificationEvent *event) {
	/*  Release resources.    */
	if (event->data)
		free(event->data);
	this->fileChangeEvents.Return(event);
}

void FileNotify::addFilePath(const char *path, Object *object) { objectMap[path] = object; }

Object *FileNotify::getObject(const char *path) { return this->objectMap[path]; }

void FileNotify::removeFilePath(const char *path, Object *object) { this->notify.erase(object->getUID()); }

FileNotify::FileNoticationEntry *FileNotify::getEntry(Object *object) { return &this->notify[object->getUID()]; }

void FileNotify::fileFetchTask(Task *package) {
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
	// fileEvent->data = NULL;
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
	// 	fileEvent->data = NULL;
	// 	fileEvent->size = 0;
	// 	fileNotify->eventDone(fileEvent);
	// }
}

class FVDECLSPEC FileNotifyTask : public Task {
  public:
	virtual void Execute(void) override {}
	virtual void Complete(void) override {}
};

void FileNotify::callback(fsw_cevent const *const events, const unsigned int event_num, void *data) {

	/*  */
	FileNotify *notify = (FileNotify *)data;

	/*  */
	for (int i = 0; i < event_num; i++) {
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
				task.callback = FileNotify::fileFetchTask;
				task.userData = entry;
				// package.puser = &entry->filepath;
				// package.begin = notify;
				// package.end = entry;
				//				package.callback = FileNotify::fileFetchTask;

				notify->scheduler->AddTask(&task);
				notify->scheduler->wait();
				// int status = schSubmitTask(notify->sch, &package, NULL);
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

void *FileNotify::fswatch(const void *psession) {
	const FSW_HANDLE session = (const FSW_HANDLE)psession;

	FSW_STATUS ret = fsw_start_monitor(session);
	if (ret != FSW_OK)
		throw RuntimeException(fmt::format("Error on starting monitoring with the Filesystem watch: %d.", ret));
}

void FileNotify::start(void) {
	/*  Create monitoring thread.   */
	if (!fsw_is_running(this->session)) {
		// TODO make use of the thread wrapper method.
		this->pthread = schCreateThread(-1, (schFunc *)FileNotify::fswatch, this->session);
	}
}

void FileNotify::stop(void) {
	FSW_STATUS ret;
	if (fsw_is_running(this->session)) {
		ret = fsw_stop_monitor(this->session);
		if (ret != FSW_OK)
			throw RuntimeException(fmt::format("Error on stopping monitoring with the Filesystem watch: %d.", ret));
	}
}
