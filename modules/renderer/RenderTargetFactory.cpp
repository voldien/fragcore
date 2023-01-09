#include "RenderTargetFactory.h"
#include <assert.h>

using namespace fragcore;

FrameBuffer *RenderTargetFactory::createShadow(IRenderer *render, int width, int height) {

	FrameBufferDesc desc = {};
	TextureDesc depthdesc;
	FrameBuffer *frame = nullptr;
	Texture *shadowtex = nullptr;

	assert(render);
	if (render == nullptr) {
		throw InvalidArgumentException("renderer interface must not be null!");
	}

	/*	*/
	depthdesc.target = TextureDesc::Target::Texture2D;
	depthdesc.width = width;
	depthdesc.height = height;
	depthdesc.usemipmaps = 0;
	depthdesc.internalformat = TextureDesc::Format::Depth;
	depthdesc.format = TextureDesc::Format::Depth;
	depthdesc.type = TextureDesc::Type::Float;
	depthdesc.sampler.AddressU = SamplerDesc::AddressMode::Clamp;
	depthdesc.sampler.AddressV = SamplerDesc::AddressMode::Clamp;
	depthdesc.sampler.AddressW = SamplerDesc::AddressMode::Clamp;
	depthdesc.sampler.minFilter = SamplerDesc::FilterMode::Linear;
	depthdesc.sampler.magFilter = SamplerDesc::FilterMode::Linear;
	depthdesc.sampler.mipmapFilter = SamplerDesc::FilterMode::Linear;
	depthdesc.sampler.anisotropy = 1.0f;
	depthdesc.pixel = nullptr;
	depthdesc.marker = nullptr;

	/*	*/
	shadowtex = render->createTexture(&depthdesc);

	/*	Framebuffer descriptor.	*/
	desc.depth = shadowtex;
	frame = render->createFrameBuffer(&desc);
	return frame;
}

FrameBuffer *RenderTargetFactory::createDeffered(IRenderer *render, int width, int height, unsigned int options) {

	Texture *depthstencil = nullptr;
	Texture *color1 = nullptr;
	Texture *color2 = nullptr;
	Texture *color3 = nullptr;
	Texture *color4 = nullptr;
	Texture *color5 = nullptr;
	TextureDesc texdesc;
	FrameBufferDesc desc;
	FrameBuffer *frame = nullptr;

	if (render == nullptr) {
		throw InvalidArgumentException("renderer interface must not be null!");
	}

	memset(&texdesc, 0, sizeof(texdesc));
	memset(&desc, 0, sizeof(desc));

	/*	Create stencil depth texture.	*/
	texdesc.target = TextureDesc::Target::Texture2D;
	texdesc.width = width;
	texdesc.height = height;
	texdesc.format = TextureDesc::Format::DepthStencil;
	texdesc.internalformat = TextureDesc::Format::DepthStencil;
	texdesc.type = TextureDesc::Type::Unsigned24_8;
	texdesc.sampler.AddressU = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.AddressV = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.AddressW = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.minFilter = SamplerDesc::FilterMode::Linear;
	texdesc.sampler.magFilter = SamplerDesc::FilterMode::Linear;
	texdesc.sampler.mipmapFilter = SamplerDesc::FilterMode::NoFilterMode;
	texdesc.sampler.anisotropy = 1.0f;
	depthstencil = render->createTexture(&texdesc);

	/*	Diffuse color texture attachment.	*/
	texdesc.format = TextureDesc::Format::RGB;
	texdesc.internalformat = TextureDesc::Format::RGB;
	texdesc.type = TextureDesc::Type::UnsignedByte;
	color1 = render->createTexture(&texdesc);
	color1->setWrapMode(Texture::eClamp);

	/*	*/
	texdesc.format = TextureDesc::Format::RGB;
	texdesc.internalformat = TextureDesc::Format::RGB;
	texdesc.type = TextureDesc::Type::UnsignedByte;
	color2 = render->createTexture(&texdesc);

	/*	*/
	texdesc.format = TextureDesc::Format::RGB;
	texdesc.internalformat = TextureDesc::Format::RGB;
	texdesc.type = TextureDesc::Type::UnsignedByte;
	color3 = render->createTexture(&texdesc);

	/*	*/
	texdesc.format = TextureDesc::Format::RGB;
	texdesc.internalformat = TextureDesc::Format::RGB;
	texdesc.type = TextureDesc::Type::UnsignedByte;
	color4 = render->createTexture(&texdesc);

	/*	*/
	texdesc.format = TextureDesc::Format::RGB;
	texdesc.internalformat = TextureDesc::Format::RGB;
	texdesc.type = TextureDesc::Type::UnsignedByte;
	color5 = render->createTexture(&texdesc);

	/*	*/
	desc.depthstencil = depthstencil;
	desc.attach[0] = color1;
	desc.attach[1] = color2;
	desc.attach[2] = color3;
	desc.attach[3] = color4;
	desc.attach[4] = color5;
	frame = render->createFrameBuffer(&desc);
	return frame;
}

