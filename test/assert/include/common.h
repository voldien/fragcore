#ifndef _FRAG_ASSERT_COMMON_H_
#define _FRAG_ASSERT_COMMON_H_ 1
#include<gtest/gtest.h>
#include <Renderer/IRenderer.h>
#include <Renderer/RendererFactory.h>
#include<Audio/AudioFactory.h>
#include<cstddef>

class CommonBaseTest : public testing::Test {
public:
protected:
	void SetUp() override;

	void TearDown() override;
};

extern fragcore::AudioInterface* createAudioInterface(fragcore::AudioFactory::AudioAPI api);
extern fragcore::IRenderer *createOpenGLRenderer(void);
extern fragcore::IRenderer *createRendererInterface(fragcore::RenderingFactory::RenderingAPI api, fragcore::IConfig* config);
extern void deleteRenderer(fragcore::IRenderer *renderer);

#endif
