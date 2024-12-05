#include "BZFileIO.h"
#include "FragDef.h"
#include "LZ4IO.h"
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <GZFileIO.h>
#include <IO/BufferIO.h>
#include <IO/FileIO.h>
#include <IO/FileSystem.h>
#include <IO/ZipFileIO.h>
#include <IO/ZipFileSystem.h>
#include <gtest/gtest.h>

using namespace fragcore;

template <class T> class IOTest : public ::testing::Test {
  protected:
	void SetUp() override {
		files.emplace_back("CompressionIOTest.cpp");
		files.emplace_back("CMakeLists.txt");
		files.emplace_back("CompressionTest.cpp");
		FileWrite = "tmp.txt";

		gzFiles.emplace_back("");
		gzFiles.emplace_back("");
		gzFiles.emplace_back("");
		gzFileWrite = "tmp.gz";
	}

	std::vector<std::string> files;
	std::string FileWrite;
	std::vector<std::string> gzFiles;
	std::string gzFileWrite;
};
TYPED_TEST_SUITE_P(IOTest);

TYPED_TEST_P(IOTest, InvalidAccess) {

	// Validate the argument.
	ASSERT_THROW(TypeParam(nullptr, IO::READ), InvalidPointerException);
	ASSERT_THROW(TypeParam("", IO::READ), InvalidArgumentException);
	ASSERT_THROW(TypeParam("", (IO::IOMode)33213123), InvalidArgumentException);
	// ASSERT_THROW(TypeParam("/", IO::READ), RuntimeException);
}

TYPED_TEST_P(IOTest, WriteDeflate) {
	std::array<uint8_t, 512> buf;
	long raw_size_bytes = 0;

	TypeParam *io_writer = nullptr;
	ASSERT_NO_THROW(io_writer = new TypeParam(this->gzFileWrite.c_str(), IO::WRITE));

	ASSERT_FALSE(io_writer->isReadable());
	ASSERT_TRUE(io_writer->isWriteable());

	std::vector<std::string>::const_iterator it = this->files.cbegin();
	for (; it != this->files.cend(); it++) {

		FileIO *fileIo = nullptr;
		ASSERT_NO_THROW(fileIo = new FileIO((*it).c_str(), IO::IOMode::READ));

		ASSERT_TRUE(fileIo->isReadable());
		ASSERT_FALSE(fileIo->isWriteable());

		while ((raw_size_bytes = fileIo->read(buf.size(), buf.data())) > 0) {
			EXPECT_LT(io_writer->write(raw_size_bytes, buf.data()), raw_size_bytes);
		}
		fileIo->close();
		delete fileIo;
	}
	ASSERT_NO_THROW(io_writer->flush());
	ASSERT_NO_THROW(io_writer->close());
	delete io_writer;
}

REGISTER_TYPED_TEST_SUITE_P(IOTest, InvalidAccess, WriteDeflate);

using IOCompressionTypes = ::testing::Types<LZ4IO, BZFileIO, GZFileIO>;
INSTANTIATE_TYPED_TEST_SUITE_P(Compression, IOTest, IOCompressionTypes);