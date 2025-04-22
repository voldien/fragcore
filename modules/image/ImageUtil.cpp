#include "ImageUtil.h"
#include "ImageFormat.h"
#include "Math3D/Math3D.h"

using namespace fragcore;

Image ImageUtil::convert2NormalMap(const Image &image, const float strength) {

	Image normalMap(image.width(), image.height(), fragcore::ImageFormat::RGB24);

	const Vector2 size = Vector2(2.0, 0.0);

#ifdef _OPENMP
#pragma omp parallel for simd collapse(2) shared(image)
#endif
	for (unsigned int index_x = 0; index_x < image.width(); index_x++) {
		for (unsigned int index_y = 0; index_y < image.height(); index_y++) {

			const float s01 = image.getColor((index_x + 1) % image.width(), (index_y + 1) % image.height(), 0).x();
			const float s21 = image.getColor((index_x + -1) % image.width(), (index_y + 1) % image.height(), 0).x();
			const float s10 = image.getColor((index_x + 1) % image.width(), (index_y + -1) % image.height(), 0).x();
			const float s12 = image.getColor((index_x + -1) % image.width(), (index_y + -1) % image.height(), 0).x();
			const float bump_strength = 2.0f;

			Vector3 va = Vector3(size.x(), size.y(), strength * bump_strength * (s21 - s10));
			Vector3 vb = Vector3(size.y(), size.x(), strength * bump_strength * (s12 - s01));

			va.normalize();
			vb.normalize();
			Vector3 normal = va.cross(vb) * 0.5f + Vector3(0.5f, 0.5f, 0.5f);

			Color normalColor = Color(normal.x(), normal.y(), normal.z(), 1.0);
			normalMap.setColor(index_x, index_y, 0, normalColor);
		}
	}

	return normalMap;
}

Image ImageUtil::convert2GrayScale(const Image &image) {
	Image grayScale(image.width(), image.height(), fragcore::ImageFormat::RGB24);

#ifdef _OPENMP
#pragma omp parallel for simd collapse(2) shared(image)
#endif
	for (unsigned int index_x = 0; index_x < image.width(); index_x++) {
		for (unsigned int index_y = 0; index_y < image.height(); index_y++) {
			Color color = image.getColor(index_x, index_y, 0);
			const float lumen = 0.299 * color.r() + 0.587 * color.g() + 0.114 * color.b();
			Color grayScaleColor = Color(lumen, lumen, lumen, 1);
			grayScale.setColor(index_x, index_y, 0, grayScaleColor);
		}
	}

	return grayScale;
}