FrameBuffer *RenderTargetFactory::createColor(IRenderer *render, int width, int height) {

	Texture *color;
	FrameBuffer *fraobj;
	TextureDesc texdesc;
	FrameBufferDesc desc;

	assert(render);
	if (render == nullptr) {
		throw InvalidArgumentException("renderer interface must not be null!");
	}

	/*	Zero out descriptor.	*/
	memset(&texdesc, 0, sizeof(texdesc));
	memset(&desc, 0, sizeof(desc));

	/*	Color texture descriptor.	*/
	texdesc.target = TextureDesc::Target::Texture2D;
	texdesc.width = width;
	texdesc.height = height;
	texdesc.format = TextureDesc::Format::RGBA;
	texdesc.internalformat = TextureDesc::Format::RGBA;
	texdesc.pixelFormat = TextureFormat::BGR24;
	texdesc.graphicFormat = GraphicFormat::R8G8B8_SRGB;
	texdesc.type = TextureDesc::Type::UnsignedByte;
	texdesc.srgb = 1;
	texdesc.usemipmaps = 0;
	texdesc.compression = 0;
	texdesc.sampler.AddressU = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.AddressV = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.AddressW = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.minFilter = SamplerDesc::FilterMode::Linear;
	texdesc.sampler.magFilter = SamplerDesc::FilterMode::Linear;
	texdesc.sampler.mipmapFilter = SamplerDesc::FilterMode::NoFilterMode;
	texdesc.sampler.anisotropy = 1.0f;
	// texdesc.marker.markerName = "FrameBuffer Color Component";

	color = render->createTexture(&texdesc);
	desc.attach[0] = color;
	color = render->createTexture(&texdesc);
	desc.attach[1] = color;

	/*	Framebuffer descriptor.	*/
	fraobj = render->createFrameBuffer(&desc);

	return fraobj;
}

FrameBuffer *RenderTargetFactory::createHDR(IRenderer *renderer, int width, int height) {

	Texture *color;
	FrameBuffer *fraobj;
	TextureDesc texdesc;
	FrameBufferDesc desc;

	assert(renderer);
	if (renderer == nullptr) {
		throw InvalidArgumentException("renderer interface must not be null!");
	}

	/*	Zero out descriptor.	*/
	memset(&texdesc, 0, sizeof(texdesc));
	memset(&desc, 0, sizeof(desc));

	/*	Color texture descriptor.	*/
	texdesc.target = TextureDesc::Target::Texture2D;
	texdesc.width = width;
	texdesc.height = height;
	texdesc.format = TextureDesc::Format::RGB;
	texdesc.internalformat = TextureDesc::Format::RGB;
	texdesc.type = TextureDesc::Type::Float;
	texdesc.sampler.AddressU = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.AddressV = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.AddressW = SamplerDesc::AddressMode::Clamp;
	texdesc.sampler.minFilter = SamplerDesc::FilterMode::Linear;
	texdesc.sampler.magFilter = SamplerDesc::FilterMode::Linear;
	texdesc.sampler.mipmapFilter = SamplerDesc::FilterMode::Linear;
	texdesc.sampler.anisotropy = 1.0f;
	color = renderer->createTexture(&texdesc);

	/*	Framebuffer descriptor.	*/
	desc.attach[0] = color;
	fraobj = renderer->createFrameBuffer(&desc);

	return fraobj;
}
