#include <Math3D/AABB.h>
#include <Math3D/Line.h>
#include <gtest/gtest.h>

using namespace fragcore;

class LineEquationTest : public ::testing::TestWithParam<std::tuple<double, double, double, double>> {};

TEST_P(LineEquationTest, Values) {
	auto [x, min, max, expected] = GetParam();
	auto clampedValue = Math::clamp(x, min, max);

	EXPECT_FLOAT_EQ(clampedValue, expected);
}

INSTANTIATE_TEST_SUITE_P(Math, LineEquationTest,
						 ::testing::Values(std::make_tuple(5, 3, 4, 4), std::make_tuple(1, 3, 4, 3),
										   std::make_tuple(-1000, 3, 4, 3)));
