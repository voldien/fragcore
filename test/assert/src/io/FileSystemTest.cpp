#include "TaskScheduler/IScheduler.h"
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <IO/FileSystem.h>
#include <gtest/gtest.h>

using namespace fragcore;

class FileSystemTest : public testing::Test {
  public:
	void SetUp() override {}

	Ref<IScheduler> sche{nullptr};
};

TEST(FileSystem, FileSystem_Without_Calling_Create_Throw_Exception) {
	ASSERT_THROW(FileSystem::getFileSystem(), RuntimeException);
}

TEST(FileSystem, Create_FileSystem_Without_Task_Scheduler) { ASSERT_NO_THROW(FileSystem::createFileSystem()); }

TEST_F(FileSystemTest, Create_With_Task_Scheduler_Throw_No_Exception) {

	ASSERT_NO_THROW(FileSystem::createFileSystem(this->sche));
}

TEST_F(FileSystemTest, Create_With_Task_Scheduler_Check_Supported) {
	FileSystem *fileSystem = FileSystem::createFileSystem(this->sche);

	ASSERT_TRUE(fileSystem->isASyncSupported());
}

TEST(FileSystem, FileSystem_WithNoTaskScheduler_Check_NotSupported) {
	FileSystem *fileSystem = FileSystem::createFileSystem();
	ASSERT_FALSE(fileSystem->isASyncSupported());
}

TEST(FileSystem, FileSystem_Get_Directories_No_Throw) {
	FileSystem *fileSystem = FileSystem::createFileSystem();
	ASSERT_NO_THROW(fileSystem->listDirectories("."));
}

TEST(FileSystem, FileSystem_Get_Directory_list_file_No_Throw) {
	FileSystem *fileSystem = FileSystem::createFileSystem();
	ASSERT_NO_THROW(fileSystem->listFiles("."));
}

TEST(FileSystem, FileSystem_Compute_Absolute_Path_Correct) {
	FileSystem *fileSystem = FileSystem::createFileSystem();

	std::string fullPath;
	ASSERT_NO_THROW(fullPath = fileSystem->getAbsolutePath("."));
	ASSERT_TRUE(!fullPath.empty());
}

TEST(FileSystem, FileSystem_Compute_Relative_Path_Correct) {}
