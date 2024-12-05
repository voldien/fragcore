
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

// TEST_F(ZipFileSystem, IOGZipFile) {

// 	ZipFileSystem *zipFile;
// 	ASSERT_THROW(zipFile = ZipFileSystem::createZipFileObject(""), std::invalid_argument);
// 	ASSERT_NO_THROW(zipFile = ZipFileSystem::createZipFileObject(this->zipFile));
// 	// TODO add listing of files.
// 	std::vector<std::string> baseDir = zipFile->listDirectories("");
// 	std::vector<std::string>::const_iterator listIt = baseDir.cbegin();
// 	for (; listIt != baseDir.cend(); listIt++) {
// 		std::vector<std::string> files = zipFile->listFiles((*listIt).c_str());
// 		files.size();
// 	}
// 	// TODO check the references.
// 	ZipFileIO *zileFileIo = (ZipFileIO *)zipFile->openFile(zipInFiles[0], IO::READ);
// 	delete zipFile;
// 	zileFileIo->close();
// 	ASSERT_NO_THROW(zipFile = ZipFileSystem::createZipFileObject(this->zipFile));
// 	ASSERT_THROW(zipFile->openFile("none-existing.txt", IO::READ), std::invalid_argument);
// 	/*  */
// 	std::vector<char *>::const_iterator it = zipInFiles.cbegin();
// 	for (; it != zipInFiles.cend(); it++) {
// 		char *path = (*it);
// 		ZipFileIO *zileFileIo = (ZipFileIO *)zipFile->openFile(path, IO::READ);
// 	}
// }