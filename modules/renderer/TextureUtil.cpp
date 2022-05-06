
#include "TextureUtil.h"
#include "Core/IO/IOUtil.h"
#include "Core/Math.h"
#include "Texture.h"
#include <Core/IO/FileSystem.h>
#include <FreeImage.h>
#include <ImageLoader.h>
#include <fmt/core.h>
using namespace fragcore;

void TextureUtil::loadTexture(Ref<IO> &io, IRenderer *renderer, Texture **texture) {
	TextureDesc desc = {};
	TextureDesc::Format format;
	TextureDesc::Format internalformat;
	TextureDesc::Type type;

	/*	Load texture.   */

	ImageLoader loader;
	Image image = loader.loadImage(io);

	/*  Texture attributes. */
	desc.width = image.width();
	desc.height = image.height();
	desc.depth = image.layers();
	desc.pixel = image.getPixelData();
	desc.pixelSize = image.getSize();
	desc.compression = TextureDesc::eNoCompression;

	/*  Texture.    */
	desc.target = TextureDesc::Target::Texture2D;
	desc.format = format;
	desc.pixelFormat = (TextureFormat)format;
	desc.internalformat = internalformat;
	desc.graphicFormat = (GraphicFormat)internalformat;
	desc.type = type;
	desc.numlevel = 4;
	desc.usemipmaps = 1;
	desc.srgb = 0;

	/*  sampler.    */
	desc.sampler.AddressU = SamplerDesc::AddressMode::Repeat;
	desc.sampler.AddressV = SamplerDesc::AddressMode::Repeat;
	desc.sampler.AddressW = SamplerDesc::AddressMode::Repeat;
	desc.sampler.anisotropy = 8.0f;
	desc.sampler.minFilter = SamplerDesc::FilterMode::Linear;
	desc.sampler.magFilter = SamplerDesc::FilterMode::Linear;
	desc.sampler.mipmapFilter = SamplerDesc::FilterMode::Linear;

	desc.marker = nullptr;

	*texture = renderer->createTexture(&desc);
	/*  TODO rename.    */
	(*texture)->setName(io->getName());
}

void TextureUtil::loadTexture(const std::string &filepath, IRenderer *renderer, Texture **texture) {

	Ref<IO> ref = Ref<IO>(FileSystem::getFileSystem()->openFile(filepath.c_str(), IO::IOMode::READ));
	loadTexture(ref, renderer, texture);
}

void TextureUtil::saveTexture(const std::string &filepath, IRenderer *renderer, Texture *texture) {
	Ref<IO> ref = Ref<IO>(FileSystem::getFileSystem()->openFile(filepath.c_str(), IO::IOMode::READ));
	saveTexture(ref, renderer, texture);
}

void TextureUtil::saveTexture(Ref<IO> &io, IRenderer *renderer, Texture *texture) { /*	Image Loader.	*/
	ImageLoader imageLoader;
	/*	TODO verify.	*/
	void *pixels = texture->mapTexture(texture->getFormat(), 0);
	Image image(texture->width(), texture->height(), TextureFormat::Alpha8);
	image.setPixelData(pixels, -1);
	/*	*/
	imageLoader.saveImage(io, image);
	free(pixels);
}

Texture *TextureUtil::createTexture(IRenderer *renderer, unsigned int width, unsigned int height, const Ref<IO> &io,
									TextureFormat format, GraphicFormat graphicformat) {
	long int size = 0;
	void *pixels = nullptr;
	// size = IOUtil::loadFileMem(io, &pixels);

	Texture *texture = createTexture(renderer, width, height, pixels, size, format, graphicformat);
	free(pixels);
	return texture;
}

Texture *TextureUtil::createTexture(IRenderer *renderer, unsigned int width, unsigned int height, const void *pixels,
									unsigned int size, TextureFormat format, GraphicFormat graphicformat) {
	/*  */
	TextureDesc desc = {};
	desc.width = width;
	desc.height = height;
	desc.depth = 1;
	desc.pixel = pixels;
	desc.pixelSize = size;
	desc.compression = TextureDesc::eNoCompression;
	desc.nrSamples = 0;

	/*  Texture.    */
	desc.target = TextureDesc::Target::Texture2D;
	// desc.format = format;
	// desc.internalformat = TextureDesc::eRGBA;

	desc.pixelFormat = format;
	desc.graphicFormat = graphicformat;
	desc.numlevel = 5; // Math::clamp((int)(log(width),0, 5);
	desc.usemipmaps = 1;
	desc.srgb = 0;

	/*  Sampler.    */
	desc.sampler.AddressU = SamplerDesc::AddressMode::Repeat;
	desc.sampler.AddressV = SamplerDesc::AddressMode::Repeat;
	desc.sampler.AddressW = SamplerDesc::AddressMode::Repeat;
	desc.sampler.anisotropy = 8.0f;
	desc.sampler.minFilter = SamplerDesc::FilterMode::Linear;
	desc.sampler.magFilter = SamplerDesc::FilterMode::Linear;
	desc.sampler.mipmapFilter = SamplerDesc::FilterMode::Linear;
	desc.sampler.maxLOD = 0;
	desc.sampler.minLOD = 0;
	desc.sampler.biasLOD = 0;

	return renderer->createTexture(&desc);
}
