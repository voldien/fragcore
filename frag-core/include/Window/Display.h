/**
	FragEngine, A Two layer Game Engine.
    Copyright (C) 2016  Valdemar Lindberg

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
#ifndef _FRAG_CORE_DISPLAY_H_
#define _FRAG_CORE_DISPLAY_H_ 1
#include"../Def.h"
#include "../Core/Object.h"
#include"../Renderer/RenderDesc.h"
#include<vector>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Display : public Object {
	public:

		virtual unsigned int x(void) const = 0;

		virtual unsigned int y(void) const = 0;

		virtual unsigned int width(void) const = 0;

		virtual unsigned int height(void) const = 0;

		virtual unsigned int refreshRate(void) const = 0;

		typedef struct mode_t {
			int x;
			int y;
			unsigned int width;
			unsigned int height;
			unsigned int refreshRate;
			TextureFormat format;
		} Mode;

		typedef struct dpi_t {
			float ddpi;
			float hdpi;
			float vdpi;
		} DPI;

		typedef struct lut_t{
			
		} LUT;

		virtual std::vector<Mode> getModes(void) const = 0;
		//virtual void setMode(const Mode *mode);

		virtual void getDPI(DPI *dpi) = 0;

		virtual TextureFormat getFormat(void) = 0;

	public:
		// virtual void calculateGammaLookupTable(float gamma, ushort *rgbRamp) const; /*  */
		// virtual float computeGammaExponent(const ushort *rgbRamp) const;			/*  */

	};

}

#endif