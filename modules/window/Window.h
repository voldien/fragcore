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
#ifndef _FRAG_CORE_WINDOW_H_
#define _FRAG_CORE_WINDOW_H_ 1
#include "Display.h"
#include <Core/Math.h>
#include <Core/SmartReference.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Window : public SmartReference {
	  public:
		virtual ~Window() = default;

	  public:
		virtual void show() = 0;

		virtual void hide() = 0;

		virtual void close() = 0;

		virtual void focus() = 0;

		virtual void restore() = 0;

		virtual void maximize() = 0;

		virtual void minimize() = 0;

		virtual void setTitle(const std::string &title) = 0;

		virtual std::string getTitle() const = 0;

		// TODO change the type to image.
		virtual void setIcon(void *) = 0;
		//		virtual void setIcon(Image& image) = 0;

		virtual void *getIcon() const = 0;
		// virtual Image getIcon() const = 0;
		//		virtual Image* setIcon(Image* image) = 0;

		virtual void getPosition(int *x, int *y) const = 0;

		virtual void setPosition(int x, int y) = 0;

		virtual void setSize(int width, int height) = 0;

		virtual void getSize(int *width, int *height) const = 0;

		virtual int width() const = 0;
		virtual int height() const = 0;

		virtual Display *getCurrentDisplay() const = 0;

		virtual float getGamma() const = 0;

		virtual void setGamma(float gamma) = 0;

		virtual void resizable(bool resizable) = 0;

		virtual void setFullScreen(bool fullscreen) = 0;
		virtual void setFullScreen(Display &display) = 0;

		virtual bool isFullScreen() const = 0;

		virtual void setBordered(bool borded) = 0;

		virtual void setMinimumSize(int width, int height) = 0;
		virtual void getMinimumSize(int *width, int *height) = 0;
		virtual void setMaximumSize(int width, int height) = 0;
		virtual void getMaximumSize(int *width, int *height) = 0;

		FV_ALWAYS_INLINE void setUserData(void *userData) noexcept { this->userData = userData; }

		FV_ALWAYS_INLINE virtual void *getUserData() const noexcept { return this->userData; }

		virtual intptr_t getNativePtr() const = 0; /*  Get native window reference object. */

		// TODO add getWindowManager()

	  protected: /*  Internal utility methods.   */
		template <typename T, typename U> void calculateGammaLookupTable(float gamma, U *rgbRamp) const {

			// TODO that or just clamp it.
			if (gamma < 0.0)
				throw InvalidArgumentException("gamma exponent must be positive");
			gamma = Math::min(0.0f, gamma);

			float exponent = 1.0f / gamma;

			/*  Create lookup table.    */
			for (uint i = 0; i < 256; ++i) {
				/*  */
				float linear = float(i) * 1.0f / 255u;
				float corrected = (float)std::pow(linear, exponent);
				U entry = U(corrected * 65535);

				rgbRamp[i] = rgbRamp[i + 256] = rgbRamp[i + 512] = entry;
			}
		}
		template <typename T, typename U> T computeGammaExponent(const U *rgbRamp) const {
			T gamma = static_cast<T>(0.0);

			// TODO improve
			for (size_t i = 64; i < 256; ++i) {
				double corrected = (double)rgbRamp[i] / 65535.0;
				double linear = double(i) * 1.0 / 255u;
				gamma += std::log(linear) / std::log(corrected);
			}
			return gamma;
		}

	  protected:
		void *userData = nullptr;
	};
} // namespace fragcore

#endif
