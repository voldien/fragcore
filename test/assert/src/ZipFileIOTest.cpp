
#include <Core/IO/ZipFileSystem.h>
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <gtest/gtest.h>

using namespace fragcore;

TEST(ZipFileIO, Invalid_Path_Throw_Exception) { ASSERT_ANY_THROW(ZipFileSystem::createZipFileObject("")); }
