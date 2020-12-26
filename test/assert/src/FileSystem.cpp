#include <Core/IO/GZFileIO.h>
#include <Core/IO/ZipFileSystem.h>
#include <Core/IO/FileIO.h>
#include <Core/IO/ZipFileIO.h>
#include <Core/IO/FileSystem.h>
#include <Core/IO/BufferIO.h>
#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <Core/RefPtr.h>
#include<Exception/RuntimeException.h>
#include <FragCore.h>

using namespace fragcore;

TEST(FileSystem, FileSystem_Without_Calling_Create_Throw_Exception){
	ASSERT_THROW(FileSystem::getFileSystem(), RuntimeException);
}

TEST(FileSystem, Create_FileSystem_Without_Task_Scheduler)
{
	ASSERT_NO_THROW(FileSystem::createFileSystem());
}

TEST(FileSystem, Create_FileSystem_With_Task_Scheduler_Throw_No_Exception)
{
	TaskScheduler *task = new TaskScheduler();
	Ref<IScheduler> refTask = Ref<IScheduler>(task);

	ASSERT_NO_THROW(FileSystem::createFileSystem(refTask));
}

TEST(FileSystem, FileSystem_With_Task_Scheduler_Check_Supported){
	TaskScheduler *task = new TaskScheduler();
	Ref<IScheduler> refTask = Ref<IScheduler>(task);
	FileSystem *fileSystem = FileSystem::createFileSystem(refTask);

	ASSERT_TRUE(fileSystem->isASyncSupported());
}

TEST(FileSystem, FileSystem_WithNoTaskScheduler_Check_NotSupported){
	FileSystem *fileSystem = FileSystem::createFileSystem();
	ASSERT_FALSE(fileSystem->isASyncSupported());
}

TEST(FileSystem, FileSystem_Get_Directories_No_Throw){
	FileSystem *fileSystem = FileSystem::createFileSystem();
	ASSERT_NO_THROW(fileSystem->listDirectories("."));
}

TEST(FileSystem, FileSystem_Get_Directory_list_file_No_Throw)
{
	FileSystem *fileSystem = FileSystem::createFileSystem();
	ASSERT_NO_THROW(fileSystem->listFiles("."));
}

TEST(FileSystem, FileSystem_Compute_Absolute_Path_Correct){
	FileSystem *fileSystem = FileSystem::createFileSystem();
	std::string fullPath = fileSystem->getAbsolutePath("");

	//TODO add evoluation.
}

TEST(FileSystem, FileSystem_Compute_Relative_Path_Correct){

}



/*	TODO relocate to its own file later.	*/
TEST(ZipFileSystem, OpenZipFile_Invalid_Path_Throw_Exception){
	ASSERT_THROW(ZipFileSystem::createZipFileObject(""), InvalidArgumentException);
}

TEST(ZipFileSystem, OpenZipFile_Invalid_Memory_Throw_Exception){
	void *buf = "hello world!";
	int len = sizeof(buf);

	ASSERT_THROW(ZipFileSystem::createZipFileObject(buf, len), RuntimeException);
}

TEST(ZipFileSystem, OpenZipFile_Create_with_Task)
{ //TODO
}