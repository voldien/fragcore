/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAG_CORE_GL_TEXTURE_H_
#define _FRAG_CORE_GL_TEXTURE_H_ 1
#include "../Texture.h"
#include <GL/glew.h>

namespace fragcore {
	/**
	 *	Texture object.
	 */
	class FVDECLSPEC GLTexture : public Texture {
		friend class IRenderer;

	  public:
		GLTexture() = default;
		~GLTexture() override;

		/**
		 *
		 * @param index
		 */
		virtual void bind(unsigned int index) override;

		/**
		 *
		 * @param index
		 * @param level
		 * @param format
		 */
		virtual void bindImage(unsigned int index, int level, MapTarget target, Format format) override;

		/**
		 *
		 * @return
		 */
		virtual bool isValid() override;

		/**
		 *
		 * @param sampler
		 */
		virtual void setSampler(Sampler *sampler) override;

		/**
		 *
		 * @param level
		 */
		virtual void setMipLevel(unsigned int level) override;

		/**
		 *
		 * @param mode
		 */
		virtual void setFilterMode(FilterMode mode) override;

		/**
		 *
		 * @return
		 */
		virtual FilterMode getFilterMode() override;

		/**
		 *
		 * @param mode
		 */
		virtual void setWrapMode(WrapMode mode) override;

		/**
		 *
		 * @return
		 */
		virtual WrapMode getWrapMode() override;

		/**
		 *
		 * @param anisotropic
		 */
		virtual void setAnisotropic(float anisotropic) override;

		/**
		 *
		 * @return
		 */
		virtual float getAnisotropic() const override;

		virtual CompareFunc getCompare() const override;

		virtual void setCompareFunc(CompareFunc compareFunc) override;

		void setMipMapBaseLevel(unsigned int level) override;

		unsigned int getMipMapBaseLevel() const override;

		void setMipMapBias(float bias) override;

		float getMipMapBias(float bias) const override;

		void setBorderColor(float color) override;

		float getBorderColor() const override;

		unsigned int setMaxLod(unsigned int level) override;

		unsigned int getMaxLod() const override;

		unsigned int setMinLod(unsigned int level) override;

		unsigned int getMinLod() const override;

		virtual Format getFormat() const override;

		// TODO set const for the width and height
		virtual unsigned int width() override;

		virtual unsigned int height() override;

		virtual unsigned int layers() const override;

		// TODO add mip map streaming.

		virtual void resize(int width, int height, Texture::Format format, bool hasMipMap) override;

		/**
		 *
		 * @param size
		 * @return
		 */
		virtual void *mapTexture(Format format, unsigned int level) override; // TODO add map target.

		/**
		 *
		 */
		virtual void unMapTexture() override;

		// TOOD determine of range mapping is possible with flushing and etc.

		/**
		 *
		 * @param pixels
		 * @param size
		 */
		virtual void setPixels(Format format, unsigned int level, const void *pixels,
							   unsigned long size) override; // TODO add fvformatf

		/**
		 *
		 * @param mipmap
		 * @return
		 */
		virtual void *getPixels(TextureFormat format, unsigned int level,
								unsigned long *nBytes) override; // TOOD add fvformatf.

		// virtual bool UseSRGB();
		virtual void clear() override;

		// TODO add get native ptr object.
		virtual intptr_t getNativePtr() const override;

		virtual void setName(const std::string &name) override;

	  public:
		unsigned int getTarget() const noexcept { return this->target; }
		unsigned int getTexture() const noexcept { return this->texture; }

	  private:
		TextureDesc desc;
		unsigned int target;
		unsigned int texture;
		unsigned int pbo; /*  TODO resolve if to relocate.    */
		Sampler *sampler;
	};
} // namespace fragcore

#endif
