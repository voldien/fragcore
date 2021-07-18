
#include "Utils/TextureUtil.h"
#include "Core/IO/IOUtil.h"
#include "Core/Math.h"
#include "Exception/RuntimeException.h"
#include "Renderer/Texture.h"
#include "Utils/StringUtil.h"
#include <Core/IO/FileSystem.h>
#include <Exception/InvalidArgumentException.h>
#include <FreeImage.h>
#include <stdexcept>
#include <stdio.h>
#include<fmt/core.h>
using namespace fragcore;

void TextureUtil::loadTexture(const char *path, IRenderer *renderer, Texture **texture) {

	TextureDesc desc = {0};
	void *pixeldata = nullptr;
	TextureDesc::Format format;
	TextureDesc::Format internalformat;
	unsigned int width;
	unsigned int height;
	unsigned int long pixelSize;
	TextureDesc::Type type;

	/*	Load texture.   */
	if ((pixeldata =
			 TextureUtil::loadTextureData(path, &width, &height, &format, &internalformat, &type, &pixelSize))) {

		/*  Texture attributes. */
		desc.width = width;
		desc.height = height;
		desc.depth = 1;
		desc.pixel = pixeldata;
		desc.pixelSize = pixelSize;
		desc.compression = TextureDesc::eNoCompression;

		/*  Texture.    */
		desc.target = TextureDesc::eTexture2D;
		desc.format = format;
		desc.pixelFormat = (TextureFormat)format;
		desc.internalformat = internalformat;
		desc.graphicFormat = (GraphicFormat)internalformat;
		desc.type = type;
		desc.numlevel = 4;
		desc.usemipmaps = 1;
		desc.srgb = 0;

		/*  sampler.    */
		desc.sampler.AddressU = SamplerDesc::eRepeat;
		desc.sampler.AddressV = SamplerDesc::eRepeat;
		desc.sampler.AddressW = SamplerDesc::eRepeat;
		desc.sampler.anisotropy = 8.0f;
		desc.sampler.minFilter = SamplerDesc::eLinear;
		desc.sampler.magFilter = SamplerDesc::eLinear;
		desc.sampler.mipmapFilter = SamplerDesc::eLinear;

		desc.marker.markerName = path;

		*texture = renderer->createTexture(&desc);
		/*  TODO rename.    */
		(*texture)->setName(path);
		free(pixeldata);
	}
}

void TextureUtil::loadTexture(const void *pbuf, long int size, IRenderer *renderer, Texture **texture) { /*  */
}

void *TextureUtil::loadTextureData(const char *cfilename, unsigned int *pwidth, unsigned int *pheight,
								   TextureDesc::Format *pformat, TextureDesc::Format *pinternalformat,
								   TextureDesc::Type *ptype, unsigned long *pixelSize) {

	void *pixeldata = nullptr;

	Ref<IO> f = Ref<IO>(FileSystem::getFileSystem()->openFile(cfilename, IO::READ));
	long inbytes = IOUtil::loadFileMem(f, (char **)&pixeldata);
	void *pixelResult =
		loadTextureDataFromMem(pixeldata, inbytes, pwidth, pheight, pformat, pinternalformat, ptype, pixelSize);
	free(pixeldata);
	f->close();
	//	delete f;

	return pixelResult;
}

void *TextureUtil::loadTextureData(const char *cfilename, unsigned int *pwidth, unsigned int *pheight,
								   unsigned long *pixelSize) {
	return TextureUtil::loadTextureData(cfilename, pwidth, pheight, nullptr, nullptr, nullptr, pixelSize);
}

void *TextureUtil::loadTextureData(const void *pbuf, long int size, unsigned int *width, unsigned int *height) {
	return loadTextureDataFromMem(pbuf, size, width, height, nullptr, nullptr, nullptr, nullptr);
}

