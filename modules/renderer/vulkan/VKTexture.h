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
#ifndef _FRAGCORE_VK_TEXTURE_H_
#define _FRAGCORE_VK_TEXTURE_H_ 1
#include "../Texture.h"
#include "RenderDesc.h"
#include <vulkan/vulkan.h>

namespace fragcore {
	/**
	 *	Texture object.
	 */
	class FVDECLSPEC VKTexture : public Texture {
		friend class IRenderer;

	  public:
		VKTexture() = default;
		~VKTexture() override;

		/**
		 *
		 * @param index
		 */
		void bind(unsigned int index) override;

		/**
		 *
		 * @param index
		 * @param level
		 * @param format
		 */
		void bindImage(unsigned int index, int level, MapTarget target, Format format) override;

		/**
		 *
		 * @return
		 */
		bool isValid() override;

		/**
		 *
		 * @param sampler
		 */
		void setSampler(Sampler *sampler) override;

		/**
		 *
		 * @param level
		 */
		void setMipLevel(unsigned int level) override;

		/**
		 *
		 * @param mode
		 */
		void setFilterMode(FilterMode mode) override;

		/**
		 *
		 * @return
		 */
		FilterMode getFilterMode() override;

		/**
		 *
		 * @param mode
		 */
		void setWrapMode(WrapMode mode) override;

		/**
		 *
		 * @return
		 */
		WrapMode getWrapMode() override;

		/**
		 *
		 * @param anisotropic
		 */
		void setAnisotropic(float anisotropic) override;

		/**
		 *
		 * @return
		 */
		float getAnisotropic() const override;

		CompareFunc getCompare() const override;

		void setCompareFunc(CompareFunc compareFunc) override;

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

		Format getFormat() const override;

		// TODO set const for the width and height
		unsigned int width() override;

		unsigned int height() override;

		unsigned int layers() const override;

		// TODO add mip map streaming.

		void resize(int width, int height, Texture::Format format, bool hasMipMap) override;

		/**
		 *
		 * @param size
		 * @return
		 */
		void *mapTexture(Format format, unsigned int level) override; // TODO add map target.

		/**
		 *
		 */
		void unMapTexture() override;

		// TOOD determine of range mapping is possible with flushing and etc.

		/**
		 *
		 * @param pixels
		 * @param size
		 */
		void setPixels(Format format, unsigned int level, const void *pixels,
					   unsigned long size) override; // TODO add fvformatf

		/**
		 *
		 * @param mipmap
		 * @return
		 */
		void *getPixels(ImageFormat format, unsigned int level,
						unsigned long *nBytes) override; // TOOD add fvformatf.

		// virtual bool UseSRGB();
		void clear() override;

		// TODO add get native ptr object.
		intptr_t getNativePtr() const override;

		void setName(const std::string &name) override;

	  public:
		VkImage getImage() const noexcept { return this->texture; }
		VkImageView getImageView() const noexcept { return this->view; }

	  private:
		TextureDesc desc;
		unsigned int target;
		VkImage texture;
		VkDeviceMemory imageMemory;
		VkBuffer *imageBuffer;
		VkSampler sampler;
		VkImageView view;
	};
} // namespace fragcore

#endif
