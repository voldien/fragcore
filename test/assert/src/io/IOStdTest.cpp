#include <Core/IO/FileSystem.h>
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <gtest/gtest.h>
using namespace fragcore;

TEST(StandardIOTest, Get_Standard_File_No_Throw_Exception) {
	ASSERT_NO_THROW(SystemInfo::getStdOut());
	ASSERT_NO_THROW(SystemInfo::getStdIn());
	ASSERT_NO_THROW(SystemInfo::getStdErr());
}

TEST(StandardIOTest, Stdout_Readable_No_Exception) { ASSERT_TRUE(SystemInfo::getStdOut()->readable()); }
TEST(StandardIOTest, Stderr_Readable_No_Exception) { ASSERT_FALSE(SystemInfo::getStdErr()->readable()); }
TEST(StandardIOTest, Stdin_Readable_No_Exception) { ASSERT_FALSE(SystemInfo::getStdIn()->readable()); }

TEST(StandardIOTest, Send_Stout_No_throw) {
	char buf[] = {'h', 'e', 'l', 'l', 'o', '\n'};
	const unsigned int nBytes = sizeof(buf);
	Ref<IO> io_stdout = SystemInfo::getStdOut();
	ASSERT_NO_THROW(io_stdout->write(nBytes, buf));
}

TEST(StandardIOTest, IO_STD_STDOUT_ReadOnly) {}
TEST(StandardIOTest, IO_STD_STDIN_WriteOnly) {}
TEST(StandardIOTest, IO_STD_Read_No_Throw) {}
TEST(StandardIOTest, IO_STD_Write_STDIN_Throw_Exception) {}

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