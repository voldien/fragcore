#include<gtest/gtest.h>
#include <Renderer/RendererFactory.h>
#include<Core/IO/FileSystem.h>
#include<Utils/TextureUtil.h>
#include<Renderer/Sampler.h>
#include <common.h>
#include <Renderer/Query.h>
#include<Core/IO/FileIO.h>
#include <Font.h>
#include<FontFactory.h>
#include <RenderingInterfaceTest.h>
#include<Core/IConfig.h>

using namespace fragcore;

class RenderingInterfaceTest : public CommonBaseTest
{
protected:
	void TearDown() override;

	void SetUp() override;

	std::vector<RenderingFactory::RenderingAPI> apis;
	std::vector<const char *> apiNames;
	Capability capability;
	IConfig *config;
};

void RenderingInterfaceTest::TearDown() {
	Test::TearDown();
	delete this->config;
}

void RenderingInterfaceTest::SetUp() {
	Test::SetUp();

	apis.push_back(RenderingFactory::OpenGL);
	apis.push_back(RenderingFactory::Vulkan);
	//apis.push_back(RenderingFactory::eOpenCL);
	//apis.push_back(RenderingFactory::eDirectX);

	apiNames.push_back("OpenGL");
	apiNames.push_back("Vulkan");
	//apiNames.push_back("OpenCL");
	//apiNames.push_back("DirectX");
	this->config = new IConfig();
	IConfig &renderingConfig = *this->config;
	renderingConfig.set("extensions", "");
	renderingConfig.set("layers", "");
	// OpenGL configuration.
	renderingConfig.set("opengl", -1);
	renderingConfig.set("core", 1);
	// Vulkan configuration.
	renderingConfig.set("vulkan-version", 110);
	// DirectX configuration.
	renderingConfig.set("direct-x-version", 0);
	//
	renderingConfig.set("alpha", false);
	renderingConfig.set("v-sync", true);
	renderingConfig.set("gamma-correction", true);
	renderingConfig.set("debug", false);
	renderingConfig.set("debug-tracer", true);
	renderingConfig.set("anti-aliasing-samples", 0);
	renderingConfig.set("anti-aliasing", false);
	renderingConfig.set("version", "auto");

}

TEST_F(RenderingInterfaceTest, RenderInterface_Create_Enum_OpenGL_No_Throw){
	ASSERT_NO_THROW(RenderingFactory::createRendering(RenderingFactory::OpenGL, NULL));
}

TEST_F(RenderingInterfaceTest, RenderInterface_Delete_Enum_OpenGL_No_Throw)
{
	IRenderer *renderer = RenderingFactory::createRendering(RenderingFactory::OpenGL, NULL);
	ASSERT_NO_THROW(renderer);
}

TEST_F(RenderingInterfaceTest, RenderInterface_Create_Enum_Vulkan_No_Throw)
{
	ASSERT_NO_THROW(RenderingFactory::createRendering(RenderingFactory::Vulkan, NULL));
}

TEST_F(RenderingInterfaceTest, RenderInterface_Delete_Enum_Vulkan_No_Throw)
{
	IRenderer *renderer = RenderingFactory::createRendering(RenderingFactory::Vulkan, NULL);
	ASSERT_NO_THROW(renderer);
}

//TODO add once directX is being adding.
// TEST_F(RenderingInterfaceTest, RenderInterface_Create_Enum_DirectX_No_Throw){
// 	ASSERT_NO_THROW(RenderingFactory::createRendering(RenderingFactory::OpenGL, NULL));
// }

TEST_F(RenderingInterfaceTest, RenderInterface_Enum_OpenGL_Get_Capabilites_No_Throw)
{
	IRenderer* renderer = RenderingFactory::createRendering(RenderingFactory::OpenGL, NULL);
	Capability capability;
	ASSERT_NO_THROW(renderer->getCapability(&capability));
}

