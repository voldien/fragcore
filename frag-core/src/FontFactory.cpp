#include "FontFactory.h"
#include "Core/IO/FileSystem.h"
#include "Core/IO/IOUtil.h"
#include "Core/Math.h"
#include "Utils/TextureUtil.h"
#include <cassert>
#include <cmath>
#include<fmt/core.h>
#include <ft2build.h>
#include FT_FREETYPE_H
using namespace fragcore;

Font *FontFactory::createFont(Ref<IRenderer> &renderer, Ref<IO> &io, float size, Encoding encoding) {
	Font *object;
	Texture *fontTexture;

	/*  */
	size_t numChar = 128;
	const size_t nrFontFields = 9;
	std::map<unsigned int, Font::Character> characters;
	int i, j, l;
	FT_Error ftError;
	FT_Library ft;
	FT_Glyph_Metrics *metrics;
	FT_Face face;
	FT_GlyphSlot slot;
	FT_Matrix matrix; /* transformation matrix */
	FT_Vector pen;	  /* untransformed origin  */
	FT_Bitmap *bmp;
	int glyph_width;
	int glyph_height;
	int num_segments_x = numChar;
	int num_segments_y = numChar;
	int segment_size_x = 0;
	int segment_size_y = 0;
	int font_tex_width;
	int font_tex_height;
	int bitmap_offset_x = 0, bitmap_offset_y = 0;

	/*  Validate arguments. */
	if (renderer == nullptr)
		throw InvalidArgumentException("Requires none-null renderer object.");
	if (size <= 0.0f)
		throw InvalidArgumentException("Requires size to be greater than 0.");

	/*  Init library.   */
	ftError = FT_Init_FreeType(&ft);
	if (ftError != FT_Err_Ok)
		throw InvalidArgumentException(fmt::format("Failed to initialize FreeType - {}.\n", ftError));

	// TODO add support for IO object.
	/*  Load font face by the path. */
	char *buf;
	long int nBytes = IOUtil::loadFileMem(io, &buf);
	ftError = FT_New_Memory_Face(ft, (const FT_Byte *)buf, nBytes, 0, &face);
	if (ftError != FT_Err_Ok) {
		FT_Done_FreeType(ft);
		throw InvalidArgumentException(fmt::format("Failed to load font - {}.\n", ftError));
	}

	FT_Encoding ft_encoding = FT_ENCODING_NONE; // TODO determine the default enum value.
	if (encoding == Encoding::UTF8 || encoding == Encoding::UTF16)
		ft_encoding = FT_ENCODING_UNICODE;
	else
		ft_encoding = FT_ENCODING_UNICODE;

	/*	*/
	ftError = FT_Select_Charmap(face, ft_encoding);
	if (ftError != FT_Err_Ok) {
		FT_Done_FreeType(ft);
		throw InvalidArgumentException(fmt::format("Failed to load font - {}.\n", ftError));
	}

	/*  Set the size of the font.   */
	ftError = FT_Set_Char_Size(face, 0, (int)size << 6, 96, 96); // TODO get the DPI
	if (ftError != FT_Err_Ok) {
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		throw InvalidArgumentException(fmt::format("Failed to set character size - {}.\n", ftError));
	}

	/*	First calculate the max width and height of a character in a passed font	*/
	for (i = 0; i < numChar; i++) {

		/*  Load character. */
		ftError = FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		if (ftError != FT_Err_Ok) {
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
			throw InvalidArgumentException(fmt::format("FT_Load_Char failed - {}.\n", ftError));
		}

		/*  */
		slot = face->glyph;
		bmp = &slot->bitmap;

		/*  Character bitmap size.  */
		glyph_width = bmp->width;
		glyph_height = bmp->rows;

		/*  Compute max width and height.   */
		if (glyph_width > segment_size_x) {
			segment_size_x = glyph_width;
		}

		if (glyph_height > segment_size_y) {
			segment_size_y = glyph_height;
		}
	}

	/*  Compute texture size.    */
	font_tex_width = Math::NextPowerOfTwo(num_segments_x);
	font_tex_height = Math::NextPowerOfTwo(num_segments_y);

	int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(numChar));
	font_tex_width = 1;
	while (font_tex_width < max_dim) {
		font_tex_width <<= 1;
	}
	font_tex_height = font_tex_width;

	/*	Allocate pixel data.    */
	const unsigned int fontPixelSize = sizeof(char) * font_tex_width * font_tex_height;
	char *font_texture_data = (char *)malloc(fontPixelSize);
	memset((void *)font_texture_data, 0, fontPixelSize);

	int pen_x = 0, pen_y = 0;

	/*  Iterate through each character. */
	for (i = 0; i < numChar; i++) {

		/*  Load character. */
		ftError = FT_Load_Char(face, i, FT_LOAD_RENDER);
		if (ftError != FT_Err_Ok) {

			/*  Release all resources.    */
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
			free(font_texture_data);
			throw InvalidArgumentException("FT_Load_Char failed - {}.\n", ftError);
		}
		FT_Bitmap *bmp = &face->glyph->bitmap;

		/*  Rotate the face.    */
		matrix.xx = (FT_Fixed)(-cos(Math::PI) * 0x10000L);
		matrix.xy = (FT_Fixed)(-sin(Math::PI) * 0x10000L);
		matrix.yx = (FT_Fixed)(sin(Math::PI) * 0x10000L);
		matrix.yy = (FT_Fixed)(cos(Math::PI) * 0x10000L);

		// /*  */
		pen.x = 0;
		pen.y = 0;
		FT_Set_Transform(face, &matrix, &pen);

		/*  */
		slot = face->glyph;
		bmp = &slot->bitmap;

		/*  Get power of two size.  */
		glyph_width = bmp->width;
		glyph_height = bmp->rows;

		if (bmp->buffer) {
			if (pen_x + bmp->width >= font_tex_width) {
				pen_x = 0;
				pen_y += ((face->size->metrics.height >> 6) + 1);
			}
			for (int row = 0; row < glyph_height; ++row) {
				for (int col = 0; col < glyph_width; ++col) {
					int x = pen_x + col;
					int y = pen_y + row;
					font_texture_data[y * font_tex_width + x] = bmp->buffer[row * bmp->pitch + col];
				}
			}
		}

		pen_x += bmp->width + 1;

		/*  Set character properties.   */
		Font::Character character;
		character.Advance = (float)(slot->advance.x >> 6);
		character.tex_x1 = (float)bitmap_offset_x / (float)font_tex_width;
		character.tex_x2 = (float)(bitmap_offset_x + bmp->width) / (float)font_tex_width;
		character.tex_y1 = (float)bitmap_offset_y / (float)font_tex_height;
		character.tex_y2 = (float)(bitmap_offset_y + bmp->rows) / (float)font_tex_height;
		character.width = bmp->width;
		character.height = bmp->rows;
		character.offset_x = (float)slot->bitmap_left;
		character.offset_y = (float)((slot->metrics.horiBearingY - face->glyph->metrics.height) >> 6);
		characters[i] = character;
	}

	/*  Release resources.  */
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	/*  Allocate texture.   */
	/*  Texture attributes. */
	TextureDesc desc = {0};
	desc.width = font_tex_width;
	desc.height = font_tex_height;
	desc.depth = 1;
	desc.pixel = font_texture_data;
	desc.pixelSize = fontPixelSize;
	desc.compression = TextureDesc::eNoCompression;
	desc.nrSamples = 0;

	/*  Texture.    */
	desc.target = TextureDesc::eTexture2D;
	desc.format = TextureDesc::eSingleColor;
	desc.internalformat = TextureDesc::eRGBA;

	desc.pixelFormat = TextureFormat::R8;
	desc.graphicFormat = GraphicFormat::R8G8B8_SRGB;
	desc.numlevel = 3;
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
	desc.sampler.maxLOD = 0;
	desc.sampler.minLOD = 0;
	desc.sampler.biasLOD = 0;

	fontTexture = renderer->createTexture(&desc);

	// TODO remove
	TextureUtil::saveTexture("test.png", *renderer, fontTexture);

	/*  Free resources. */
	// free(signedTexture);
	free(font_texture_data);

	/*  Create texture object.  */
	object = new Font();
	object->texture = fontTexture;
	object->characters = characters;
	return object;
}

