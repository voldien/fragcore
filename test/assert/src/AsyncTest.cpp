#include <Core/IO/ASyncIO.h>
#include <Core/IO/BufferIO.h>
#include <Core/IO/FileIO.h>
#include <Core/IO/FileSystem.h>
#include <Core/IO/GZFileIO.h>
#include <Core/IO/ZipFileIO.h>
#include <Core/IO/ZipFileSystem.h>
#include <Core/RefPtr.h>
#include <Core/TaskScheduler/IScheduler.h>
#include <FragCore.h>
#include <SDL2/SDL.h>
#include <Window/Display.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(ASyncIO, Async_Create_NoThrowException) {
	Ref<IScheduler> sche = Ref<IScheduler>(new TaskScheduler());
	ASSERT_NO_THROW(ASyncIO async(sche));
}

TEST(ASyncIO, Async_Create_Handle_No_Throw_Exception) {
	ASSERT_NO_THROW(ASyncHandle handle);
}

TEST(ASyncIO, ASync_Open_IO_Async_Handle_No_Throw) {
	Ref<IScheduler> sche = Ref<IScheduler>(new TaskScheduler());
	ASyncIO async(sche);

	Ref<IO> ioRef = SystemInfo::getStdOut();
	ASyncHandle handle;
	ASSERT_NO_THROW(handle = async.asyncOpen(ioRef));
	ASSERT_NO_THROW(async.asyncClose(handle));
}

TEST(ASyncIO, ASync_ReadIOAsync_ThrowException) {
	Ref<IScheduler> sche = Ref<IScheduler>(new TaskScheduler());
	ASyncIO async(sche);

	Ref<IO> ioRef = SystemInfo::getStdOut();
	ASyncHandle handle;
	handle = async.asyncOpen(ioRef);
	ASSERT_THROW(async.asyncWriteFile(handle, nullptr, 0, nullptr), InvalidArgumentException);
}

TEST(ASyncIO, ASync_Wait_Correctly_NoThrowException) {
	Ref<IScheduler> sche = Ref<IScheduler>(new TaskScheduler());
	ASyncIO async(sche);

	Ref<IO> ioRef = SystemInfo::getStdOut();
	ASyncHandle handle;
	handle = async.asyncOpen(ioRef);
	char buf[32];
	async.asyncReadFile(handle, buf, sizeof(buf), nullptr);
	ASSERT_NO_THROW(async.asyncWait(handle));
}

TEST(ASyncIO, ASync_CallBack_Called_No_Throw) {}

TEST(ASyncIO, Async) {
	// ASyncHandle handle;
	// IScheduler *sch;

	// /*  Create scheduler.   */
	// //ASSERT_EQ(schCreateTaskPool(&sch, 2, SCH_FLAG_NO_AFM, 32), SCH_OK);
	// //ASSERT_EQ(schRunTaskSch(&sch), SCH_OK);

	// /*  */
	// size_t bufSize = 512;
	// char *buf = (char *)malloc(bufSize);

	// Ref<IScheduler> refPtr = Ref<IScheduler>(sch);

	// // BufferIO
	// ASync async(refPtr);
	// BufferIO bufferIo(bufSize);
	// Ref<IO> redBufIO = Ref<IO>(&bufferIo);
	// ASSERT_NO_THROW(handle = async.asyncOpen(redBufIO));
	// ASSERT_NO_THROW(async.asyncWriteFile(handle, buf, bufSize,
	// writecallback)); ASSERT_NO_THROW(async.asyncWait(handle));
	// ASSERT_NO_THROW(async.asyncClose(handle));

	// // FileSystem Async.
	// FileSystem *fileSystem;
	// ASSERT_NO_THROW(fileSystem = FileSystem::createFileSystem(refPtr));
	// ASSERT_NO_THROW(handle = fileSystem->openASyncFile(files[0], IO::READ));
	// ASSERT_GE(handle, 0);
	// ASSERT_NO_THROW(fileSystem->asyncReadFile(handle, buf, bufSize,
	// readcallback)); ASSERT_NO_THROW(fileSystem->asyncWait(handle));
	// ASSERT_NO_THROW(fileSystem->asyncClose(handle));

	// // FileSystem ZIP over IO object.
	// Ref<IO> io = Ref<IO>(fileSystem->openFile(zipFile, IO::READ));
	// ZipFileSystem *zipFileSystemIO;
	// ASSERT_NO_THROW(zipFileSystemIO = ZipFileSystem::createZipFileObject(io,
	// refPtr)); handle = zipFileSystemIO->openASyncFile(zipInFiles[0],
	// IO::READ); ASSERT_GE(handle, 0);
	// ASSERT_NO_THROW(zipFileSystemIO->asyncReadFile(handle, buf, bufSize,
	// readcallback)); ASSERT_NO_THROW(zipFileSystemIO->asyncWait(handle));
	// zipFileSystemIO->asyncClose(handle);
	// io->close();
	// delete zipFileSystemIO;

	// /**/
	// ZipFileSystem *zipFileSystem = nullptr;
	// ASSERT_NO_THROW(zipFileSystem = ZipFileSystem::createZipFileObject(zipFile,
	// refPtr)); handle = zipFileSystemIO->openASyncFile(zipInFiles[0],
	// IO::READ); ASSERT_GE(handle, 0);
	// ASSERT_NO_THROW(zipFileSystem->asyncReadFile(handle, buf, bufSize,
	// readcallback)); ASSERT_NO_THROW(zipFileSystem->asyncWait(handle));
	// zipFileSystem->asyncClose(handle);

	// free(buf);
	// EXPECT_EQ(schReleaseTaskSch(&sch), SCH_OK);
}