TEST_F(RenderingInterfaceTest, RenderInterface_Enum_OpenGL_Create_White_Texture_No_Throw){
	IRenderer *renderer = RenderingFactory::createRendering(RenderingFactory::OpenGL, NULL);
	const int size = 0;
	TextureDesc desc;
	desc.originalTexture = NULL;
	desc.nrSamples = 0;
	desc.target = TextureDesc::eTexture2D;
	desc.width = 1;
	desc.height = 1;
	desc.depth = 1;
	desc.type = TextureDesc::eUnsignedByte;
	desc.internalformat = TextureDesc::Format::eRGBA;
	desc.format = TextureDesc::Format::eRGBA;
//	desc.usemipmaps = mipmap;
//	desc.pixel = pixels;
	desc.pixelSize = size;
	desc.numlevel = 5;
	desc.compression = TextureDesc::Compression::eNoCompression;
	desc.sampler.AddressU = SamplerDesc::eRepeat;
	desc.sampler.AddressV = SamplerDesc::eRepeat;
	desc.sampler.AddressW = SamplerDesc::eRepeat;
	desc.sampler.magFilter = SamplerDesc::eLinear;
	desc.sampler.minFilter = SamplerDesc::eLinear;
	desc.sampler.mipmapFilter = SamplerDesc::eLinear;
	desc.sampler.compareFunc = SamplerDesc::CompareFunc::eNoCompare;
	desc.sampler.compareMode = 0;
	desc.sampler.anisotropy = 0;
	desc.Swizzlea = TextureDesc::eNoSwizzle;
	desc.Swizzler = TextureDesc::eNoSwizzle;
	desc.Swizzleg = TextureDesc::eNoSwizzle;
	desc.Swizzleb = TextureDesc::eNoSwizzle;
	ASSERT_NO_THROW(renderer->createTexture(&desc));
}

TEST_F(RenderingInterfaceTest, RenderInterface_Enum_OpenGL_Create_Buffer_No_Throw)
{
	// BufferDesc::BufferType target = targets[tar];
	// BufferDesc::BufferHint hint = (BufferDesc::BufferHint)(hints1[t] | hints2[h]);

	// /**/
	// BufferDesc desc;
	// desc.size = size;
	// desc.type = target;
	// desc.hint = hint;
	// desc.data = NULL;
	// desc.marker.markerName = "Buffer";

	// /**/
	// EXPECT_NO_THROW(buffer = renderer->createBuffer(&desc));
	// ASSERT_EQ(buffer->getRenderer(), renderer);
	// ASSERT_EQ(buffer->getSize(), size);
	// ASSERT_NE(buffer->mapBuffer(Buffer::eRead), NULL);

	// /**/
	// EXPECT_NO_THROW(renderer->deleteBuffer(buffer));
}

