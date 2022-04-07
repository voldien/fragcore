#include "ImageLoader.h"
#include <Core/IO/IOUtil.h>
#include <FreeImage.h>

using namespace fragcore;

Image ImageLoader::loadImage(Ref<IO> &io) {

	/*	Free image.	*/
	FREE_IMAGE_FORMAT imgtype; /**/
	FIMEMORY *stream;		   /**/
	FIBITMAP *firsbitmap;	   /**/
	void *pixelData;		   /**/
	unsigned int bitsPerPixel;
	TextureFormat imageFormat = TextureFormat::Alpha8;
	size_t pixelSize = 0;
	int width, height, depth;

	char *imageData;
	size_t imageSize = IOUtil::loadFileMem(io, &imageData);

	/*	1 byte for the size in order, Because it crash otherwise if set to 0.	*/
	stream = FreeImage_OpenMemory((BYTE *)imageData, imageSize);
	if (stream == nullptr)
		throw RuntimeException("Failed to open freeimage memory stream. \n");

	/*	Seek to beginning of the memory stream.	*/
	FreeImage_SeekMemory(stream, 0, SEEK_SET);

	/*	Load image from */
	imgtype = FreeImage_GetFileTypeFromMemory(stream, imageSize);
	FreeImage_SeekMemory(stream, 0, SEEK_SET);
	firsbitmap = FreeImage_LoadFromMemory(imgtype, stream, 0);
	/*	*/
	if (firsbitmap == nullptr) {
		FreeImage_CloseMemory(stream);
		throw RuntimeException("Failed to create free-image from memory.\n");
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
		// case FREE_IMAGE_TYPE::FIT_RGB16:
		// 	imageFormat = TextureFormat::RGB24;
		// 	break;
		// case FREE_IMAGE_TYPE::FIT_RGBAF:
		// 	imageFormat = TextureFormat::RGFloat;
		// 	break;
		default:
			throw NotSupportedException("Non Supported");
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
			throw NotSupportedException("Non Supported");
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
		throw NotSupportedException("Non Supported");
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