
#include <Core/RefPtr.h>
#include <Core/TaskScheduler/TaskScheduler.h>
#include <gtest/gtest.h>

using namespace fragcore;

TEST(Scheduler, Create_TaskScheduler_No_Exception_Thrown) { ASSERT_NO_THROW(TaskScheduler taskSch()); }

TEST(Scheduler, Create_TaskScheduler_Add_Task_No_Exception) {
	TaskScheduler taskSch;
	class QuickTask : public Task {
	  public:
		virtual void Execute() noexcept {}
		virtual void Complete() noexcept {}
	};
	ASSERT_NO_THROW(QuickTask task);
	// ASSERT_NO_THROW(taskSch.addTask(&task));
}