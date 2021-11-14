#include "Core/TaskScheduler/TaskScheduler.h"
#include <taskSch.h>

using namespace fragcore;
TaskScheduler::TaskScheduler() {
	schTaskSch *taskSch = (schTaskSch *)malloc(sizeof(schTaskSch));
	memset(taskSch, 0, sizeof(schTaskSch));
	int sch = schCreateTaskPool(taskSch, 2, SCH_FLAG_NO_AFM, 48);
	if (sch != SCH_OK)
		throw RuntimeException(schErrorMsg(sch));

	this->sch = taskSch;
}

TaskScheduler::TaskScheduler(int cores, unsigned int maxPackagesPool) {
	schTaskSch *taskSch = (schTaskSch *)malloc(sizeof(schTaskSch));
	int sch = schCreateTaskPool(taskSch, cores, SCH_FLAG_NO_AFM, maxPackagesPool);
	if (sch != SCH_OK)
		throw RuntimeException(schErrorMsg(sch));

	this->sch = taskSch;
}

TaskScheduler::~TaskScheduler() {
	schTaskSch *taskSch = static_cast<schTaskSch *>(this->sch);
	this->terminate();
	schReleaseTaskSch(taskSch);
	/*	Release.	*/
	delete taskSch;
	/*	Reset memory.	*/
	this->sch = nullptr;
}

static int internal_schCallback(struct sch_task_package_t *package) {
	/*	extract variables.	*/
	Task *task = static_cast<Task *>(package->begin);
	IScheduler *scheduler = reinterpret_cast<IScheduler *>(package->end);
	Task::TaskCallBack callback = task->callback;

	task->Execute();
	callback(task);
	task->Complete();

	/*	Release task resources.	*/
	return 0;
}

void TaskScheduler::addTask(Task *task) {
	schTaskPackage packageTask = {};
	// TODO IMPROVE
	// task->scheduler = Ref<IScheduler>(this);
	// TODO how to handle the resources.

	packageTask.callback = internal_schCallback;
	packageTask.begin = task;
	packageTask.end = this;
	packageTask.puser = nullptr;

	int status = schSubmitTask((schTaskSch *)this->sch, &packageTask, nullptr);
	if (status != SCH_OK)
		throw RuntimeException("{}", schErrorMsg(status));
}

void TaskScheduler::setUserData(const void *data) { return schSetSchUserData((schTaskSch *)this->sch, data); }
const void *TaskScheduler::getUserData() { return schGetPoolUserData((schTaskSch *)this->sch, 0); }
void TaskScheduler::run() {
	schTaskSch *taskSch = static_cast<schTaskSch *>(this->sch);
	int status = schRunTaskSch(taskSch);
	if (status != SCH_OK)
		throw RuntimeException(schErrorMsg(status));
}

void TaskScheduler::terminate() {
	schTaskSch *taskSch = static_cast<schTaskSch *>(this->sch);
	int status = schTerminateTaskSch(taskSch);

	if (status != SCH_OK)
		throw RuntimeException(schErrorMsg(status));
}

void TaskScheduler::wait() { schWaitTask((schTaskSch *)this->sch); }
void TaskScheduler::lock() {}
void TaskScheduler::unLock() {}
