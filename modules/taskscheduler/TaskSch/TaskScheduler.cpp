#include "TaskScheduler.h"
#include <taskSch.h>

using namespace fragcore;
TaskScheduler::TaskScheduler() : TaskScheduler(-1, 48) {}

TaskScheduler::TaskScheduler(int cores, unsigned int maxPackagesPool) : sch(nullptr) {
	schTaskSch *taskSch = nullptr;

	int result_code = schAllocateTaskPool(&taskSch);
	if (result_code != SCH_OK) {
		throw RuntimeException("Failed to allocate Scheduler: {}", schErrorMsg(result_code));
	}
	result_code = schCreateTaskPool(taskSch, cores, SCH_FLAG_NO_AFM, maxPackagesPool);
	if (result_code != SCH_OK) {
		throw RuntimeException("Failed to create Scheduler: {}", schErrorMsg(result_code));
	}

	this->sch = taskSch;
}

TaskScheduler::~TaskScheduler() {
	schTaskSch *taskSch = static_cast<schTaskSch *>(this->sch);
	this->terminate();
	int result_code = schReleaseTaskSch(taskSch);
	if (result_code != SCH_OK) {
	}
	/*	Release.	*/

	/*	Reset memory.	*/
	free(this->sch);
	this->sch = nullptr;
}

static int internal_schCallback_execute_task(struct sch_task_package_t *package) {
	/*	extract variables.	*/
	Task *task = static_cast<Task *>(package->begin);
	IScheduler *scheduler = reinterpret_cast<IScheduler *>(package->end);
	/*	*/
	Task::TaskCallBack callback = task->callback;

	task->Execute();
	callback(task);
	task->Complete();

	/*	Release task resouresult_codees.	*/
	return 0;
}

void TaskScheduler::addTask(Task *task) {
	schTaskPackage packageTask = {};
	// TODO IMPROVE
	// task->scheduler = Ref<IScheduler>(this);
	// TODO how to handle the resouresult_codees.

	packageTask.callback = internal_schCallback_execute_task;
	packageTask.begin = task;
	packageTask.end = this;
	packageTask.puser = nullptr;

	int status = schSubmitTask((schTaskSch *)this->sch, &packageTask, nullptr);
	if (status != SCH_OK) {
		throw RuntimeException("{}", schErrorMsg(status));
	}
}

void TaskScheduler::setUserData(const void *data) { return schSetSchUserData((schTaskSch *)this->sch, data); }
const void *TaskScheduler::getUserData() { return schGetPoolUserData((schTaskSch *)this->sch, 0); }
void TaskScheduler::run() {
	schTaskSch *taskSch = static_cast<schTaskSch *>(this->sch);
	int status = schRunTaskSch(taskSch);
	if (status != SCH_OK) {
		throw RuntimeException(schErrorMsg(status));
	}
}

void TaskScheduler::terminate() {
	schTaskSch *taskSch = static_cast<schTaskSch *>(this->sch);
	int status = schTerminateTaskSch(taskSch);

	if (status != SCH_OK) {
		throw RuntimeException(schErrorMsg(status));
	}
}

void TaskScheduler::wait() { schWaitTask((schTaskSch *)this->sch); }
void TaskScheduler::wait(Task *task) {}

void TaskScheduler::lock() {}
void TaskScheduler::unLock() {}
