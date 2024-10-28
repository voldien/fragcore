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
#ifndef _FRAGCORE_SAMPLER_H_
#define _FRAGCORE_SAMPLER_H_ 1
#include "RenderObject.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Sampler : public RenderObject {
		friend class IRenderer;

	  public:
		~Sampler() override = default;

		enum WrapMode {
			eClamp = 0x1,  /*  */
			eRepeat = 0x2, /*  */
			eMirror = 0x3, /*  */
		};

		enum class CompareFunc {
			NoCompare,	  /*  */
			lessEqual,	  /*  */
			greaterEqual, /*  */
			less,		  /*  */
			greater,	  /*  */
			equal,		  /*  */
			notequal,	  /*  */
			always,		  /*  */
			never,		  /*  */
		};

		enum class FilterMode {
			eNearest = 0x1,	  /*	*/
			eBilinear = 0x2,  /*	*/
			eTrilinear = 0x3, /*  */
		};

		virtual void setFilterMode(FilterMode mode) = 0;

		virtual FilterMode getFilterMode() = 0;

		virtual void setWrapMode(WrapMode mode) = 0;

		virtual WrapMode getWrapMode() = 0;

		virtual void setAnisotropic(float anisotropy) = 0;

		virtual float getAnisotropic() const = 0;

		virtual CompareFunc getCompare() const = 0;

		virtual void setCompareFunc(CompareFunc compareFunc) = 0;

		virtual void setMipMapBaseLevel(unsigned int level) = 0;

		virtual unsigned int getMipMapBaseLevel() const = 0;

		virtual void setMipMapBias(float bias) = 0;

		virtual float getMipMapBias(float bias) const = 0;

		virtual void setBorderColor(float color) = 0;

		virtual float getBorderColor() const = 0;

		virtual unsigned int setMaxLod(unsigned int level) = 0;

		virtual unsigned int getMaxLod() const = 0;

		virtual unsigned int setMinLod(unsigned int level) = 0;

		virtual unsigned int getMinLod() const = 0;
	};
} // namespace fragcore

#endif
