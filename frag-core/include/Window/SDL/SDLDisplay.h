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

		unsigned int x() const override;

		unsigned int y() const override;

		unsigned int width() const override;

		unsigned int height() const override;

		unsigned int refreshRate() const override;

		std::vector<Mode> getModes() const override;

		void getDPI(DPI *dpi) override;

		//fragcore::TextureFormat getFormat() override;

	protected:
		unsigned int translateFormat(unsigned int format);
	private:
		unsigned int index;
	public:
		static int getNumDisplays();
	};
}

#endif
