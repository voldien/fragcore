#include <Core/IO/BufferIO.h>
#include <Core/RefPtr.h>
#include <gtest/gtest.h>
using namespace fragcore;

class BufferIOTest : public testing::Test {
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

	std::vector<char *> files;
	char *FileWrite;
	std::vector<char *> gzFiles;
	char *gzFileWrite;
	std::vector<char *> zipInFiles;
	char *zipFile;
};

/*	TODO relocate to its own source file.	*/
TEST_F(BufferIOTest, Create_Constant_Pointer_Ref_No_Execption) {
	const char *buffer = "hello world!";
	const unsigned int bufLen = strlen(buffer);
	EXPECT_NO_THROW(BufferIO(buffer, bufLen));
}

TEST_F(BufferIOTest, Create_Correct_Buffer_Size) {
	const char *buffer = "hello world!";
	unsigned int bufLen = strlen(buffer);
	BufferIO bufferIO(buffer, bufLen);

	ASSERT_EQ(bufferIO.length(), bufLen);

	ASSERT_TRUE(bufferIO.isReadable());
	ASSERT_FALSE(bufferIO.isWriteable());

	ASSERT_NO_THROW(bufferIO.close());
}

// TEST_F(BufferIOTest, IO_Buffer_Create_Constant_Read_Only) {
// 	const char *buffer = "hello world!";
// 	const unsigned int bufLen = strlen(buffer);
// 	BufferIO bufferIO(buffer, bufLen);
// 	ASSERT_TRUE(bufferIO.isReadable());
// 	ASSERT_FALSE(bufferIO.isWriteable());
// }

// TEST_F(BufferIOTest, IO_Buffer_Created_Constant_Read_Only_Write_Throw_Exception) {
// 	const char *buffer = "hello world!";
// 	const unsigned int bufLen = strlen(buffer);
// 	BufferIO bufferIO(buffer, bufLen);

// 	// ASSERT_THROW(bufferIO.write(1, {1}), RuntimeException);
// }

// TODO disolve to each own function.
// TEST_F(IOTest, Buffer) {
// 	const char *buffer = "hello world!";
// 	unsigned int bufLen = strlen(buffer);
// 	char buf[5];
// 	BufferIO *bIO;

// 	EXPECT_NO_THROW(bIO = new BufferIO(buffer, strlen(buffer)));
// 	EXPECT_EQ(bIO->length(), bufLen);
// 	EXPECT_EQ(bIO->getPos(), 0);
// 	EXPECT_TRUE(bIO->isReadable());
// 	EXPECT_FALSE(bIO->isWriteable());
// 	EXPECT_NO_THROW(bIO->read(5, buf));
// 	delete bIO;

// 	char buf2[512];
// 	bufLen = sizeof(buf2);
// 	EXPECT_NO_THROW(bIO = new BufferIO(buf2, bufLen));
// 	EXPECT_EQ(bIO->length(), bufLen);
// 	EXPECT_EQ(bIO->getPos(), 0);
// 	EXPECT_TRUE(bIO->isReadable());
// 	EXPECT_TRUE(bIO->isWriteable());
// 	delete bIO;
// }
