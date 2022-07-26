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
#include <gtest/gtest.h>
using namespace fragcore;

class ASyncIOTest : public testing::Test {
  public:
	void SetUp() override {}

	Ref<IScheduler> sche{nullptr};
};

TEST_F(ASyncIOTest, Create_No_ThrowException) { ASSERT_NO_THROW(ASyncIO async(this->sche)); }

TEST_F(ASyncIOTest, Create_Handle_No_Throw_Exception) { ASSERT_NO_THROW(ASyncHandle handle); }

TEST_F(ASyncIOTest, Open_IO_Async_Handle_No_Throw) {
	ASyncIO async(this->sche);

	Ref<IO> ioRef = SystemInfo::getStdOut();
	ASyncHandle handle;
	ASSERT_NO_THROW(handle = async.asyncOpen(ioRef));
	ASSERT_NO_THROW(async.asyncClose(handle));
}

TEST_F(ASyncIOTest, Read_IO_Async_ThrowException) {
	ASyncIO async(this->sche);

	Ref<IO> ioRef = SystemInfo::getStdOut();

	ASyncHandle handle = 0;
	handle = async.asyncOpen(ioRef);
	ASSERT_THROW(async.asyncReadFile(handle, nullptr, 0, nullptr), InvalidArgumentException);
	async.asyncClose(handle);
}

TEST_F(ASyncIOTest, Wait_Correctly_NoThrowException) {
	ASyncIO async(this->sche);

	Ref<IO> ioRef = SystemInfo::getStdOut();
	ASyncHandle handle;
	handle = async.asyncOpen(ioRef);
	char buf[32];
	async.asyncReadFile(handle, buf, sizeof(buf), nullptr);
	// ASSERT_NO_THROW(async.asyncWait(handle));
}

TEST(ASyncIO, CallBack_Called_No_Throw) {}

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