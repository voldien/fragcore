#include <Core/Hash.h>
#include <Core/IO/BufferIO.h>
#include <Core/IO/FileIO.h>
#include <Core/IO/FileSystem.h>
#include <Core/IO/ZipFileIO.h>
#include <Core/IO/ZipFileSystem.h>
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <SDL2/SDL.h>
#include <Window/Display.h>
#include <gtest/gtest.h>

#include "Core/IO/GZFileIO.h"

using namespace fragcore;

class HashMD5Test : public ::testing::TestWithParam<std::tuple<std::string, unsigned long int>> {};

TEST_P(HashMD5Test, Values) {
	auto [x, expected] = GetParam();
	Hash hash(Hash::ALGORITHM::MD5);

	EXPECT_NO_THROW(hash.update(x.data(), x.size()));
	std::vector<unsigned char> md5;
	EXPECT_NO_THROW(hash.final(md5));

	// TODO Check the value
}

INSTANTIATE_TEST_SUITE_P(Hash, HashMD5Test, ::testing::Values(std::make_tuple("Hello World!", 128)));

