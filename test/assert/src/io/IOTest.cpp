#include <Core/IO/BufferIO.h>
#include <Core/IO/FileIO.h>
#include <Core/IO/FileSystem.h>
#include <Core/IO/GZFileIO.h>
#include <Core/IO/ZipFileIO.h>
#include <Core/IO/ZipFileSystem.h>
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <gtest/gtest.h>
using namespace fragcore;

class IOTest : public testing::Test {
  protected:
	void SetUp() override {
		files.push_back("Makefile");
		files.push_back("CMakeLists.txt");
		files.push_back("src/IO.cpp");
		FileWrite = "tmp.txt";

		gzFiles.push_back("");
		gzFiles.push_back("");
		gzFiles.push_back("");
		gzFileWrite = "tmp.gz";

		zipInFiles.push_back("glsl/wave.frag");
		zipInFiles.push_back("glsl/sepia.frag");
		zipInFiles.push_back("glsl/sepia.comp");

		zipFile = "zipfile.zip";
	}

	static void readcallback(ASyncIO *async, ASyncHandle handle) {}

	static void writecallback(ASyncIO *aSync, ASyncHandle handle) {}

	std::vector<std::string> files;
	std::string FileWrite;
	std::vector<std::string> gzFiles;
	std::string gzFileWrite;
	std::vector<std::string> zipInFiles;
	std::string zipFile;
};

TEST_F(IOTest, IO_File_Invalid_Path_Throw_Exception) {
	ASSERT_THROW(FileSystem::getFileSystem()->openFile("", IO::READ), InvalidArgumentException);
}
TEST_F(IOTest, IO_File_Invalid_Mode_Throw_Exception) {
	ASSERT_THROW(FileSystem::getFileSystem()->openFile(__FILE__, (IO::IOMode)0), InvalidArgumentException);
}
TEST_F(IOTest, IO_File_Open_Read_File_No_Throw) {
	ASSERT_NO_THROW(FileSystem::getFileSystem()->openFile(__FILE__, IO::READ));
}

TEST_F(IOTest, IO_File_Open_Read_No_Exception) {
	char buf[512];
	IO *io = FileSystem::getFileSystem()->openFile(__FILE__, IO::READ);

	ASSERT_NO_THROW(io->read(sizeof(buf), buf));

	FileSystem::getFileSystem()->closeFile(io);
}

TEST_F(IOTest, IO_File_Write_In_ReadOnly_Throw_Exception) {
	char buf[512];
	IO *io = FileSystem::getFileSystem()->openFile(__FILE__, IO::READ);

	ASSERT_THROW(io->write(sizeof(buf), buf), RuntimeException);

	FileSystem::getFileSystem()->closeFile(io);
}

TEST_F(IOTest, IOFile) {
	const size_t blockSize = 512;
	char buf[blockSize];
	long size;

	ASSERT_THROW(FileIO("", IO::READ), InvalidArgumentException);

	/*	*/
	FileIO write = FileIO(FileWrite, IO::WRITE);
	ASSERT_FALSE(write.isReadable());
	ASSERT_TRUE(write.isWriteable());

	std::vector<std::string>::const_iterator it = files.cbegin();
	for (; it != files.cend(); it++) {
		FileIO *fileIo = nullptr;

		ASSERT_NO_THROW(fileIo = new FileIO((*it), IO::IOMode::READ));
		/*	*/
		ASSERT_TRUE(fileIo->isReadable());
		ASSERT_FALSE(fileIo->isWriteable());

		/*  */
		while ((size = fileIo->read(blockSize, buf)) > 0) {
			EXPECT_EQ(SystemInfo::getStdOut()->write(size, buf), size);
			EXPECT_EQ(write.write(size, buf), size);
		}
		ASSERT_NO_THROW(delete fileIo);
	}

	write.flush();
	ASSERT_NO_THROW(write.close());
}
