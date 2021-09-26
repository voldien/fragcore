#include <Core/IO/BufferIO.h>
#include <Core/IO/FileIO.h>
#include <Core/IO/FileSystem.h>
#include <Core/IO/GZFileIO.h>
#include <Core/IO/ZipFileIO.h>
#include <Core/IO/ZipFileSystem.h>
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <SDL2/SDL.h>
#include <Window/Display.h>
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

	static void readcallback(ASync *async, ASyncHandle handle) {}

	static void writecallback(ASync *aSync, ASyncHandle handle) {}

	std::vector<char *> files;
	char *FileWrite;
	std::vector<char *> gzFiles;
	char *gzFileWrite;
	std::vector<char *> zipInFiles;
	char *zipFile;
};

TEST_F(IOTest, IO_File_Invalid_Path_Throw_Exception) {
	ASSERT_THROW(FileSystem::getFileSystem()->openFile("", IO::READ), InvalidArgumentException);
}
TEST_F(IOTest, IO_File_Invalid_Mode_Throw_Exception) {
	ASSERT_THROW(FileSystem::getFileSystem()->openFile(__FILE__, (IO::IOMode)-1), InvalidArgumentException);
}
TEST_F(IOTest, IO_File_Open_Read_File_No_Throw) {
	ASSERT_NO_THROW(FileSystem::getFileSystem()->openFile(__FILE__, IO::READ));
}

TEST_F(IOTest, IO_File_Open_Read_No_Exception) {
	char buf[512];
	IO *io = FileSystem::getFileSystem()->openFile(__FILE__, IO::READ);

	// ASSERT_NO_THROW(io->read(buf, sizeof(buf)));

	FileSystem::getFileSystem()->closeFile(io);
}

TEST_F(IOTest, IO_File_Write_In_ReadOnly_Throw_Exception) {
	char buf[512];
	IO *io = FileSystem::getFileSystem()->openFile(__FILE__, IO::READ);
	ASSERT_THROW(io->write(0, buf), RuntimeException);

	FileSystem::getFileSystem()->closeFile(io);
}

TEST_F(IOTest, IOFile) {
	char buf[512];
	long size;

	ASSERT_THROW(FileIO("", IO::READ), InvalidArgumentException);

	/*	*/
	FileIO write = FileIO(FileWrite, IO::WRITE);
	ASSERT_FALSE(write.isReadable());
	ASSERT_TRUE(write.isWriteable());

	std::vector<char *>::const_iterator it = files.cbegin();
	for (; it != files.cend(); it++) {
		FileIO *fileIo;
		ASSERT_NO_THROW(fileIo = new FileIO((*it), IO::IOMode::READ));
		ASSERT_TRUE(fileIo->isReadable());
		ASSERT_FALSE(fileIo->isWriteable());

		/*  */
		while ((size = fileIo->read(512, buf)) > 0) {
			EXPECT_EQ(SystemInfo::getStdOut()->write(size, buf), size);
			EXPECT_EQ(write.write(size, buf), size);
		}
		delete fileIo;
	}

	write.flush();
	ASSERT_NO_THROW(write.close());
}

TEST_F(IOTest, IO_STD_STDOUT_ReadOnly) {

}
TEST_F(IOTest, IO_STD_STDIN_WriteOnly) {

}
TEST_F(IOTest, IO_STD_Read_No_Throw) {

}
TEST_F(IOTest, IO_STD_Write_STDIN_Throw_Exception) {
	
}

TEST_F(IOTest, Std) {
	// char buf[] = {'h', 'e', 'l', 'l', 'o', '\n'};
	// const unsigned int nBytes = sizeof(buf);

	// /*  */
	// ASSERT_EQ(stdoutIO->write(nBytes, buf), nBytes);
	// EXPECT_NO_THROW(stdoutIO->write(nBytes, buf));
	// EXPECT_NO_THROW(stdoutIO->flush());
	// ASSERT_STREQ(stdoutIO->getName().c_str(), "stdout");
	// ASSERT_FALSE(stdoutIO->isReadable());

	// /*  */
	// ASSERT_THROW(stdinIO->write(nBytes, buf), RuntimeException);
	// ASSERT_STREQ(stdinIO->getName().c_str(), "stdin");
	// ASSERT_TRUE(stdinIO->isReadable());

	// /*  */
	// ASSERT_EQ(stderrIO->write(nBytes, buf), nBytes);
	// EXPECT_NO_THROW(stderrIO->write(nBytes, buf));
	// EXPECT_NO_THROW(stderrIO->flush());
	// ASSERT_STREQ(stderrIO->getName().c_str(), "stderr");

	// /*  */
	// EXPECT_NO_THROW(stdinIO->close());
	// EXPECT_NO_THROW(stderrIO->close());
	// EXPECT_NO_THROW(stdoutIO->close());
}

