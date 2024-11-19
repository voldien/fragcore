#include "ImageUtil.h"
#include "ImageFormat.h"
#include "Math3D/Math3D.h"

using namespace fragcore;

Image ImageUtil::convert2NormalMap(const Image &image) {

	Image normalMap(image.width(), image.height(), fragcore::ImageFormat::RGB24);

	const Vector2 size = Vector2(2.0, 0.0);

#ifdef _OPENMP
#pragma omp parallel for simd shared(image)
#endif
	for (unsigned int index_x = 0; index_x < image.width(); index_x++) {
		for (unsigned int index_y = 0; index_y < image.height(); index_y++) {

			const float s01 = image.getColor((index_x + 1) % image.width(), (index_y + 1) % image.height(), 0).x();
			const float s21 = image.getColor((index_x + -1) % image.width(), (index_y + 1) % image.height(), 0).x();
			const float s10 = image.getColor((index_x + 1) % image.width(), (index_y + -1) % image.height(), 0).x();
			const float s12 = image.getColor((index_x + -1) % image.width(), (index_y + -1) % image.height(), 0).x();

			const float bump_strength = 2.0f;

			Vector3 va = bump_strength * Vector3(size.x(), size.y(), s21 - s10);
			Vector3 vb = bump_strength * Vector3(size.y(), size.x(), s12 - s01);
			va.normalize();
			vb.normalize();
			Vector3 normal = va.cross(vb) * 0.5f + Vector3(0.5f, 0.5f, 0.5f);
			const Color normalColor = Color(normal.x(), normal.y(), normal.z(), 1.0);

			normalMap.setColor(index_x, index_y, 0, normalColor);
		}
	}

	return normalMap;
}