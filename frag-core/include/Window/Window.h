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
#ifndef _FRAG_CORE_WINDOW_H_
#define _FRAG_CORE_WINDOW_H_ 1
#include "../Core/SmartReference.h"
#include "Display.h"

namespace fragcore {
	
	/**
	 *
	 */
	class FVDECLSPEC Window : public SmartReference {
	public:
		virtual void show(void) = 0;

		virtual void hide(void) = 0;

		virtual void close(void) = 0;

		virtual void focus(void) = 0;

		virtual void restore(void) = 0;

		virtual void maximize(void) = 0;

		virtual void minimize(void) = 0;

		virtual void setTitle(const char *title) = 0;

		virtual const char *getTitle(void) const = 0;

		//TODO change the type to image.
		virtual void setIcon(void*) = 0;
//		virtual void setIcon(Image* image) = 0;

		virtual void* getIcon(void) const = 0;
//		virtual Image* setIcon(Image* image) = 0;

		virtual void getPosition(int *x, int *y) const = 0;

		virtual void setPosition(int x, int y) = 0;

		virtual void setSize(int width, int height) = 0;

		virtual void getSize(int *width, int *height) const = 0;

		virtual Display *getCurrentDisplay(void) const = 0;

		virtual float getGamma(void) const = 0;

		virtual void setGamma(float gamma) = 0;

		virtual void resizable(bool resizable) = 0;

		virtual void setFullScreen(bool fullscreen) = 0;
		virtual void setFullScreen(Display& display) = 0;

		virtual bool isFullScreen(void) const = 0;

		virtual void setBordered(bool borded) = 0;

		virtual void setMinimumSize(int width, int height) = 0;
		virtual void getMinimumSize (int* width, int* height) = 0;
		virtual void setMaximumSize(int width, int height) = 0;
		virtual void getMaximumSize (int* width, int* height) = 0;

		FV_ALWAYS_INLINE void setUserData(void *userData) {
			this->userData = userData;
		}

		FV_ALWAYS_INLINE virtual void *getUserData(void) const {
			return this->userData;
		}

		virtual intptr_t getNativePtr(void) const = 0;  /*  Get native window reference object. */

	protected:  /*  Internal utility methods.   */

		//TODO determine how to handle it.
		virtual void calculateGammaLookupTable(float gamma, ushort* rgbRamp) const; /*  */
		virtual float computeGammaExponent(const ushort* rgbRamp) const;            /*  */

	protected:
		void *userData = NULL;
	};
}

#endif