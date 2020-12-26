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


TEST(Hash, Hash_MD5_Compute_No_Throw){
	Hash hash(Hash::ALGORITHM::MD5);
	const char *data = "Hello World!";

	EXPECT_NO_THROW(hash.update(data, sizeof(data)));
	std::vector<unsigned char> md5;
	EXPECT_NO_THROW(hash.final(md5));
}

TEST(Hash, Hash_MD5_Compute_Correct_Value){
	Hash hash(Hash::ALGORITHM::MD5);
	const char *data = "Hello World!";
	const char *md5 = "";
	std::vector<unsigned char> result;

	hash.update(data, sizeof(data));
	hash.final(result);

	/*	TODO compare the value.	*/
}

TEST(Hash, Hash){
	Hash hash(Hash::ALGORITHM::MD5);
	const char *data = "Hello World!";
	EXPECT_NO_THROW (hash.update(data, sizeof(data)));
	ASSERT_EQ(hash.getByteRead(), sizeof(data));
	std::vector<unsigned char> md5;
	EXPECT_NO_THROW(hash.final(md5));
	//TODO add assertion that the value is correct.

	ASSERT_ANY_THROW(Hash badhashType((Hash::ALGORITHM)100));

}


