#include<gtest/gtest.h>
#include<Core/Math.h>

using namespace fragcore;

TEST(Math, Clamp_Max_Value){
	ASSERT_EQ(Math::clamp(5, 3, 4), 4);
}
TEST(Math, Clamp_Min_Value){
	ASSERT_EQ(Math::clamp(1, 3, 4), 3);
}
TEST(Math, Min_Min_Value){
	ASSERT_EQ(Math::min(5, 3), 3);
}
TEST(Math, Max_Max_Value)
{
	ASSERT_EQ(Math::max(1, 3), 3);
}

TEST(Math, PowerOf2_Found){
	for (int i = 0; i < 32; i++)
		ASSERT_TRUE(Math::IsPowerOfTwo(1 << i));
}
TEST(Math, PowerOf2_Next_Found){
	//TODO improve.
	for (int i = 0; i < 31; i++)
	{
		unsigned int p2 = Math::NextPowerOfTwo(1 << i);
		ASSERT_TRUE(Math::IsPowerOfTwo(p2));
		ASSERT_TRUE(p2 == (1 << (i + 1)));
	}
}

TEST(Math, Sum_Float_EQ){
	std::vector<float> seq = {1, 2, 3, 4, 5};
	ASSERT_EQ(Math::sum(seq), 15);
}


TEST(Math, Distrubtion){

	/*	Gamma space */
	const int sample_space = 2048;
	float pre_gamma = 0.0f;
	for (int i = 0; i < sample_space; i++)
	{
		float linear = (float)i / (float)sample_space;
		float gamma = Math::gammaCorrection(linear, 2.2f);
		ASSERT_TRUE(gamma > pre_gamma);
		pre_gamma = gamma;
	}

	/*	Guassian distribution.	*/
	//TODO add
	const float theta = 0.7f;
	const int num_guass = 5;
	const int num_total_guass = num_guass * 2 + 1;
	std::vector<float> guassian;
	Math::guassian1Df(guassian, num_guass, theta);
	ASSERT_EQ(guassian.size(), num_total_guass);
	float sum = Math::sum(guassian);
	ASSERT_FLOAT_EQ(sum, 1.0f);

	Math::guassian2Df(guassian, num_guass, theta);
}