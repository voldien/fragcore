#include<gtest/gtest.h>
#include<Core/IConfig.h>
#include<Core/IO/BufferIO.h>
#include<Core/IO/IO.h>
#include<Core/IO/FileSystem.h>
#include<Core/Ref.h>
#include"IConfigTest.h"

#define CONFIG_DEFAULT_NAME "Config"

using namespace fragcore;

void IConfigTest::TearDown() {
	Test::TearDown();
}

void IConfigTest::SetUp() {
	Test::SetUp();
	this->config = new IConfig();
	this->config->setName(CONFIG_DEFAULT_NAME);
	this->config->set<int>("a", 0);
	this->config->set<int>("b", 1);
	this->config->set<int>("c", 2);
}

TEST(Config, ICOnfig_Default_Constructor_No_Throw){
	ASSERT_NO_THROW(IConfig());
}

TEST(Config, Create_SubConfig_No_Throw){
	IConfig iconfig;
	ASSERT_NO_THROW(iconfig.getSubConfig("child"));
}

TEST(Config, XMLLoadConfiguration) {
	const char *arg[] = {
		"fragview",
	};
	const int nArg = sizeof(arg) / sizeof(arg[0]);

	FileSystem::createFileSystem();
	Ref<IO> io = Ref<IO>(
	FileSystem::getFileSystem()->openFile("config.xml", IO::READ));
	IConfig *config = new IConfig();
	EXPECT_NO_THROW(config->parseConfigFile(io, IConfig::XML));

	FileSystem::getFileSystem()->closeFile(*io);
}

TEST(Config, XMLSaveConfiguration) {
	
}

TEST(Config, YAMLLoadConfiguration) {
	const char *arg[] = {

	};
	const int nArg = sizeof(arg) / sizeof(arg[0]);

	//Config *config = Config::createConfig(nArg, arg, "config.yaml");
}

TEST(Config, YAMLSaveConfiguration) {

}

TEST_F(IConfigTest, Copy) {
	IConfig config = *this->config;
	IConfig *pConfig = &config;

	//TODO
	/*  Assert true.    */
	for (int i = 0; i < config.getNumChildren() - 1; i++) {
		//IConfig *subConfig = config.getChild(i);
	}

}

TEST_F(IConfigTest, Set) {
	const char *name = "The current name of the config object";
	IConfig *config = new IConfig();
	ASSERT_NO_THROW(config->setName(name));
	ASSERT_STREQ(config->getName().c_str(), name);

	ASSERT_NO_THROW(config->set("a", 0));
	ASSERT_NO_THROW(config->get<int>("a"));
	ASSERT_EQ(config->get<int>("a"), 0);
	ASSERT_NO_THROW(config->set("a", 1));
	ASSERT_NO_THROW(config->get<int>("a"));
	ASSERT_EQ(config->get<int>("a"), 1);
}

TEST_F(IConfigTest, Get) {
	ASSERT_STREQ(this->config->getName().c_str(), CONFIG_DEFAULT_NAME);
	ASSERT_EQ(this->config->get<int>("a"), 0);
	ASSERT_EQ(this->config->get<int>("b"), 1);
	ASSERT_EQ(this->config->get<int>("c"), 2);
}

TEST_F(IConfigTest, SubConfig) {
	/*  Copy.   */
	IConfig config = *this->config;
	IConfig *pConfig = &config;

	IConfig *subConfig;
	IConfig *subConfig2;
	ASSERT_NO_THROW(subConfig = &pConfig->getSubConfig("sub"));
	subConfig->setName("subconfig");
	ASSERT_NO_THROW(subConfig2 = &pConfig->getSubConfig("sub"));
	ASSERT_TRUE(*subConfig == *subConfig2);
}

TEST_F(IConfigTest, TypeCheck) {
	IConfig *config = new IConfig();

	config->get<int>("a1");
	config->get<float>("a1");
	config->get<double>("a1");
	config->get<bool>("a1");
	config->get<void *>("a1");
}

TEST_F(IConfigTest, Conditional) {

}