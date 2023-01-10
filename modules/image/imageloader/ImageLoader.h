/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_IMAGE_LOADER_H_
#define _FRAG_CORE_IMAGE_LOADER_H_ 1
#include "Image.h"
#include <Core/IO/FileSystem.h>
#include <Core/IO/IO.h>
#include <Core/Object.h>
#include <Core/Ref.h>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC ImageLoader : public Object {
	  public:
		ImageLoader() = default;
		ImageLoader(const ImageLoader &other);
		ImageLoader(ImageLoader &&other);
		ImageLoader & operator=(const ImageLoader &other);
		ImageLoader &operator=(ImageLoader &&other);
		virtual ~ImageLoader() = default;

		// TODO add support to load image in specific format.
		Image loadImage(const std::string &path) {
			FileSystem::createFileSystem();
			Ref<IO> ref = Ref<IO>(FileSystem::getFileSystem()->openFile(path.c_str(), IO::IOMode::READ));
			return loadImage(ref);
		}
		Image loadImage(Ref<IO> &io);

		void loadImageData(const std::string &path, unsigned int *width, unsigned int *height);

		void saveImage(const std::string &path, Image &image){
			Ref<IO> io = Ref<IO>(new FileIO(path.c_str(), FileIO::WRITE));
			saveImage(io, image);
			io->close();
		}
		void saveImage(Ref<IO> &IO, Image &image);
	};
} // namespace fragcore

#endif
