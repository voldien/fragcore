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
#ifndef _FRAGCORE_IMAGE_LOADER_H_
#define _FRAGCORE_IMAGE_LOADER_H_ 1
#include "Image.h"
#include <Core/Object.h>
#include <Core/Ref.h>
#include <IO/FileSystem.h>
#include <IO/IO.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC ImageLoader : public Object {
	  public:
		enum class FileFormat { Default, Png, Jpeg, Exr };
		enum class Compression { None, Default };

	  public:
		ImageLoader() = default;
		ImageLoader(const ImageLoader &other);
		ImageLoader(ImageLoader &&other);
		ImageLoader &operator=(const ImageLoader &other);
		ImageLoader &operator=(ImageLoader &&other);
		~ImageLoader() override = default;

		Image loadImage(const std::string &path, const FileFormat fileformat = FileFormat::Default) {
			FileSystem::createFileSystem();
			Ref<IO> io_ref = Ref<IO>(FileSystem::getFileSystem()->openFile(path.c_str(), IO::IOMode::READ));
			return loadImage(io_ref);
		}
		Image loadImage(Ref<IO> &io_in);

		void loadImageData(const std::string &path, unsigned int *width, unsigned int *height);

		void saveImage(const std::string &path, const Image &image, const FileFormat fileformat = FileFormat::Default) {
			Ref<IO> io_out = Ref<IO>(new FileIO(path.c_str(), FileIO::WRITE));
			saveImage(io_out, image, fileformat);
			io_out->close();
		}
		void saveImage(Ref<IO> &in_out, const Image &image, const FileFormat fileformat);
	};
} // namespace fragcore

#endif
