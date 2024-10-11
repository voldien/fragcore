#include <Core/RefPtr.h>
#include <FragCore.h>
#include <IO/BufferIO.h>
#include <IO/FileIO.h>
#include <IO/FileSystem.h>
#include <IO/GZFileIO.h>
#include <IO/ZipFileIO.h>
#include <IO/ZipFileSystem.h>
#include <gtest/gtest.h>

using namespace fragcore;

// TEST_F(IOTest, IOGzFile) {
// 	char buf[512];
// 	long size;

// 	// Validate the argument.
// 	ASSERT_THROW(GZFileIO(NULL, IO::READ), std::invalid_argument);
// 	ASSERT_THROW(GZFileIO("", IO::READ), RuntimeException);
// 	ASSERT_THROW(GZFileIO("", (IO::IOMode)33213123), std::invalid_argument);

// 	GZFileIO write = GZFileIO(gzFileWrite, IO::WRITE);
// 	ASSERT_FALSE(write.isReadable());
// 	ASSERT_TRUE(write.isWriteable());

// 	std::vector<char *>::const_iterator it = gzFiles.cbegin();
// 	for (; it != gzFiles.cend(); it++) {
// 		GZFileIO *fileIo;
// 		ASSERT_NO_THROW(fileIo = new GZFileIO((*it), IO::IOMode::READ));
// 		ASSERT_TRUE(fileIo->isReadable());
// 		ASSERT_FALSE(fileIo->isWriteable());

// 		while ((size = fileIo->read(512, buf)) > 0) {
// 			EXPECT_EQ(SystemInfo::getStdOut()->write(size, buf), size);
// 			EXPECT_EQ(write.write(size, buf), size);
// 		}
// 		delete fileIo;
// 	}

// 	write.flush();
// 	ASSERT_NO_THROW(write.close());
// }

// TEST_F(IOTest, IOGZipFile) {
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
