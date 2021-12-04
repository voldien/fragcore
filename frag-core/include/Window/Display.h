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
#ifndef _FRAG_CORE_DISPLAY_H_
#define _FRAG_CORE_DISPLAY_H_ 1
#include "../Core/Object.h"
#include "../Def.h"
#include <vector>

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Display : public Object {
	  public:
		/**
		 * @brief Get display position
		 *
		 * @return unsigned int
		 */
		virtual unsigned int x() const = 0;

		/**
		 * @brief
		 *
		 * @return unsigned int
		 */
		virtual unsigned int y() const = 0;

		/**
		 * @brief
		 *
		 * @return unsigned int
		 */
		virtual unsigned int width() const = 0;

		/**
		 * @brief
		 *
		 * @return unsigned int
		 */
		virtual unsigned int height() const = 0;

		/**
		 * @brief
		 *
		 * @return unsigned int
		 */
		virtual unsigned int refreshRate() const = 0;

		typedef struct mode_t {
			int x;
			int y;
			unsigned int width;
			unsigned int height;
			unsigned int refreshRate;
			// TextureFormat format;
		} Mode;

		typedef struct dpi_t {
			float ddpi;
			float hdpi;
			float vdpi;
		} DPI;

		/**
		 * @brief Look up Table for each
		 * pixels value should be remapped to.
		 *
		 */
		typedef struct lut_t {

		} LUT;

		/**
		 * @brief Get the Modes object
		 *
		 * @return std::vector<Mode>
		 */
		virtual std::vector<Mode> getModes() const = 0;
		virtual void setMode(const Mode &mode) {}

		virtual void getDPI(DPI *dpi) = 0;

		// TODO resolve!
		// virtual TextureFormat getFormat() = 0;

	  public:
		// virtual void calculateGammaLookupTable(float gamma, ushort *rgbRamp) const; /*  */
		// virtual float computeGammaExponent(const ushort *rgbRamp) const;			/*  */
	};

} // namespace fragcore

#endif
