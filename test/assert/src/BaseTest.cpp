#include <Core/Base16.h>
#include <Core/Base32.h>
#include <Core/Base64.h>
#include <gtest/gtest.h>

using namespace fragcore;

class BaseTest : public ::testing::TestWithParam<std::tuple<std::vector<unsigned int>, std::vector<unsigned int>>> {};
// Base32 base32;
// Base64 base64;
TEST_P(BaseTest, Compare) {
	//auto [x, min, max, expected] = GetParam();
	//

	//EXPECT_FLOAT_EQ(clampedValue, expected);
}

// INSTANTIATE_TEST_SUITE_P(Base, BaseTest,
// 						 ::testing::Values(std::make_tuple(5, 3, 4, 4), std::make_tuple(1, 3, 4, 3),
// 										   std::make_tuple(-1000, 3, 4, 3)));
