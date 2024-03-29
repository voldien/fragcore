#include "TextureFactory.h"
#include "Core/Math.h"
#include <IRenderer.h>
#include <Texture.h>
#include <TextureUtil.h>

using namespace fragcore;

Texture *TextureFactory::createChecker(IRenderer *renderer, int width, int height) {
	void *pixels;

	assert(renderer && width > 0 && height > 0);

	createChecker(width, height, (char **)&pixels);
	int pixelSize = width * height * 3;

	Texture *texture = TextureUtil::createTexture(renderer, width, height, pixels, pixelSize, TextureFormat::R8,
												  GraphicFormat::R8G8B8_SRGB);

	free(pixels);
	return texture;
}

Texture *TextureFactory::createPerlinNoise(IRenderer *renderer, int width, int height) {
	char *pixels = nullptr;
	size_t pixelSize = static_cast<size_t>(width) * static_cast<size_t>(height) * 1;

	assert(renderer && width > 0 && height > 0);
	if (renderer == nullptr)
		throw InvalidArgumentException();

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			pixels[y * width + x] = Math::PerlinNoise(x * 0.001f, y * 0.001f);
		}
	}

	Texture *texture = TextureUtil::createTexture(renderer, width, height, pixels, pixelSize, TextureFormat::R8,
												  GraphicFormat::R8G8B8_SRGB);

	free(pixels);
	return texture;
}

void TextureFactory::createChecker(int width, int height, char **pixelsResult) {
	/*  TODO perform cleaning. */

	unsigned int x, y, Xpatter, Ypatter, bpp = 4;
	size_t pixelSize = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(bpp);

	*pixelsResult = (char *)malloc(pixelSize);

	int CheckerXDimension = 32;
	int CheckerYDimension = 32;

	Xpatter = (width / CheckerXDimension); // pattern count in X

	Ypatter = (height / CheckerYDimension); // pattern count in Y
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {

			float patternIndexX = (float)x / (float)CheckerXDimension;
			float patternIndexY = (float)y / (float)CheckerYDimension;

			switch (bpp) {
			case 1:
				if ((int)patternIndexX % 2 == 0) { // black
					pixelsResult[0][x * height * 1 + y * 1] = (((int)patternIndexY % 2 == 0) ? 0 : 255);
				} else { // white
					pixelsResult[0][x * height * 1 + y * 1] = (((int)patternIndexY % 2 == 0) ? 255 : 0);
				}

				continue;
			case 3:
				if ((int)patternIndexX % 2 == 0) {
					pixelsResult[0][x * height * 3 + y * 3] = (((int)patternIndexY % 2 == 0) ? 0 : 255);
					pixelsResult[0][x * height * 3 + y * 3 + 1] = (((int)patternIndexY % 2 == 0) ? 0 : 255);
					pixelsResult[0][x * height * 3 + y * 3 + 2] = (((int)patternIndexY % 2 == 0) ? 0 : 255);
				} else {
					pixelsResult[0][x * height * 3 + y * 3] = (((int)patternIndexY % 2 == 0) ? 255 : 0);
					pixelsResult[0][x * height * 3 + y * 3 + 1] = (((int)patternIndexY % 2 == 0) ? 255 : 0);
					pixelsResult[0][x * height * 3 + y * 3 + 2] = (((int)patternIndexY % 2 == 0) ? 255 : 0);
				}

				continue;
			case 4:
				if ((int)patternIndexX % 2 == 0) {
					pixelsResult[0][x * height * 4 + y * 4] = (((int)patternIndexY % 2 == 0) ? 0 : 255);
					pixelsResult[0][x * height * 4 + y * 4 + 1] = (((int)patternIndexY % 2 == 0) ? 0 : 255);
					pixelsResult[0][x * height * 4 + y * 4 + 2] = (((int)patternIndexY % 2 == 0) ? 0 : 255);
					pixelsResult[0][x * height * 4 + y * 4 + 3] = 255;
				} else {
					pixelsResult[0][x * height * 4 + y * 4] = (((int)patternIndexY % 2 == 0) ? 255 : 0);
					pixelsResult[0][x * height * 4 + y * 4 + 1] = (((int)patternIndexY % 2 == 0) ? 255 : 0);
					pixelsResult[0][x * height * 4 + y * 4 + 2] = (((int)patternIndexY % 2 == 0) ? 255 : 0);
					pixelsResult[0][x * height * 4 + y * 4 + 3] = 255;
				}
				continue;
			} // iteration of bpp
			continue;
		} // iteration of X
		continue;
	} // iteration of Y
}
