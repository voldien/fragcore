// #include <Core/IO/BufferIO.h>
// #include <Core/IO/FileSystem.h>
// #include<Core/SystemInfo.h>
// #include <Renderer/RendererFactory.h>
// #include <RenderingInterfaceTest.h>
// #include <Video/VideoFactory.h>
// #include <gtest/gtest.h>
// using namespace fragcore;

// TEST(VideoPlayer, VideoPlayer_Load_Mp4_Default_No_Throw) {
// 	//	VideoFactory::loadVideoTexture();
// }

// TEST(VideoPlayer, VideoPlayer_Load_Invalid_IO_Throw_Exception) {
// 	VideoFactory::loadVideoTexture(SystemInfo::getStdIn(), nullptr, nullptr, nullptr);
// }

// TEST(VideoPlayer, loadFile) {
// 	// std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

// 	// Ref<IO> ioFile = Ref<IO>(FileSystem::getFileSystem()->openFile("", IO::READ));

// 	// for (; it != apis.end(); it++) {
// 	// 	RenderingFactory::RenderingAPI api = (*it);
// 	// 	SETRENDERAPISCOPE(this, api)
// 	// 	IRenderer *renderer;
// 	// 	VideoTexture* videoTexture;

// 	// 	ASSERT_NO_FATAL_FAILURE(renderer = createRendererInterface(api, this->config));

// 	// 	ASSERT_NO_THROW(videoTexture = VideoFactory::loadVideoTexture(ioFile, NULL, renderer, NULL));
// 	// 	ASSERT_NE(videoTexture->getTexture(), NULL);
// 	// 	ASSERT_FALSE(videoTexture->isPlaying());
// 	// 	ASSERT_NO_THROW(videoTexture->play());
// 	// 	ASSERT_NO_THROW(videoTexture->stop());
// 	// 	ASSERT_FALSE(videoTexture->isPlaying());

// 	// 	ioFile->seek(0, IO::Seek::SET);
// 	// }
// 	// ioFile->close();
// }