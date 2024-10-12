#include "TaskScheduler.h"
#include "marl/defer.h"
#include "marl/event.h"
#include "marl/scheduler.h"
#include "marl/task.h"
#include "marl/waitgroup.h"
#include <exception>

using namespace fragcore;
TaskScheduler::TaskScheduler() : TaskScheduler(-1) {}

TaskScheduler::TaskScheduler(int cores) : sch(nullptr) {

	marl::Scheduler *scheduler = new marl::Scheduler(marl::Scheduler::Config::allCores());
	scheduler->bind();

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

void TaskScheduler::wait() {}
void TaskScheduler::wait(Task *task) {}

void TaskScheduler::lock() {}
void TaskScheduler::unLock() {}
