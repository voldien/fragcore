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
#ifndef _FRAG_CORE_GL_SAMPLER_H_
#define _FRAG_CORE_GL_SAMPLER_H_ 1
#include "../Sampler.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC GLSampler : public Sampler {
		friend class IRenderer;

	  public:
		GLSampler() = default;

		~GLSampler() override;

		void setFilterMode(FilterMode mode) override;

		FilterMode getFilterMode() override;

		void setWrapMode(WrapMode mode) override;

		WrapMode getWrapMode() override;

		//

		void setAnisotropic(float anisotropy) override;

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

		void setName(const std::string &name) override;

		intptr_t getNativePtr() const override;

	  public:
		unsigned int getSampler() const noexcept { return this->sampler; }

	  private:
		unsigned int sampler;
	};
} // namespace fragcore

#endif