TEST_F(RenderingInterfaceTest, CreateInterface)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();
	const char *argv[] = {
		"fragview"};

	for (; it != apis.end(); it++)
	{
		//		RenderingFactory::RenderingAPI api = (*it);
		//		SETRENDERAPISCOPE(this, api)
		//
		//
		//		Config *config = Config::createConfig(1, argv, NULL);
		//		IRenderer *renderer;
		//
		//		ASSERT_ANY_THROW(renderer = RenderingFactory::createRendering(0, &config->getSubConfig("render-driver")));
		//		ASSERT_ANY_THROW(renderer = RenderingFactory::createRendering("", &config->getSubConfig("render-driver")));
		//		ASSERT_NO_THROW(renderer = RenderingFactory::createRendering(api, &config->getSubConfig("render-driver")));
		//
		//		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Buffer)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		const int size = 4096;
		Buffer *buffer;
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));
		ASSERT_NO_FATAL_FAILURE(renderer);

		const std::vector<BufferDesc::BufferHint> hints1 = {
			BufferDesc::eRead,
			BufferDesc::eWrite,
			(BufferDesc::BufferHint)(BufferDesc::eRead | BufferDesc::eWrite),
		};
		const std::vector<BufferDesc::BufferHint> hints2 = {
			BufferDesc::eStatic, BufferDesc::eDynamic, BufferDesc::eStream};
		const std::vector<BufferDesc::BufferType> targets = {
			BufferDesc::eArray,
			BufferDesc::eElementArray,
			BufferDesc::eUniform,
			BufferDesc::eTexture,
			BufferDesc::eShaderStorage,
			BufferDesc::eTransformFeedback,
			BufferDesc::ePixelUnpack,
			BufferDesc::ePixelPack,
			BufferDesc::eIndirectDraw,
			BufferDesc::eIndirectDispatch};

		/*	Test all possible combinations of buffers.	*/
		for (int t = 0; t < hints1.size(); t++)
		{
			for (int h = 0; h < hints2.size(); h++)
			{
				for (int tar = 0; tar < targets.size(); tar++)
				{
					BufferDesc::BufferType target = targets[tar];
					BufferDesc::BufferHint hint = (BufferDesc::BufferHint)(hints1[t] | hints2[h]);

					/**/
					BufferDesc desc;
					desc.size = size;
					desc.type = target;
					desc.hint = hint;
					desc.data = NULL;
					desc.marker.markerName = "Buffer";

					/**/
					EXPECT_NO_THROW(buffer = renderer->createBuffer(&desc));
					ASSERT_EQ(buffer->getRenderer(), renderer);
					ASSERT_EQ(buffer->getSize(), size);
					ASSERT_NE(buffer->mapBuffer(Buffer::eRead), NULL);

					/**/
					EXPECT_NO_THROW(renderer->deleteBuffer(buffer));
				}
			}
		}

		/*	Handling of big sizes.	*/
		Capability capability;
		renderer->getCapability(&capability);
		const std::vector<int> maxSizes = {
			capability.sMaxUniformBlockSize,
			capability.sMaxTextureBufferSize,
		};

		/*	Test all possible combinations of buffers.	*/
		for (int t = 0; t < hints1.size(); t++)
		{
			for (int h = 0; h < hints2.size(); h++)
			{
				for (int tar = 0; tar < targets.size(); tar++)
				{
					BufferDesc::BufferType target = targets[tar];
					BufferDesc::BufferHint hint = (BufferDesc::BufferHint)(hints1[t] | hints2[h]);

					/*	*/
					BufferDesc desc;
					desc.size = size;
					desc.type = target;
					desc.hint = hint;
					desc.data = NULL;

					/**/
					EXPECT_NO_THROW(buffer = renderer->createBuffer(&desc));
					ASSERT_EQ(buffer->getRenderer(), renderer);
					ASSERT_EQ(buffer->getSize(), size);
					ASSERT_NE(buffer->mapBuffer(Buffer::eRead), NULL);

					/**/
					EXPECT_NO_THROW(renderer->deleteBuffer(buffer));
				}
			}
		}

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Texture)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)

		unsigned int width, height;
		void *pixels;
		unsigned long size;
		Texture *texture;
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		/*	Load the pixel data used for testing all the combination with the texture create function.	*/
		EXPECT_NO_THROW(pixels = TextureUtil::loadTextureData("../../fragview.png", &width, &height, &size));

		const std::vector<TextureDesc::Compression> compressions = {
			TextureDesc::eNoCompression,
			TextureDesc::eCompression,
			TextureDesc::eRGTC,
			TextureDesc::eDXT1,
			TextureDesc::eDXT4,
			TextureDesc::eS3TC

		};
		const std::vector<bool> mimaps = {false, true};
		const float ansio = 4.0f;
		for (int m = 0; m < mimaps.size(); m++)
		{
			bool mipmap = mimaps[m];
			for (int c = 0; c < compressions.size(); c++)
			{
				TextureDesc::Compression compression = compressions[c];

				TextureDesc desc;
				desc.originalTexture = NULL;
				desc.nrSamples = 0;
				desc.target = TextureDesc::eTexture2D;
				desc.width = width;
				desc.height = height;
				desc.depth = 1;
				desc.type = TextureDesc::eUnsignedByte;
				desc.internalformat = TextureDesc::Format::eRGBA;
				desc.format = TextureDesc::Format::eRGBA;
				desc.usemipmaps = mipmap;
				desc.pixel = pixels;
				desc.pixelSize = size;
				desc.numlevel = 5;
				desc.compression = TextureDesc::Compression::eNoCompression;
				desc.compression = compression;
				desc.sampler.AddressU = SamplerDesc::eRepeat;
				desc.sampler.AddressV = SamplerDesc::eRepeat;
				desc.sampler.AddressW = SamplerDesc::eRepeat;
				desc.sampler.magFilter = SamplerDesc::eLinear;
				desc.sampler.minFilter = SamplerDesc::eLinear;
				desc.sampler.mipmapFilter = SamplerDesc::eLinear;
				desc.sampler.compareFunc = SamplerDesc::CompareFunc::eNoCompare;
				desc.sampler.compareMode = 0;
				desc.sampler.anisotropy = ansio;
				desc.Swizzlea = TextureDesc::eNoSwizzle;
				desc.Swizzler = TextureDesc::eNoSwizzle;
				desc.Swizzleg = TextureDesc::eNoSwizzle;
				desc.Swizzleb = TextureDesc::eNoSwizzle;
				ASSERT_NO_THROW(texture = renderer->createTexture(&desc));
				ASSERT_NE(texture, NULL);
				ASSERT_EQ(texture->width(), width);
				ASSERT_EQ(texture->height(), height);
				ASSERT_FLOAT_EQ(texture->getAnisotropic(), ansio);
				ASSERT_EQ(texture->getRenderer(), renderer);
				/*	TODO.	*/
				//texture->getWrapMode();
				//texture->getFilterMode();
				//texture->getFormat();

				renderer->deleteTexture(texture);
			}
		}

		//TODO check original texture support.
		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Shader)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Geometry)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Sampler)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api);
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		const std::vector<SamplerDesc::AddressMode> addresses = {
			SamplerDesc::eRepeat,
			SamplerDesc::eRepeatMirror,
			SamplerDesc::eClamp,
			SamplerDesc::eClampBorder

		};

		const std::vector<SamplerDesc::FilterMode> filterModes = {
			SamplerDesc::eLinear,
			SamplerDesc::eNearset};

		for (int f = 0; f < filterModes.size(); f++)
		{
			SamplerDesc::FilterMode filter;
			for (int a = 0; a < addresses.size(); a++)
			{
				SamplerDesc::AddressMode addressMode = addresses[a];
				Sampler *sampler;

				SamplerDesc desc;
				desc.anisotropy = 4.0f;
				desc.minLOD = 0;
				desc.minFilter = filter;
				desc.magFilter = filter;
				desc.AddressU = addressMode;
				desc.AddressV = addressMode;
				desc.AddressW = addressMode;
				desc.maxLOD = 5;
				desc.compareFunc = SamplerDesc::eNoCompare;
				desc.mipmapFilter = filter;
				desc.biasLOD = 2;
				desc.borderColor[0] = 0.0f;
				desc.borderColor[1] = 0.0f;
				desc.borderColor[2] = 0.0f;
				desc.borderColor[3] = 0.0f;

				EXPECT_NO_THROW(sampler = renderer->createSampler(&desc));
				/*  */
				ASSERT_EQ(sampler->getRenderer(), renderer);
				ASSERT_FLOAT_EQ(sampler->getAnisotropic(), desc.anisotropy);
				ASSERT_NE(sampler, NULL);

				/*  */
				Sampler::WrapMode wrapMode = sampler->getWrapMode();
				Sampler::FilterMode filterMode = sampler->getFilterMode();

				renderer->deleteSampler(sampler);
			}
		}

		/*  Test bad description.   */

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, RenderPipeline)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;
	}
}