void *TextureUtil::loadTextureDataFromMem(const void *pbuf, long int size, unsigned int *width, unsigned int *height,
										  TextureDesc::Format *pformat, TextureDesc::Format *pinternalformat,
										  TextureDesc::Type *ptype, unsigned long *pixelSize) {

	/*	Free image.	*/
	FREE_IMAGE_FORMAT imgtype;		 /**/
	FREE_IMAGE_COLOR_TYPE colortype; /**/
	FREE_IMAGE_TYPE imgt;			 /**/
	FIMEMORY *stream;				 /**/
	FIBITMAP *firsbitmap;			 /**/
	void *pixel;					 /**/
	unsigned int bpp;

	assert(width && height && pbuf);
	if (size <= 0)
		throw InvalidArgumentException("Texture data must be greater than 0.");

	/*	1 byte for the size in order, Because it crash otherwise if set to 0.	*/
	stream = FreeImage_OpenMemory((BYTE *)pbuf, size);
	if (stream == nullptr)
		throw RuntimeException(fmt::format("Failed to open freeimage memory stream. \n"));

	/*	Seek to beginning of the memory stream.	*/
	FreeImage_SeekMemory(stream, 0, SEEK_SET);

	/*	Load image from */
	imgtype = FreeImage_GetFileTypeFromMemory(stream, size);
	FreeImage_SeekMemory(stream, 0, SEEK_SET);
	firsbitmap = FreeImage_LoadFromMemory(imgtype, stream, 0);
	if (firsbitmap == nullptr) {
		FreeImage_CloseMemory(stream);
		throw RuntimeException(fmt::format("Failed to create free-image from memory.\n"));
	}

	/*	Reset to beginning of stream.	*/
	FreeImage_SeekMemory(stream, 0, SEEK_SET);
	imgt = FreeImage_GetImageType(firsbitmap);
	colortype = FreeImage_GetColorType(firsbitmap);
	// imagetype = FreeImage_GetImageType(firsbitmap);

	switch (colortype) {
	case FIC_RGB:
		if (pformat)
			*pformat = (TextureDesc::Format)TextureFormat::BGR24; // TextureDesc::eBGR;
		if (pinternalformat)
			*pinternalformat = (TextureDesc::Format)GraphicFormat::R8G8B8_SRGB; // TextureDesc::eRGB;
		bpp = 3;
		break;
	case FIC_RGBALPHA:
		if (pformat)
			*pformat = (TextureDesc::Format)TextureFormat::BGRA32; // TextureDesc::eBGR;
		if (pinternalformat)
			*pinternalformat = (TextureDesc::Format)GraphicFormat::R8G8B8A8_SRGB; // TextureDesc::eRGB;
		/*			if (pformat)
						*pformat = TextureDesc::eBGRA;
					if (pinternalformat)
						*pinternalformat = TextureDesc::eRGBA;*/
		bpp = 4;
		break;
	case FIC_MINISWHITE:
	case FIC_MINISBLACK:
		if (pformat)
			*pformat = TextureDesc::eSingleColor;
		if (pinternalformat)
			*pinternalformat = TextureDesc::eSingleColor;
		bpp = 1;
		break;
	default:
		break;
	}

	/*	Get attributes from the image.	*/
	pixel = FreeImage_GetBits(firsbitmap);
	if (width)
		*width = FreeImage_GetWidth(firsbitmap);
	if (height)
		*height = FreeImage_GetHeight(firsbitmap);
	bpp = (FreeImage_GetBPP(firsbitmap) / 8);
	if (pixelSize)
		*pixelSize = (*width) * (*height) * bpp;

	if (ptype)
		*ptype = TextureDesc::eUnsignedByte;

	/*	Check error and release resources.	*/
	if (pixel == nullptr || size == 0) {
		FreeImage_Unload(firsbitmap);
		FreeImage_CloseMemory(stream);
		throw RuntimeException(fmt::format("Failed getting pixel data from FreeImage.\n"));
	}

	/*	Make a copy of pixel data.	*/
	void *pixels = malloc(*pixelSize);
	if (pixels == nullptr)
		throw RuntimeException(fmt::format("Failed to allocate {}, %s.\n", size, strerror(errno)));

	memcpy(pixels, pixel, *pixelSize);

	/*	Release free image resources.	*/
	FreeImage_Unload(firsbitmap);
	FreeImage_CloseMemory(stream);
	return pixels;
}

