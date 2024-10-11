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
#ifndef _FRAG_CORE_VK_SAMPLER_H_
#define _FRAG_CORE_VK_SAMPLER_H_ 1
#include "../Sampler.h"
#include <vulkan/vulkan.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VKSampler : public Sampler {
		friend class IRenderer;

	  public:
		virtual ~VKSampler();

		virtual void setFilterMode(FilterMode mode) override;

		virtual FilterMode getFilterMode() override;

		virtual void setWrapMode(WrapMode mode) override;

		virtual WrapMode getWrapMode() override;

		virtual void setAnisotropic(float anisotropy) override;

		virtual float getAnisotropic() const override;

		virtual CompareFunc getCompare() const override;

		virtual void setCompareFunc(CompareFunc compareFunc) override;

		virtual void setMipMapBaseLevel(unsigned int level) override;

		virtual unsigned int getMipMapBaseLevel() const override;

		virtual void setMipMapBias(float bias) override;

		virtual float getMipMapBias(float bias) const override;

		virtual void setBorderColor(float color) override;

		virtual float getBorderColor() const override;

		virtual unsigned int setMaxLod(unsigned int level) override;

		virtual unsigned int getMaxLod() const override;

		virtual unsigned int setMinLod(unsigned int level) override;

		virtual unsigned int getMinLod() const override;

		virtual void setName(const std::string &name) override;

		intptr_t getNativePtr() const override;

	  public:
		VkSampler getSampler() const noexcept { return this->sampler; }

	  private:
		VkSampler sampler{VK_NULL_HANDLE};
	};
} // namespace fragcore

#endif
