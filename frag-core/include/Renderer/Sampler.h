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
#ifndef _FRAG_CORE_SAMPLER_H_
#define _FRAG_CORE_SAMPLER_H_ 1
#include "RenderObject.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Sampler : public RenderObject {
		friend class IRenderer;

	public:
	  Sampler(void) = default;

	  virtual ~Sampler(void);

	  enum WrapMode {
		  eClamp = 0x1,	 /*  */
		  eRepeat = 0x2, /*  */
		  eMirror = 0x3, /*  */
	  };

	  enum CompareFunc {
		  NoCompare,	/*  */
		  lessEqual,	/*  */
		  greaterEqual, /*  */
		  less,			/*  */
		  greater,		/*  */
		  equal,		/*  */
		  notequal,		/*  */
		  always,		/*  */
		  never,		/*  */
	  };

	  enum FilterMode {
		  eNearest = 0x1,	/*	*/
		  eBilinear = 0x2,	/*	*/
		  eTrilinear = 0x3, /*  */
	  };

	  virtual void setFilterMode(FilterMode mode);

	  virtual FilterMode getFilterMode(void);

	  virtual void setWrapMode(WrapMode mode);

	  virtual WrapMode getWrapMode(void);

	  virtual void setAnisotropic(float anisotropy);

	  virtual float getAnisotropic(void) const;

	  virtual CompareFunc getCompare(void) const;

	  virtual void setCompareFunc(CompareFunc compareFunc);

	  virtual void setMipMapBaseLevel(unsigned int level);

	  virtual unsigned int getMipMapBaseLevel(void) const;

	  virtual void setMipMapBias(float bias);

	  virtual float getMipMapBias(float bias) const;

	  virtual void setBorderColor(float color);

	  virtual float getBorderColor(void) const;

	  virtual unsigned int setMaxLod(unsigned int level);

	  virtual unsigned int getMaxLod(void) const;

	  virtual unsigned int setMinLod(unsigned int level);

	  virtual unsigned int getMinLod(void) const;

	  virtual void setName(const std::string &name) override;

	  intptr_t getNativePtr(void) const override;
	};
} // namespace fragcore

#endif