void TextureUtil::saveTexture(const char *filepath, IRenderer *renderer, Texture *texture) {

	void *pixels = nullptr;
	FIMEMORY *fimemory = nullptr;
	FIBITMAP *image = nullptr;
	FIBITMAP *finalImage = nullptr;
	FREE_IMAGE_FORMAT image_format = FIF_UNKNOWN;

	assert(renderer && filepath && texture);

	Texture::Format textureformat = Texture::eR8G8B8;
	const unsigned int Bpp = textureformat == Texture::eR8G8B8 ? 3 : 4; /*  TODO improve logic. */
	const unsigned long size = texture->width() * texture->height() * Bpp;

	/*  Validate argument.  */
	if (size <= 0)
		throw RuntimeException("Invalid texture, yield invalid size.");

	/*  Read current pixels from mandelbrot set.    */
	pixels = texture->mapTexture(textureformat, 0);
	if (pixels == nullptr)
		throw RuntimeException("Texture pixel mapping failed.");

	FreeImage_Initialise(FALSE);

	/*  Get file fmt::format.    */
	image_format = FreeImage_GetFIFFromFilename(filepath);
	if (image_format == FIF_UNKNOWN) {
		texture->unMapTexture();
		throw InvalidArgumentException(fmt::format("filepath file fmt::format is not supported : %s", filepath));
	}

	// TODO resolve color swizzle issue.
	/*  Allocate image buffer.  */
	image = FreeImage_ConvertFromRawBits((BYTE *)pixels, texture->width(), texture->height(), Bpp * texture->width(),
										 Bpp * 8, 0x000000FF, 0x0000FF00, 0x00FF0000, FALSE);
	if (image == nullptr) {
		FreeImage_DeInitialise();
		throw RuntimeException(fmt::format("FreeImage_ConvertFromRawBits failed: %s", filepath));
	}

	/*  */
	finalImage = FreeImage_ConvertTo32Bits(image);
	if (finalImage == nullptr) {
		throw RuntimeException(fmt::format("Failed convert image: %s", filepath));
	}

	/*  Save to file.   */
	// FreeImage_SaveToHandle()

	// FIMEMORY* mem = FreeImage_OpenMemory(nullptr, FreeImage_GetMemorySize(image));
	// FreeImage_SaveToMemory(image_format, image, mem);

	if (!FreeImage_Save(image_format, image, filepath, 0))
		throw RuntimeException(fmt::format("Failed save image: %s", filepath));

	/*  */
	FreeImage_CloseMemory(fimemory);
	texture->unMapTexture();
	FreeImage_Unload(image);
	FreeImage_DeInitialise();
}

void TextureUtil::saveTexture(Ref<IO> &io, IRenderer *renderer, Texture *texture) {}

Texture *TextureUtil::createTexture(IRenderer *renderer, unsigned int width, unsigned int height, const Ref<IO> &io,
									TextureFormat format, GraphicFormat graphicformat) {}

Texture *TextureUtil::createTexture(IRenderer *renderer, unsigned int width, unsigned int height, const void *pixels,
									unsigned int size, TextureFormat format, GraphicFormat graphicformat) {
	/*  */
	TextureDesc desc = {0};
	desc.width = width;
	desc.height = height;
	desc.depth = 1;
	desc.pixel = pixels;
	desc.pixelSize = size;
	desc.compression = TextureDesc::eNoCompression;
	desc.nrSamples = 0;

	/*  Texture.    */
	desc.target = TextureDesc::eTexture2D;
	desc.format = TextureDesc::eSingleColor;
	desc.internalformat = TextureDesc::eRGBA;

	desc.pixelFormat = format;
	desc.graphicFormat = graphicformat;
	desc.numlevel = 5; // Math::clamp((int)(log(width),0, 5);
	desc.usemipmaps = 1;
	desc.srgb = 0;

	/*  Sampler.    */
	desc.sampler.AddressU = SamplerDesc::eRepeat;
	desc.sampler.AddressV = SamplerDesc::eRepeat;
	desc.sampler.AddressW = SamplerDesc::eRepeat;
	desc.sampler.anisotropy = 8.0f;
	desc.sampler.minFilter = SamplerDesc::eLinear;
	desc.sampler.magFilter = SamplerDesc::eLinear;
	desc.sampler.mipmapFilter = SamplerDesc::eLinear;
	desc.sampler.maxLOD = 0;
	desc.sampler.minLOD = 0;
	desc.sampler.biasLOD = 0;

	return renderer->createTexture(&desc);
}

TextureUtil::TextureUtil(void) {}
