#include "FileSystemNotify.h"
#include <Core/IO/FileSystem.h>
#include <exception>
#include <taskSch.h>

#include <libfswatch/c/cevent.h>
#include <libfswatch/c/libfswatch.h>
#include <libfswatch/c/libfswatch_types.h>

using namespace fragcore;

static void callback(fsw_cevent const *const events, const unsigned int event_num, void *data);

FileSystemNotify::FileSystemNotify(Ref<IScheduler> &sch) {

	if (sch == nullptr) {
		throw InvalidArgumentException("Requires scheduler object.");
	}

	/*	Initilize the notification library.	*/
	FSW_STATUS ret = fsw_init_library();
	if (ret != FSW_OK) {
		throw RuntimeException("Failed to initialize the Filesystem watch: {}.", ret);
	}
	this->session = fsw_init_session(system_default_monitor_type);
	if (this->session == nullptr) {
		throw RuntimeException("Failed to create session for Filesystem watch: {}.", fsw_last_error());
	}
	ret = fsw_set_follow_symlinks(static_cast<FSW_HANDLE>(this->session), true);
	if (ret != FSW_OK) {
		throw RuntimeException("Failed to enable follow symlinks with Filesystem watch: {}.", ret);
	}
	ret = fsw_set_allow_overflow(static_cast<FSW_HANDLE>(this->session), false);
	if (ret != FSW_OK) {
		throw RuntimeException("Failed to disable overflow with Filesystem watch: {}.", ret);
	}
	fsw_set_verbose(false);

	ret = fsw_set_callback(static_cast<FSW_HANDLE>(this->session), callback, this);
	if (ret != FSW_OK) {
		throw RuntimeException("Failed to initialize the Filesystem watch: {}.", ret);
	}

	/*	*/
	this->scheduler = sch;

	/*  Allocate change events. */
	this->fileChangeEvents.resize(32);
}

FileSystemNotify::~FileSystemNotify() {
	// TODO: improve try catch.
	try {
		this->stop();
	} catch (const std::exception &other) {
	}
	fsw_destroy_session(static_cast<FSW_HANDLE>(this->session));
	schDeleteThread(this->pthread);
}

void FileSystemNotify::addFilePath(const char *filepath, FileWatchEvent event, void *object) {
	/*  TODO resolve and make part of the filesystem or IO.	*/
	if (access(filepath, R_OK)) {
		throw InvalidArgumentException("Not a valid file - {}", filepath);
	}

	/*  Get key.    */
	int uid = 0;

	/*  Add watch.  */
	FSW_STATUS ret;
	ret = fsw_add_path(static_cast<FSW_HANDLE>(this->session), filepath);
	if (ret != FSW_OK) {
		throw RuntimeException("Failed to initialize the Filesystem watch: {}.", ret);
	}

	/*  Create file notification entry.  */
	FileNotificationEntry entry;
	entry.filepath = filepath;
	entry.key = 0; // object->getUID();
	entry.callback = event;

	/*  Add file and path entry.    */
	this->notify[uid] = entry;
	objectMap[filepath] = object;
}

void *FileSystemNotify::getObject(const char *path) { return this->objectMap[path]; }

void FileSystemNotify::removeFilePath(const char *path) {

	std::map<int, FileNotificationEntry> *list = &this->notify;
	std::map<int, FileNotificationEntry>::iterator it;
	int key = 0; // object->getUID();

	it = list->find(key);
	if (it != list->end()) {
		FileNotificationEntry *entry = &it->second;
		/*  Remove entry.   */

		list->erase(key);
	} else {
		throw RuntimeException("Could not find object with GID: {}", key);
	}
}

FileNotificationEntry *FileSystemNotify::getEntry(const char *path) {
	return nullptr; // &this->notify[object];
}

void FileSystemNotify::fileFetchTask(Task *package) {}

class FVDECLSPEC FileNotifyTask : public Task {
  public:
	void Execute() noexcept override {}
	void Complete() noexcept override {}
};

void callback(fsw_cevent const *const events, const unsigned int event_num, void *data) {

	/*  */
	FileSystemNotify *notify = (FileSystemNotify *)data;

	/*  */
	for (unsigned int i = 0; i < event_num; i++) {
		fsw_cevent const *const event = &events[i];

		for (int j = 0; j < event->flags_num; j++) {
			fsw_event_flag flag = event->flags[j];
			const char *event_filepath = event->path;

			/*  Check event filter mask.    */
			if (!(flag & (Updated | Removed | Renamed | MovedFrom | MovedTo | OwnerModified | AttributeModified))) {
				continue;
			}

			/*  Get object associated with the reference.   */
			void *object = notify->getObject(event->path);
			assert(object);

			/*  */
			fragcore::FileNotificationEntry *entry = notify->getEntry(event_filepath);

			assert(entry->filepath == event->path);

			/*  FileSystem updated.   */
			if (flag & Updated) {
				entry->callback(notify, entry);
			}

			/*  */
			if (flag & Removed) {
			}

			/*  */
			if (flag & (Renamed | MovedFrom | MovedTo)) {
				/*  Update filepath in entries.    */
				entry->filepath = event->path;
				// TODO: update
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
	if (ret != FSW_OK) {
		throw RuntimeException("Error on starting monitoring with the Filesystem watch: {}.", ret);
	}

	return nullptr;
}

void FileSystemNotify::start() {
	/*  Create monitoring thread.   */
	if (!fsw_is_running(static_cast<FSW_HANDLE>(this->session))) {
		// TODO make use of the thread wrapper method.
		this->pthread = schCreateThread(-1, (schFunc)FileSystemNotify::fswatch, this->session);
	}
}

void FileSystemNotify::stop() {
	FSW_STATUS ret;
	if (fsw_is_running(static_cast<FSW_HANDLE>(this->session))) {
		ret = fsw_stop_monitor(static_cast<FSW_HANDLE>(this->session));
		if (ret != FSW_OK) {
			throw RuntimeException("Error on stopping monitoring with the Filesystem watch: {}.", ret);
		}
	}
}