TEST_F(RenderingInterfaceTest, Query)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;
		QueryObject *queryObject;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		QueryDesc desc;
		//renderer->createFrameBuffer()

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Sync)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, FrameBuffer)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Window)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;
	}
}

TEST_F(RenderingInterfaceTest, Viewport)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;
		Capability capability;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));
		ASSERT_NO_THROW(renderer->getCapability(&capability));

		ViewPort *defaultView = renderer->getView(0);

		for (int i = 0; i < capability.sMaxViewPorts; i++)
		{
			ViewPort *viewPort = renderer->getView(i);
		}
		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, RenderingStates)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;
		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		unsigned int i;
		for (i = IRenderer::eDepthTest; i < IRenderer::State::eDiscardRasterization; i++)
		{
			renderer->enableState((IRenderer::State)i);
			ASSERT_TRUE(renderer->isStateEnabled((IRenderer::State)i));
		}

		for (i = IRenderer::eDepthTest; i < IRenderer::State::eDiscardRasterization; i++)
		{
			renderer->disableState((IRenderer::State)i);
			ASSERT_FALSE(renderer->isStateEnabled((IRenderer::State)i));
		}

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, Render)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		deleteRenderer(renderer);
	}
}

TEST_F(RenderingInterfaceTest, ComputeShader)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer;

		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		/*  Load shader.    */

		/*  Perform compute shader. */

		/*  Validate compute shader.    */

		/*  Release resources.  */

		deleteRenderer(renderer);
	}
}

//TODO perhaps relocate and relocate the RenderingInterafceTest class to global space.
TEST_F(RenderingInterfaceTest, Font)
{
	std::vector<RenderingFactory::RenderingAPI>::const_iterator it = apis.cbegin();

	FileIO *fileIo = (FileIO *)FileSystem::getFileSystem()->openFile("../../ipag.ttf", IO::READ);

	for (; it != apis.end(); it++)
	{
		RenderingFactory::RenderingAPI api = (*it);
		SETRENDERAPISCOPE(this, api)
		IRenderer *renderer = NULL;
		Font *font = NULL;

		/*  */
		ASSERT_TRUE(renderer = createRendererInterface(api, this->config));

		/*  */
		//ASSERT_NO_THROW(font = FontFactory::createFont(renderer, (IO *) fileIo, 10.0f));

		/*  Font states.    */
		ASSERT_GE(font->getIndexCount(), 0);
		//font->getEncoding();
		ASSERT_NO_THROW(font->getCharacter('a'));

		/*  Font texture.   */
		ASSERT_NE(font->getTexture(), NULL);
		ASSERT_GE(font->getTexture()->width(), 0);
		ASSERT_GE(font->getTexture()->height(), 0);

		/*  Release rendering interface resources.  */
		EXPECT_NO_THROW(deleteRenderer(renderer));
	}

	FileSystem::getFileSystem()->closeFile(fileIo);
}