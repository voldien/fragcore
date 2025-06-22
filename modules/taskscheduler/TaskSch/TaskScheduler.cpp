#include "TaskScheduler.h"
#include "marl/defer.h"
#include "marl/scheduler.h"
#include "marl/task.h"
#include "marl/thread.h"
#include <exception>

using namespace fragcore;
TaskScheduler::TaskScheduler() : TaskScheduler(-1) {}

TaskScheduler::TaskScheduler(int cores) : sch(nullptr) {

	marl::Scheduler::Config config;
	if (cores == -1) {
		config.setWorkerThreadCount(marl::Thread::numLogicalCPUs());
	} else {
		config.setWorkerThreadCount(cores);
	}

	marl::Scheduler *scheduler = new marl::Scheduler(config);
	this->sch = scheduler;
}

TaskScheduler::~TaskScheduler() {
	/*	*/
	try {
		this->terminate();
	} catch (const std::exception &ex) {
	}

	/*	Reset memory.	*/
	marl::Scheduler *scheduler = static_cast<marl::Scheduler *>(this->sch);
	delete scheduler;
	this->sch = nullptr;
}

void TaskScheduler::addTask(Task *task) {
	marl::Scheduler *scheduler = static_cast<marl::Scheduler *>(this->sch);

	/*	*/
	std::function<void()> function = [task] {
		task->Execute();
		task->Complete();
	};

	scheduler->enqueue(marl::Task(std::forward<marl::Task::Function>(function)));
}

void TaskScheduler::setUserData(const void *data) {}
const void *TaskScheduler::getUserData() { return nullptr; }
void TaskScheduler::run() {}

void TaskScheduler::terminate() {
	marl::Scheduler *scheduler = static_cast<marl::Scheduler *>(this->sch);
	defer(scheduler->unbind());
}

void TaskScheduler::wait() { marl::Scheduler *scheduler = static_cast<marl::Scheduler *>(this->sch); }
void TaskScheduler::wait(Task *task) { marl::Scheduler *scheduler = static_cast<marl::Scheduler *>(this->sch); }

void TaskScheduler::lock() {}
void TaskScheduler::unLock() {}
