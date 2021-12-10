/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_DEFAULTFRAMEBUFFERTEXTURE_H_
#define _FRAG_CORE_DEFAULTFRAMEBUFFERTEXTURE_H_ 1
#include "../Texture.h"

namespace fragcore {

	class FrameBufferTexture : public Texture {
	  public:
		FrameBufferTexture();

		virtual void *mapTexture(Format format, unsigned int level) override;

		virtual void unMapTexture() override;

		virtual Format getFormat() const override;

		void bind(unsigned int index) override;

		bool isValid() override;

		void setMipLevel(unsigned int level) override;

		void setFilterMode(FilterMode mode) override;

		FilterMode getFilterMode() override;

		void setWrapMode(WrapMode mode) override;

		WrapMode getWrapMode() override;

		void setAnisotropic(float anisotropic) override;

		float getAnisotropic() const override;

		void setPixels(Format format, unsigned int level, const void *pixels, unsigned long size) override;

		void *getPixels(Format format, unsigned int level, unsigned long *nBytes);

		virtual unsigned int width() override;

		virtual unsigned int height() override;

	  private:
		unsigned int pbo; // TODO determine
	};
} // namespace fragcore

#endif
