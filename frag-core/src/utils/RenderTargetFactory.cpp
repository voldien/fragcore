#include"Utils/RenderTargetFactory.h"
#include<assert.h>
#include <stdexcept>

using namespace fragcore;

FrameBuffer* RenderTargetFactory::createShadow(IRenderer* render, int width, int height){

	FrameBufferDesc desc = {};
	TextureDesc depthdesc;
	FrameBuffer* frame = nullptr;
	Texture* shadowtex = nullptr;

	assert(render);
	if(render == nullptr)
		throw InvalidArgumentException("renderer interface must not be null!");


	/*	*/
	depthdesc.target = TextureDesc::eTexture2D;
	depthdesc.width = width;
	depthdesc.height = height;
	depthdesc.usemipmaps = 0;
	depthdesc.internalformat = TextureDesc::eDepth;
	depthdesc.format = TextureDesc::eDepth;
	depthdesc.type = TextureDesc::eFloat;
	depthdesc.sampler.AddressU = SamplerDesc::eClamp;
	depthdesc.sampler.AddressV = SamplerDesc::eClamp;
	depthdesc.sampler.AddressW = SamplerDesc::eClamp;
	depthdesc.sampler.minFilter = SamplerDesc::eLinear;
	depthdesc.sampler.magFilter = SamplerDesc::eLinear;
	depthdesc.sampler.mipmapFilter = SamplerDesc::eLinear;
	depthdesc.sampler.anisotropy = 1.0f;
	depthdesc.pixel = nullptr;

	/*	*/
	shadowtex = render->createTexture(&depthdesc);

	/*	Framebuffer descriptor.	*/
	desc.depth = shadowtex;
	frame = render->createFrameBuffer(&desc);
	return frame;
}

