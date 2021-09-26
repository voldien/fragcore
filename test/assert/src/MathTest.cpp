#include <Core/Math.h>
#include <gtest/gtest.h>

using namespace fragcore;

class ClampTest : public ::testing::TestWithParam<std::tuple<double, double, double, double>> {};

TEST_P(ClampTest, Values) {
	auto [x, min, max, expected] = GetParam();
	auto clampedValue = Math::clamp(x, min, max);

	EXPECT_FLOAT_EQ(clampedValue, expected);
}

INSTANTIATE_TEST_SUITE_P(Math, ClampTest,
						 ::testing::Values(std::make_tuple(5, 3, 4, 4), std::make_tuple(1, 3, 4, 3),
										   std::make_tuple(-1000, 3, 4, 3)));

class MaxTest : public ::testing::TestWithParam<std::tuple<double, double, double>> {};

TEST_P(MaxTest, Values) {
	auto [x, y, expected] = GetParam();
	auto maxValue = Math::max(x, y);

	EXPECT_FLOAT_EQ(maxValue, expected);
}

INSTANTIATE_TEST_SUITE_P(Math, MaxTest,
						 ::testing::Values(std::make_tuple(5, 3, 5), std::make_tuple(5, 3, 5),
										   std::make_tuple(5, 3, 5)));

class MinTest : public ::testing::TestWithParam<std::tuple<double, double, double>> {};

TEST_P(MinTest, Values) {
	auto [x, y, expected] = GetParam();
	auto minValue = Math::min(x, y);

	EXPECT_FLOAT_EQ(minValue, expected);
}

INSTANTIATE_TEST_SUITE_P(Math, MinTest,
						 ::testing::Values(std::make_tuple(5, 3, 5), std::make_tuple(5, 3, 5),
										   std::make_tuple(5, 3, 5)));

TEST(Math, PowerOf2_Found) {
	for (unsigned int i = 0; i < 64; i++)
		ASSERT_TRUE(Math::IsPowerOfTwo<unsigned long int>(1 << i));
}

TEST(Math, PowerOf2_Next_Found) {

	for (unsigned int i = 0; i < 31; i++) {
		auto p2 = Math::NextPowerOfTwo<unsigned long int>(1 << i);
		ASSERT_TRUE(Math::IsPowerOfTwo(p2));
		ASSERT_TRUE(p2 == (1 << (i + 1)));
	}
}

class SumTest : public ::testing::TestWithParam<std::tuple<std::vector<float>, float>> {};

TEST_P(SumTest, Values) {
	auto [x, expected] = GetParam();
	auto sum = Math::sum(x);

	EXPECT_NEAR(sum, expected, 10e-8);
}

INSTANTIATE_TEST_SUITE_P(Math, SumTest,
						 ::testing::Values(std::make_tuple(std::vector<float>{1, 2, 3, 4, 5}, 15),
										   std::make_tuple(std::vector<float>{5, 5, 5, 5, 5}, 25)));

class GuassianDistributionTest : public ::testing::TestWithParam<std::tuple<float, float,std::vector<float>>> {};

TEST_P(GuassianDistributionTest, Values) {
	auto [theta, standard_deviation, expected] = GetParam();
	std::vector<float> guass;

	Math::guassian(guass, expected.size(), theta, standard_deviation);

	ASSERT_EQ(expected.size(), guass.size());

	float sum = Math::sum(guass);
	EXPECT_NEAR(sum, 1.0f, 10e-8);

	//TODO check whole vector
	//EXPECT_NEAR(sum, expected, 10e-8);
}

INSTANTIATE_TEST_SUITE_P(Math, GuassianDistributionTest,
						 ::testing::Values(std::make_tuple(0.5, 0.5, std::vector<float>{1, 2, 3, 4, 5}),
										   std::make_tuple(0.5, 0.5, std::vector<float>{5, 5, 5, 5, 5})));

TEST(Math, Distrubtion) {

	/*	Gamma space */
	const int sample_space = 2048;
	float pre_gamma = 0.0f;
	for (int i = 0; i < sample_space; i++) {
		float linear = (float)i / (float)sample_space;
		float gamma = Math::gammaCorrection(linear, 2.2f);
		ASSERT_TRUE(gamma > pre_gamma);
		pre_gamma = gamma;
	}

	/*	Guassian distribution.	*/
	// TODO add
	const float theta = 0.7f;
	const int num_guass = 5;
	const int num_total_guass = num_guass * 2 + 1;
	std::vector<float> guassian;
	Math::guassian(guassian, num_guass, theta, 0.1f);
	ASSERT_EQ(guassian.size(), num_total_guass);
	float sum = Math::sum(guassian);
	ASSERT_FLOAT_EQ(sum, 1.0f);

	Math::guassian(guassian, num_guass, theta, 0.1f);
}