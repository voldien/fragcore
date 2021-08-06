#include <gtest/gtest.h>
#include"common.h"
#include<Core/IO/FileSystem.h>
#include<Core/IConfig.h>
#include"Prerequisites.h"

using namespace fragcore;


void CommonBaseTest::SetUp() {
	Test::SetUp();
	Ref<IScheduler> ref;
	EXPECT_NO_THROW(FileSystem::createFileSystem(ref));
}

void CommonBaseTest::TearDown() {
	Test::TearDown();
}

AudioInterface* createAudioInterface(fragcore::AudioFactory::AudioAPI api){
	return AudioFactory::createAudioInterface(api, NULL);
}

IRenderer *createRendererInterface(RenderingFactory::RenderingAPI api, IConfig *config) {
	IRenderer *renderer = NULL;
	const char *argv[] = {
			"fragview"
	};

	//EXPECT_NE(config, NULL);

	EXPECT_NO_THROW(renderer = RenderingFactory::createRendering(api, &config->getSubConfig("render-driver")));
	EXPECT_NE(renderer, nullptr);
	return renderer;
}


void deleteRenderer(IRenderer *renderer) {
	EXPECT_NO_THROW(delete renderer);
}

