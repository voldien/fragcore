#include "ImageLoader.h"
#include <Core/IO/IOUtil.h>
#include <FreeImage.h>
#include <magic_enum.hpp>

using namespace fragcore;

ImageLoader::ImageLoader(const ImageLoader &other) {}
ImageLoader::ImageLoader(ImageLoader &&other) {}
ImageLoader &ImageLoader::operator=(const ImageLoader &other) { return *this; }
ImageLoader &ImageLoader::operator=(ImageLoader &&other) { return *this; }

Image ImageLoader::loadImage(Ref<IO> &io) {

	/*	Free image.	*/
	FREE_IMAGE_FORMAT imgtype; /**/
	FIMEMORY *stream;		   /**/
	FIBITMAP *firsbitmap;	   /**/
	void *pixelData;		   /**/
	size_t bitsPerPixel;	   /*	*/
	TextureFormat imageFormat = TextureFormat::Alpha8;
	size_t pixelSize = 0;		 /*	*/
	size_t width, height, depth; /*	*/

	char *imageData = nullptr;

	size_t imageSize = IOUtil::loadFileMem(io, &imageData);

	/*	1 byte for the size in order, Because it crash otherwise if set to 0.	*/
	stream = FreeImage_OpenMemory((BYTE *)imageData, imageSize);
	if (stream == nullptr) {
		throw RuntimeException("Failed to open freeimage memory stream");
	}

	/*	Seek to beginning of the memory stream.	*/
	FreeImage_SeekMemory(stream, 0, SEEK_SET);

	/*	Load image from */
	imgtype = FreeImage_GetFileTypeFromMemory(stream, imageSize);
	FreeImage_SeekMemory(stream, 0, SEEK_SET);
	firsbitmap = FreeImage_LoadFromMemory(imgtype, stream, 0);
	/*	*/
	if (firsbitmap == nullptr) {
		FreeImage_CloseMemory(stream);
		throw RuntimeException("Failed load image memory");
	}

	/*	Reset to beginning of stream.	*/
	FreeImage_SeekMemory(stream, 0, SEEK_SET);

	/*	*/
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(firsbitmap);
	FREE_IMAGE_COLOR_TYPE colortype = FreeImage_GetColorType(firsbitmap);

	/*	Get attributes from the image.	*/
	pixelData = FreeImage_GetBits(firsbitmap);

	width = FreeImage_GetWidth(firsbitmap);
	height = FreeImage_GetHeight(firsbitmap);
	depth = 1;
	bitsPerPixel = FreeImage_GetBPP(firsbitmap);

	pixelSize = (width * height * depth * bitsPerPixel) / 8;

	// TODO fix, seems to be a bug with freeiamge.
	if (bitsPerPixel == 32 && colortype == FIC_RGB) {
		colortype = FIC_RGBALPHA;
	}

	/*	*/
	switch (colortype) {
	case FIC_RGB:
		switch (imageType) {
		case FREE_IMAGE_TYPE::FIT_BITMAP:
			imageFormat = TextureFormat::BGR24;
			break;
		case FREE_IMAGE_TYPE::FIT_RGB16:
			imageFormat = TextureFormat::RGB24;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBF:
			imageFormat = TextureFormat::RGBFloat;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBAF:
			imageFormat = TextureFormat::RGBAFloat;
			break;
		default:
			throw NotSupportedException("None Supported Color Type {} ", imageType);
		}
		break;
	case FIC_RGBALPHA:
		switch (imageType) {
		case FREE_IMAGE_TYPE::FIT_BITMAP:
			imageFormat = TextureFormat::BGRA32;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBA16: // TODO add unsigned int
			imageFormat = TextureFormat::RGBAHalf;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBAF:
			imageFormat = TextureFormat::RGBAFloat;
			break;
		default:
			throw NotSupportedException("None Supported Color Type");
		}
		break;
	case FIC_MINISWHITE:
	case FIC_MINISBLACK:

		switch (imageType) {
		case FREE_IMAGE_TYPE::FIT_BITMAP:
			imageFormat = TextureFormat::Alpha8;
			break;
		case FREE_IMAGE_TYPE::FIT_FLOAT:
		default:
			break;
		}
		break;
	case FIC_CMYK:
	default:
		throw NotSupportedException("None Supported Color Type {}", std::string(magic_enum::enum_name(colortype)));
	}

	/*  */
	Image image(width, height, depth, imageFormat);
	image.setPixelData(pixelData, pixelSize);

	/*	Release free image resources.	*/
	FreeImage_Unload(firsbitmap);
	FreeImage_CloseMemory(stream);
	free(imageData);

	return image;
}

void ImageLoader::saveImage(Ref<IO> &IO, Image &Image) {
	void *pixels = nullptr;
	FIMEMORY *fimemory = nullptr;
	FIBITMAP *image = nullptr;
	FIBITMAP *finalImage = nullptr;
	FREE_IMAGE_FORMAT image_format = FIF_UNKNOWN;
	std::string filepath; // = "save.png";

	const size_t size = Image.getSize();

	// /*  Validate argument.  */
	if (size <= 0) {
		throw RuntimeException("Invalid texture, yield invalid size.");
	}

	// /*  Read current pixels from mandelbrot set.    */
	pixels = Image.getPixelData();
	if (pixels == nullptr) {
		throw RuntimeException("Texture pixel mapping failed.");
	}

	FreeImage_Initialise(FALSE);

	// /*  Get file fmt::format.    */
	image_format = FIF_PNG; // FreeImage_GetFIFFromFilename(filepath.c_str());
	if (image_format == FIF_UNKNOWN) {
		throw InvalidArgumentException("filepath file format is not supported : {}", filepath);
	}

	// // TODO resolve color swizzle issue.
	Image.getFormat();

	// /*  Allocate image buffer.  */
	image = FreeImage_ConvertFromRawBits((BYTE *)pixels, Image.width(), Image.height(),
										 (Image::getFormatPixelSize(Image.getFormat()) * Image.width()) / 8,
										 Image::getFormatPixelSize(Image.getFormat()), 0x000000FF, 0x0000FF00,
										 0x00FF0000, FALSE);
	if (image == nullptr) {
		FreeImage_DeInitialise();
		throw RuntimeException("FreeImage_ConvertFromRawBits failed: {}", filepath);
	}

	// /*  */
	finalImage = FreeImage_ConvertTo32Bits(image);
	if (finalImage == nullptr) {
		throw RuntimeException("Failed convert image: {}", filepath);
	}

	// /*  Save to file.   */
	// FreeImage_SaveToHandle()

	FIMEMORY *mem = FreeImage_OpenMemory(nullptr, FreeImage_GetMemorySize(image));
	if (FreeImage_SaveToMemory(image_format, image, mem)) {

		BYTE *save_pixel_data;
		DWORD save_size;
		if (FreeImage_AcquireMemory(mem, &save_pixel_data, &save_size)) {
			IOUtil::saveFileMem(IO, (char *)save_pixel_data, save_size);
		}
	}

	/*  */
	FreeImage_CloseMemory(fimemory);

	FreeImage_Unload(image);
	FreeImage_DeInitialise();
}