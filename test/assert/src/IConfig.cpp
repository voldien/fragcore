#include <Core/IConfig.h>
#include <gtest/gtest.h>
using namespace fragcore;

class ConfigSetGet : public ::testing::TestWithParam<std::tuple<std::vector<unsigned int>, std::vector<unsigned int>>> {
};

TEST_P(ConfigSetGet, Default_Constructor_No_Throw) { ASSERT_NO_THROW(IConfig config); }

TEST_P(ConfigSetGet, Get_Reference_No_Throw) {
	IConfig config;
	int rand_value = rand();
	config.set("key", rand_value);
	EXPECT_EQ(config.get<int>("key"), rand_value);
}

TEST_P(ConfigSetGet, Get_Sub_Config) {
	IConfig config;
	ASSERT_NO_THROW(IConfig &sub_conf = config.getSubConfig("candy"));
}

TEST_P(ConfigSetGet, Get_Sub_Config_Values) {
	IConfig config;
	IConfig &sub_conf = config.getSubConfig("candy");

	int rand_value = rand();
	config.set("key", rand_value);
	EXPECT_EQ(config.get<int>("key"), rand_value);
}

INSTANTIATE_TEST_SUITE_P(Base, ConfigSetGet,
						 ::testing::Values(std::make_tuple(std::vector<unsigned int>{}, std::vector<unsigned int>{})));

template <class T> class ConfigSetGetType : public ::testing::Test {};
TYPED_TEST_SUITE_P(ConfigSetGetType);

TYPED_TEST_P(ConfigSetGetType, Default_Set_Get_Constructor_No_Throw) {
	IConfig config;
	ASSERT_NO_THROW(config.set("key", TypeParam()));
	ASSERT_NO_THROW(config.get<TypeParam>("key"));
}

REGISTER_TYPED_TEST_SUITE_P(ConfigSetGetType, Default_Set_Get_Constructor_No_Throw);
using ConfigSetGetRegisterTypes = ::testing::Types<uint32_t, uint16_t, float, double, long int>;
INSTANTIATE_TYPED_TEST_SUITE_P(ConfigRegister, ConfigSetGetType, ConfigSetGetRegisterTypes);