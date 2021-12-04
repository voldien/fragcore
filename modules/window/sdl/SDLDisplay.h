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
#ifndef _FRAG_CORE_SDLDISPLAY_H_
#define _FRAG_CORE_SDLDISPLAY_H_ 1
#include<Window/Display.h>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC SDLDisplay : public Display {
	  public:
		SDLDisplay(int index);

		unsigned int x() const override;

		unsigned int y() const override;

		unsigned int width() const override;

		unsigned int height() const override;

		unsigned int refreshRate() const override;

		std::vector<Mode> getModes() const override;

		void getDPI(DPI *dpi) override;

		// fragcore::TextureFormat getFormat() override;

	  protected:
		unsigned int translateFormat(unsigned int format);

	  private:
		unsigned int index;

	  public:
		static int getNumDisplays();
	};
} // namespace fragcore

#endif
