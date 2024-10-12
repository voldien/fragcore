#include <Core/Hash.h>
#include <Core/RefPtr.h>
#include <FragCore.h>
#include <IO/BufferIO.h>
#include <IO/FileIO.h>
#include <IO/FileSystem.h>
#include <IO/ZipFileIO.h>
#include <IO/ZipFileSystem.h>
#include <gtest/gtest.h>

using namespace fragcore;
template <typename T>
class HashBaseUnitTest : public ::testing::TestWithParam<std::tuple<std::string, Hash::ALGORITHM, T>> {};

using HashMD5Test = HashBaseUnitTest<std::string>;

TEST_P(HashMD5Test, ComputeHashCorrect) {
	auto [str, hash_algo, expected] = GetParam();
	Hash hash(hash_algo);

	EXPECT_NO_THROW(hash.update(str.data(), str.size()));
	ASSERT_EQ(hash.getByteRead(), str.size());

	const size_t mdSize = 16;
	ASSERT_EQ(hash.getResultSize(), mdSize);

	/*	*/
	std::vector<unsigned char> md5(hash.getResultSize());
	EXPECT_NO_THROW(hash.final(md5));

	/*	*/
	ASSERT_TRUE(memcmp(md5.data(), expected.data(), hash.getResultSize()) == 0);
}

INSTANTIATE_TEST_SUITE_P(ComputeHash, HashMD5Test,
						 ::testing::Values(std::make_tuple("Hello World!", Hash::ALGORITHM::MD5,
														   "ed076287532e86365e841e92bfc50d8c")));