/*	TODO relocate to its own source file.	*/
TEST_F(IOTest, IO_Buffer_Create_No_Execption) {
	const char *buffer = "hello world!";
	const unsigned int bufLen = strlen(buffer);
	EXPECT_NO_THROW(BufferIO(buffer, bufLen));
}

TEST_F(IOTest, IO_Buffer_Create_Correct_Buffer_Size) {
	const char *buffer = "hello world!";
	const unsigned int bufLen = strlen(buffer);
	BufferIO bufferIO(buffer, bufLen);

	ASSERT_EQ(bufferIO.length(), bufLen);
}

TEST_F(IOTest, IO_Buffer_Create_Constant_Read_Only) {
	const char *buffer = "hello world!";
	const unsigned int bufLen = strlen(buffer);
	BufferIO bufferIO(buffer, bufLen);
	ASSERT_TRUE(bufferIO.isReadable());
	ASSERT_FALSE(bufferIO.isWriteable());
}

TEST_F(IOTest, IO_Buffer_Created_Constant_Read_Only_Write_Throw_Exception) {
	const char *buffer = "hello world!";
	const unsigned int bufLen = strlen(buffer);
	BufferIO bufferIO(buffer, bufLen);

	// ASSERT_THROW(bufferIO.write(1, {1}), RuntimeException);
}

// TODO disolve to each own function.
TEST_F(IOTest, Buffer) {
	const char *buffer = "hello world!";
	unsigned int bufLen = strlen(buffer);
	char buf[5];
	BufferIO *bIO;

	EXPECT_NO_THROW(bIO = new BufferIO(buffer, strlen(buffer)));
	EXPECT_EQ(bIO->length(), bufLen);
	EXPECT_EQ(bIO->getPos(), 0);
	EXPECT_TRUE(bIO->isReadable());
	EXPECT_FALSE(bIO->isWriteable());
	EXPECT_NO_THROW(bIO->read(5, buf));
	delete bIO;

	char buf2[512];
	bufLen = sizeof(buf2);
	EXPECT_NO_THROW(bIO = new BufferIO(buf2, bufLen));
	EXPECT_EQ(bIO->length(), bufLen);
	EXPECT_EQ(bIO->getPos(), 0);
	EXPECT_TRUE(bIO->isReadable());
	EXPECT_TRUE(bIO->isWriteable());
	delete bIO;
}

TEST_F(IOTest, IOGzFile) {
	char buf[512];
	long size;

	// Validate the argument.
	ASSERT_THROW(GZFileIO(NULL, IO::READ), std::invalid_argument);
	ASSERT_THROW(GZFileIO("", IO::READ), RuntimeException);
	ASSERT_THROW(GZFileIO("", (IO::IOMode)33213123), std::invalid_argument);

	GZFileIO write = GZFileIO(gzFileWrite, IO::WRITE);
	ASSERT_FALSE(write.isReadable());
	ASSERT_TRUE(write.isWriteable());

	std::vector<char *>::const_iterator it = gzFiles.cbegin();
	for (; it != gzFiles.cend(); it++) {
		GZFileIO *fileIo;
		ASSERT_NO_THROW(fileIo = new GZFileIO((*it), IO::IOMode::READ));
		ASSERT_TRUE(fileIo->isReadable());
		ASSERT_FALSE(fileIo->isWriteable());

		while ((size = fileIo->read(512, buf)) > 0) {
			EXPECT_EQ(SystemInfo::getStdOut()->write(size, buf), size);
			EXPECT_EQ(write.write(size, buf), size);
		}
		delete fileIo;
	}

	write.flush();
	ASSERT_NO_THROW(write.close());
}

TEST_F(IOTest, IOGZipFile) {
	ZipFileSystem *zipFile;
	ASSERT_THROW(zipFile = ZipFileSystem::createZipFileObject(""), std::invalid_argument);
	ASSERT_NO_THROW(zipFile = ZipFileSystem::createZipFileObject(this->zipFile));

	// TODO add listing of files.
	std::vector<std::string> baseDir = zipFile->listDirectories("");
	std::vector<std::string>::const_iterator listIt = baseDir.cbegin();
	for (; listIt != baseDir.cend(); listIt++) {
		std::vector<std::string> files = zipFile->listFiles((*listIt).c_str());
		files.size();
	}

	// TODO check the references.
	ZipFileIO *zileFileIo = (ZipFileIO *)zipFile->openFile(zipInFiles[0], IO::READ);
	delete zipFile;
	zileFileIo->close();

	ASSERT_NO_THROW(zipFile = ZipFileSystem::createZipFileObject(this->zipFile));
	ASSERT_THROW(zipFile->openFile("none-existing.txt", IO::READ), std::invalid_argument);

	/*  */
	std::vector<char *>::const_iterator it = zipInFiles.cbegin();
	for (; it != zipInFiles.cend(); it++) {
		char *path = (*it);

		ZipFileIO *zileFileIo = (ZipFileIO *)zipFile->openFile(path, IO::READ);
	}
}
