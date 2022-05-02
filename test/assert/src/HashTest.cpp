#include <Core/Hash.h>
#include <Core/IO/BufferIO.h>
#include <Core/IO/FileIO.h>
#include <Core/IO/FileSystem.h>
#include <Core/IO/ZipFileIO.h>
#include <Core/IO/ZipFileSystem.h>
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <gtest/gtest.h>

#include "Core/IO/GZFileIO.h"

using namespace fragcore;
template <typename T>
class HashBaseUnitTest : public ::testing::TestWithParam<std::tuple<std::string, Hash::ALGORITHM, T>> {};

using HashMD5Test = HashBaseUnitTest<std::vector<unsigned char>>;

TEST_P(HashMD5Test, ComputeHashCorrect) {
	auto [x, hash_algo, expected] = GetParam();
	Hash hash(hash_algo);

	EXPECT_NO_THROW(hash.update(x.data(), x.size()));

	const size_t mdSize = 16;
	ASSERT_EQ(hash.getResultSize(), mdSize);
	std::vector<unsigned char> md5;
	EXPECT_NO_THROW(hash.final(md5));

	ASSERT_TRUE(md5 == expected);
}

// ed076287532e86365e841e92bfc50d8c
INSTANTIATE_TEST_SUITE_P(ComputeHash, HashMD5Test,
							 ::testing::Values(std::make_tuple("Hello World!", Hash::ALGORITHM::MD5,
															   std::vector<unsigned char>{
																   65, 64, 30, 37, 36, 32, 38, 37, 35, 33, 32,
																   65, 38, 36, 33, 36, 35, 65, 38, 34, 31, 65,
																   39, 32, 62, 66, 63, 35, 30, 64, 38, 63})));
