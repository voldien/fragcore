#ifndef _FRAG_CORE_SDLDISPLAY_H_
#define _FRAG_CORE_SDLDISPLAY_H_ 1
#include"../../Def.h"
#include"../Display.h"

namespace fragcore {

	/**
	 * 
	 */
	class FVDECLSPEC SDLDisplay : public Display {
	public:
		SDLDisplay(int index);

		unsigned int x(void) const override;

		unsigned int y(void) const override;

		unsigned int width(void) const override;

		unsigned int height(void) const override;

		unsigned int refreshRate(void) const override;

		std::vector<Mode> getModes(void) const override;

		void getDPI(DPI *dpi) override;

		fragcore::TextureFormat getFormat(void) override;

	protected:
		unsigned int translateFormat(unsigned int format);
	private:
		unsigned int index;
	public:
		static int getNumDisplays(void);
	};
}

#endif