Font *FontFactory::createSDFFont(Ref<IRenderer> &renderer, Ref<IO> &io, float size, Encoding encoding) {
	Font *object;
	Texture *fontTexture;

	/*  */
	const size_t numChar = 256;
	const size_t nrFontFields = 9;
	std::map<unsigned int, Font::Character> characters;
	int i, j, l;
	FT_Error ftError;
	FT_Library ft;
	FT_Glyph_Metrics *metrics;
	FT_Face face;
	FT_GlyphSlot slot;
	FT_Matrix matrix; /* transformation matrix */
	FT_Vector pen;	  /* untransformed origin  */
	FT_Bitmap *bmp;
	int glyph_width;
	int glyph_height;
	int num_segments_x = numChar;
	int num_segments_y = numChar;
	int segment_size_x = 0;
	int segment_size_y = 0;
	int font_tex_width;
	int font_tex_height;
	int bitmap_offset_x = 0, bitmap_offset_y = 0;

	/*  Validate arguments. */
	if (renderer == nullptr)
		throw InvalidArgumentException("Requires none-null renderer object.");
	if (size <= 0.0f)
		throw InvalidArgumentException("Requires size to be greater than 0.");

	/*  Init library.   */
	ftError = FT_Init_FreeType(&ft);
	if (ftError != FT_Err_Ok)
		throw InvalidArgumentException(fmt::format("Failed to initialize FreeType - {}.\n", ftError));

	// TODO add support for IO object.
	/*  Load font face by the path. */
	char *buf;
	long int nBytes = IOUtil::loadFileMem(io, &buf);
	ftError = FT_New_Memory_Face(ft, (const FT_Byte *)buf, nBytes, 0, &face);
	if (ftError != FT_Err_Ok) {
		FT_Done_FreeType(ft);
		throw InvalidArgumentException(fmt::format("Failed to load font - {}.\n", ftError));
	}

	ftError = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	if (ftError != FT_Err_Ok) {
		FT_Done_FreeType(ft);
		throw InvalidArgumentException(fmt::format("Failed to load font - {}.\n", ftError));
	}

	/*  Set the size of the font.   */
	// ftError = FT_Set_Char_Size(face, 0, size, 96, 96);   //TODO get the DPI
	ftError = FT_Set_Pixel_Sizes(face, 96, 96);
	if (ftError != FT_Err_Ok) {
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		throw InvalidArgumentException(fmt::format("Failed to set character size - {}.\n", ftError));
	}

	/*	First calculate the max width and height of a character in a passed font	*/
	for (i = 0; i < numChar; i++) {

		/*  Load character. */
		ftError = FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		if (ftError != FT_Err_Ok) {
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
			throw InvalidArgumentException(fmt::format("FT_Load_Char failed - {}.\n", ftError));
		}

		/*  */
		slot = face->glyph;
		bmp = &slot->bitmap;

		/*  Character bitmap size.  */
		glyph_width = bmp->width;
		glyph_height = bmp->rows;

		/*  Compute max width and height.   */
		if (glyph_width > segment_size_x) {
			segment_size_x = glyph_width;
		}

		if (glyph_height > segment_size_y) {
			segment_size_y = glyph_height;
		}
	}

	/*  Compute texture size.    */
	font_tex_width = Math::NextPowerOfTwo(num_segments_x * segment_size_x);
	font_tex_height = Math::NextPowerOfTwo(num_segments_y * segment_size_y);

	int max_dim = (1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(128));
	font_tex_width = 1;
	while (font_tex_width < max_dim)
		font_tex_width <<= 1;
	font_tex_height = font_tex_width;

	const int sdf_div = 8;
	font_tex_height /= sdf_div;
	font_tex_width /= sdf_div;

	/*	Allocate pixel data.    */
	const unsigned int fontPixelSize = sizeof(char) * font_tex_width * font_tex_height;
	char *font_texture_data = (char *)malloc(fontPixelSize);
	memset((void *)font_texture_data, 0, fontPixelSize);

	int pen_x = 0, pen_y = 0;

	/*  Iterate through each character. */
	for (i = 0; i < numChar; i++) {

		/*  Load character. */
		ftError = FT_Load_Char(face, i, FT_LOAD_RENDER);
		if (ftError != FT_Err_Ok) {

			/*  Release all resources.    */
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
			free(font_texture_data);
			throw InvalidArgumentException(fmt::format("FT_Load_Char failed - {}.\n", ftError));
		}
		FT_Bitmap *bmp = &face->glyph->bitmap;

		/*  Rotate the face.    */
		matrix.xx = (FT_Fixed)(-cos(Math::PI) * 0x10000L);
		matrix.xy = (FT_Fixed)(-sin(Math::PI) * 0x10000L);
		matrix.yx = (FT_Fixed)(sin(Math::PI) * 0x10000L);
		matrix.yy = (FT_Fixed)(cos(Math::PI) * 0x10000L);
		/*  */
		div_t temp = div(i, num_segments_x);
		/*  */
		bitmap_offset_x = segment_size_x * temp.rem;
		bitmap_offset_y = segment_size_y * temp.quot;
		/*  */
		pen.x = bitmap_offset_x;
		pen.y = bitmap_offset_y;
		FT_Set_Transform(face, &matrix, &pen);

		/*  */
		slot = face->glyph;
		bmp = &slot->bitmap;

		/*  Get power of two size.  */
		glyph_width = bmp->width;
		glyph_height = bmp->rows;

		int font_sdf_width = glyph_width / sdf_div;
		int font_sdf_height = glyph_height / sdf_div;

		if (bmp->buffer) {

			/*	*/
			if (pen_x + font_sdf_width >= font_tex_width) {
				pen_x = 0;
				pen_y += ((face->size->metrics.height >> 6) + 1);
			}

			for (int sdf_x = 0; sdf_x < font_sdf_width; sdf_x++) {
				for (int sdf_y = 0; sdf_y < font_sdf_height; sdf_y++) {
					int bitmap_x = ((float)sdf_x / (float)font_sdf_width) * glyph_width;
					int bitmap_y = ((float)sdf_y / (float)font_sdf_height) * glyph_height;
					int fg = bmp->buffer[bitmap_y * bmp->pitch + bitmap_x] == 0;
					int distance = 100000000;

					/*	*/
					for (int row = 0; row < glyph_height; ++row) {
						for (int col = 0; col < glyph_width; ++col) {
							int p = bmp->buffer[row * bmp->pitch + col];

							if (fg == p) {
								float dx = (col - bitmap_x) * 0.01f;
								dx *= dx;
								float dy = (row - bitmap_y) * 0.01f;
								dy *= dy;
								float t = sqrt(dx + dy);
								if (t < distance)
									distance = (int)t;
							} else {
								float dx = (col - bitmap_x) * 0.01f;
								dx *= dx;
								float dy = (row - bitmap_y) * 0.01f;
								dy *= dy;
								float t = sqrt(dx + dy);
								if (t < distance)
									distance = (int)t;
							}
						}
					}

					int x = pen_x + sdf_x;
					int y = pen_y + sdf_y;
					font_texture_data[y * font_tex_width + x] = distance;
				}
			}
		}

		pen_x += font_sdf_width + 1;

		/*  Set character properties.   */
		Font::Character character;
		character.Advance = (float)(slot->advance.x >> 6);
		character.tex_x1 = (float)bitmap_offset_x / (float)font_tex_width;
		character.tex_x2 = (float)(bitmap_offset_x + bmp->width) / (float)font_tex_width;
		character.tex_y1 = (float)bitmap_offset_y / (float)font_tex_height;
		character.tex_y2 = (float)(bitmap_offset_y + bmp->rows) / (float)font_tex_height;
		character.width = font_sdf_width;
		character.height = font_sdf_height;
		character.offset_x = (float)slot->bitmap_left;
		character.offset_y = (float)((slot->metrics.horiBearingY - face->glyph->metrics.height) >> 6);
		characters[i] = character;
	}

	/*  Release resources.  */
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	/*  Allocate texture.   */
	/*  Texture attributes. */
	TextureDesc desc = {};
	desc.width = font_tex_width;
	desc.height = font_tex_height;
	desc.depth = 1;
	desc.pixel = font_texture_data;
	desc.pixelSize = fontPixelSize;
	desc.compression = TextureDesc::eNoCompression;
	desc.nrSamples = 0;

	/*  Texture.    */
	desc.target = TextureDesc::eTexture2D;
	desc.format = TextureDesc::eSingleColor;
	desc.internalformat = TextureDesc::eRGBA;

	desc.pixelFormat = TextureFormat::R8;
	desc.graphicFormat = GraphicFormat::R8G8B8_SRGB;
	desc.numlevel = 3;
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
	desc.sampler.maxLOD = 0;
	desc.sampler.minLOD = 0;
	desc.sampler.biasLOD = 0;

	fontTexture = renderer->createTexture(&desc);

	// TODO remove
	TextureUtil::saveTexture("test-font-sdf.png", *renderer, fontTexture);

	/*  Create texture object.  */
	object = new Font();
	object->texture = fontTexture;
	object->characters = characters;
	return object;
}