FrameBuffer* RenderTargetFactory::createDeffered(IRenderer* render, int width, int height, unsigned int options){

	Texture* depthstencil = nullptr;
	Texture* color1 = nullptr;
	Texture* color2 = nullptr;
	Texture* color3 = nullptr;
	Texture* color4 = nullptr;
	Texture* color5 = nullptr;
	TextureDesc texdesc;
	FrameBufferDesc desc;
	FrameBuffer* frame = nullptr;

	if(render == nullptr)
		throw InvalidArgumentException("renderer interface must not be null!");

	memset(&texdesc, 0, sizeof(texdesc));
	memset(&desc, 0, sizeof(desc));

	/*	Create stencil depth texture.	*/
	texdesc.target = TextureDesc::eTexture2D;
	texdesc.width = width;
	texdesc.height = height;
	texdesc.format = TextureDesc::eDepthStencil;
	texdesc.internalformat = TextureDesc::eDepthStencil;
	texdesc.type = TextureDesc::eUnsigned24_8;
	texdesc.sampler.AddressU = SamplerDesc::eClamp;
	texdesc.sampler.AddressV = SamplerDesc::eClamp;
	texdesc.sampler.AddressW = SamplerDesc::eClamp;
	texdesc.sampler.minFilter = SamplerDesc::eLinear;
	texdesc.sampler.magFilter = SamplerDesc::eLinear;
	texdesc.sampler.mipmapFilter = SamplerDesc::eNoFilterMode;
	texdesc.sampler.anisotropy = 1.0f;
	depthstencil = render->createTexture(&texdesc);

	/*	Diffuse color texture attachment.	*/
	texdesc.format = TextureDesc::eRGB;
	texdesc.internalformat = TextureDesc::eRGB;
	texdesc.type = TextureDesc::eUnsignedByte;
	color1 = render->createTexture(&texdesc);
	color1->setWrapMode(Texture::eClamp);

	/*	*/
	texdesc.format = TextureDesc::eRGB;
	texdesc.internalformat = TextureDesc::eRGB;
	texdesc.type = TextureDesc::eUnsignedByte;
	color2 = render->createTexture(&texdesc);

	/*	*/
	texdesc.format = TextureDesc::eRGB;
	texdesc.internalformat = TextureDesc::eRGB;
	texdesc.type = TextureDesc::eUnsignedByte;
	color3 = render->createTexture(&texdesc);

	/*	*/
	texdesc.format = TextureDesc::eRGB;
	texdesc.internalformat = TextureDesc::eRGB;
	texdesc.type = TextureDesc::eUnsignedByte;
	color4 = render->createTexture(&texdesc);

	/*	*/
	texdesc.format = TextureDesc::eRGB;
	texdesc.internalformat = TextureDesc::eRGB;
	texdesc.type = TextureDesc::eUnsignedByte;
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
	if (render == nullptr)
		throw InvalidArgumentException("renderer interface must not be null!");

	/*	Zero out descriptor.	*/
	memset(&texdesc, 0, sizeof(texdesc));
	memset(&desc, 0, sizeof(desc));

	/*	Color texture descriptor.	*/
	texdesc.target = TextureDesc::eTexture2D;
	texdesc.width = width;
	texdesc.height = height;
	texdesc.format = TextureDesc::eRGBA;
	texdesc.internalformat = TextureDesc::eRGBA;
	texdesc.pixelFormat = TextureFormat::BGR24;
	texdesc.graphicFormat = GraphicFormat::R8G8B8_SRGB;
	texdesc.type = TextureDesc::eUnsignedByte;
	texdesc.srgb = 1;
	texdesc.usemipmaps = 0;
	texdesc.compression = 0;
	texdesc.sampler.AddressU = SamplerDesc::eClamp;
	texdesc.sampler.AddressV = SamplerDesc::eClamp;
	texdesc.sampler.AddressW = SamplerDesc::eClamp;
	texdesc.sampler.minFilter = SamplerDesc::eLinear;
	texdesc.sampler.magFilter = SamplerDesc::eLinear;
	texdesc.sampler.mipmapFilter = SamplerDesc::eNoFilterMode;
	texdesc.sampler.anisotropy = 1.0f;
	texdesc.marker.markerName = "FrameBuffer Color Component";

	color = render->createTexture(&texdesc);
	desc.attach[0] = color;
	color = render->createTexture(&texdesc);
	desc.attach[1] = color;

	/*	Framebuffer descriptor.	*/
	fraobj = render->createFrameBuffer(&desc);

	return fraobj;
}

FrameBuffer* RenderTargetFactory::createHDR(IRenderer* renderer, int width, int height){

	Texture* color;
	FrameBuffer* fraobj;
	TextureDesc texdesc;
	FrameBufferDesc desc;

	assert(renderer);
	if(renderer == nullptr)
		throw InvalidArgumentException("renderer interface must not be null!");

	/*	Zero out descriptor.	*/
	memset(&texdesc, 0, sizeof(texdesc));
	memset(&desc, 0, sizeof(desc));

	/*	Color texture descriptor.	*/
	texdesc.target = TextureDesc::eTexture2D;
	texdesc.width = width;
	texdesc.height = height;
	texdesc.format = TextureDesc::eRGB;
	texdesc.internalformat = TextureDesc::eRGB;
	texdesc.type = TextureDesc::eFloat;
	texdesc.sampler.AddressU = SamplerDesc::eClamp;
	texdesc.sampler.AddressV = SamplerDesc::eClamp;
	texdesc.sampler.AddressW = SamplerDesc::eClamp;
	texdesc.sampler.minFilter = SamplerDesc::eLinear;
	texdesc.sampler.magFilter = SamplerDesc::eLinear;
	texdesc.sampler.mipmapFilter = SamplerDesc::eLinear;
	texdesc.sampler.anisotropy = 1.0f;
	color = renderer->createTexture(&texdesc);

	/*	Framebuffer descriptor.	*/
	desc.attach[0] = color;
	fraobj = renderer->createFrameBuffer(&desc);

	return fraobj;
}
