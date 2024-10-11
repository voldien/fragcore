
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <IO/ZipFileSystem.h>
#include <gtest/gtest.h>

using namespace fragcore;

TEST(ZipFileSystem, OpenZipFile_Valid_Path_Throw_No_Exception) {
	ASSERT_NO_THROW(ZipFileSystem::createZipFileObject("test.zip"));
}

/*	TODO relocate to its own file later.	*/
TEST(ZipFileSystem, OpenZipFile_Invalid_Path_Throw_Exception) {
	ASSERT_THROW(ZipFileSystem::createZipFileObject(""), InvalidArgumentException);
}

TEST(ZipFileSystem, OpenZipFile_Invalid_Memory_Throw_Exception) {
	const char *buf = "hello world!";
	int len = sizeof(buf);

	ASSERT_THROW(ZipFileSystem::createZipFileObject((void *)buf, len), RuntimeException);
}

TEST(ZipFileSystem, OpenZipFile_Create_with_Task) { // TODO
}

TEST(ZipFileIO, Invalid_Path_Throw_Exception) { ASSERT_ANY_THROW(ZipFileSystem::createZipFileObject("")); }
