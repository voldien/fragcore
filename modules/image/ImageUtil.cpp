#include "ImageUtil.h"
#include "ImageFormat.h"

using namespace fragcore;

Image ImageUtil::convert2NormalMap(const Image &image) {

	Image normalMap(image.width(), image.height(), fragcore::ImageFormat::RGB24);

	for (unsigned int index_x = 0; index_x < image.width(); index_x++) {
		for (unsigned int index_y = 0; index_y < image.height(); index_y++) {

			const Color tl = image.getColor((index_x + -1) % image.width(), (index_y + -1) % image.height(), 0);
			const Color l = image.getColor((index_x + -1) % image.width(), (index_y + 0) % image.height(), 0);
			const Color bl = image.getColor((index_x + -1) % image.width(), (index_y + 1) % image.height(), 0);
			const Color t = image.getColor((index_x + 0) % image.width(), (index_y + -1) % image.height(), 0);
			const Color b = image.getColor((index_x + 0) % image.width(), (index_y + 1) % image.height(), 0);
			const Color tr = image.getColor((index_x + 1) % image.width(), (index_y + -1) % image.height(), 0);
			const Color r = image.getColor((index_x + 1) % image.width(), (index_y + 0) % image.height(), 0);
			const Color br = image.getColor((index_x + 1) % image.width(), (index_y + 1) % image.height(), 0);

			const float strength = 8.0f;

			// sobel filter
			const float dX = (tr.r() + 2.0 * r.r() + br.r()) - (tl.r() + 2.0 * l.r() + bl.r());
			const float dY = (bl.r() + 2.0 * b.r() + br.r()) - (tl.r() + 2.0 * t.r() + tr.r());
			const float dZ = 1;

			Color normal(dX, dY, dZ, 0);
			// normal.normalize();

			normalMap.setColor(index_x, index_y, 0, normal);
		}
	}

	return normalMap;
}