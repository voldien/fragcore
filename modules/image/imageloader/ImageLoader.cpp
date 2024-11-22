#include "ImageLoader.h"
#include "ImageFormat.h"
#include <FreeImage.h>
#include <IO/IOUtil.h>
#include <magic_enum.hpp>

using namespace fragcore;

ImageLoader::ImageLoader(const ImageLoader &other) = default;
ImageLoader::ImageLoader(ImageLoader &&other) : Object(other) {}
ImageLoader &ImageLoader::operator=(const ImageLoader &other) { return *this; }
ImageLoader &ImageLoader::operator=(ImageLoader &&other) { return *this; }

Image ImageLoader::loadImage(Ref<IO> &io_in, const FileFormat fileformat) {

	/*	Free image.	*/
	FREE_IMAGE_FORMAT imgtype;		/**/
	FIMEMORY *stream = nullptr;		/**/
	FIBITMAP *firsbitmap = nullptr; /**/
	void *pixelData = nullptr;		/**/
	size_t bitsPerPixel = 0;		/*	*/
	ImageFormat imageFormat = ImageFormat::Alpha8;
	size_t pixelSize = 0;					 /*	*/
	size_t width = 0, height = 0, depth = 0; /*	*/

	char *imageData = nullptr;

	/*	Load image data.	*/
	size_t imageSize = IOUtil::loadFileMem(io_in, &imageData);

	/*	Create memory object.*/
	stream = FreeImage_OpenMemory((BYTE *)imageData, imageSize);
	if (stream == nullptr) {
		throw RuntimeException("Failed to open freeimage memory stream {0}", imageSize);
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
		throw RuntimeException("Failed load image memory: {0} : size {1}", magic_enum::enum_name(imgtype), imageSize);
	}

	/*	Reset to beginning of stream.	*/
	FreeImage_SeekMemory(stream, 0, SEEK_SET);

	/*	*/
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(firsbitmap);
	FREE_IMAGE_COLOR_TYPE colortype = FreeImage_GetColorType(firsbitmap);

	/*	Get attributes from the image.	*/
	pixelData = FreeImage_GetBits(firsbitmap);

	/*	*/
	width = FreeImage_GetWidth(firsbitmap);
	height = FreeImage_GetHeight(firsbitmap);
	depth = 1;
	bitsPerPixel = FreeImage_GetBPP(firsbitmap);

	/*	*/
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
			imageFormat = ImageFormat::BGR24;
			break;
		case FREE_IMAGE_TYPE::FIT_RGB16:
			imageFormat = ImageFormat::RGB24;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBF:
			imageFormat = ImageFormat::RGBFloat;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBAF:
			imageFormat = ImageFormat::RGBAFloat;
			break;
		default:
			throw NotSupportedException("None Supported (RGB) Color Type {} ", magic_enum::enum_name(imageType));
		}
		break;
	case FIC_RGBALPHA:
		switch (imageType) {
		case FREE_IMAGE_TYPE::FIT_BITMAP:
			imageFormat = ImageFormat::BGRA32;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBA16: // TODO add unsigned int
			imageFormat = ImageFormat::RGBAHalf;
			break;
		case FREE_IMAGE_TYPE::FIT_RGBAF:
			imageFormat = ImageFormat::RGBAFloat;
			break;
		default:
			throw NotSupportedException("None Supported (RGBA) Color Type {} ", magic_enum::enum_name(imageType));
		}
		break;
	case FIC_MINISWHITE:
	case FIC_MINISBLACK:
		switch (imageType) {
		case FREE_IMAGE_TYPE::FIT_BITMAP:
			imageFormat = ImageFormat::Alpha8;
			break;
		case FREE_IMAGE_TYPE::FIT_FLOAT:
			imageFormat = ImageFormat::RFloat;
			break;
		case FREE_IMAGE_TYPE::FIT_UINT16:
			imageFormat = ImageFormat::R16U;
			break;
		case FREE_IMAGE_TYPE::FIT_INT16:
			imageFormat = ImageFormat::R16;
			break;
		case FREE_IMAGE_TYPE::FIT_UINT32:
			imageFormat = ImageFormat::R32U;
			break;
		case FREE_IMAGE_TYPE::FIT_INT32:
			imageFormat = ImageFormat::R32;
			break;
		default:
			throw NotSupportedException("None Supported (Single Channel) Color Type {} ",
										magic_enum::enum_name(imageType));
		}
		break;
	case FIC_CMYK:
	default:
		throw NotSupportedException("None Supported (Unknown) Color Type {} ", magic_enum::enum_name(imageType));
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

void ImageLoader::saveImage(Ref<IO> &io_in, const Image &Image, const FileFormat fileformat) {

	void *pixels = nullptr;
	FIMEMORY *fimemory = nullptr;
	FIBITMAP *image = nullptr;
	FIBITMAP *finalImage = nullptr;
	FREE_IMAGE_FORMAT image_format = FIF_UNKNOWN;
	std::string filepath;

	const size_t size = Image.getSize();

	/*  Validate argument.  */
	if (size <= 0) {
		throw RuntimeException("Invalid texture, yield invalid size.");
	}

	/*  Read current pixels from mandelbrot set.    */
	pixels = Image.getPixelData();
	if (pixels == nullptr) {
		throw RuntimeException("Texture pixel mapping failed.");
	}

	FreeImage_Initialise(FALSE);

	/*  Get file format.    */
	switch (fileformat) {
	case FileFormat::Png:
		image_format = FIF_PNG;
		break;
	case FileFormat::Jpeg:
		image_format = FIF_JPEG;
		break;
	case FileFormat::Exr:
		image_format = FIF_EXR;
		break;
	default:
		image_format = FIF_UNKNOWN;
		break;
	}

	if (image_format == FIF_UNKNOWN) {
		throw InvalidArgumentException("filepath file format is not supported : {}", filepath);
	}

	// TODO resolve color swizzle issue.
	Image.getFormat();

	/*  Allocate image buffer.  */
	image = FreeImage_ConvertFromRawBits((BYTE *)pixels, Image.width(), Image.height(),
										 (Image::getFormatPixelBitSize(Image.getFormat()) * Image.width()) / 8,
										 Image::getFormatPixelBitSize(Image.getFormat()), 0x000000FF, 0x0000FF00,
										 0x00FF0000, FALSE);
	if (image == nullptr) {
		FreeImage_DeInitialise();
		throw RuntimeException("FreeImage_ConvertFromRawBits failed: {}", filepath);
	}

	/*  */
	finalImage = FreeImage_ConvertTo32Bits(image);
	if (finalImage == nullptr) {
		throw RuntimeException("Failed convert image: {}", filepath);
	}

	/*  Save to file.   */
	FIMEMORY *mem = FreeImage_OpenMemory(nullptr, FreeImage_GetMemorySize(image));
	if (FreeImage_SaveToMemory(image_format, image, mem, JPEG_QUALITYSUPERB)) {

		BYTE *save_pixel_data = nullptr;
		DWORD save_size = 0;
		if (FreeImage_AcquireMemory(mem, &save_pixel_data, &save_size)) {
			IOUtil::saveFileMem(io_in, (char *)save_pixel_data, save_size);
		}
	}

	/*  */
	FreeImage_CloseMemory(fimemory);

	FreeImage_Unload(image);
	FreeImage_DeInitialise();
}