#ifndef _FRAG_CORE_DEFAULTFRAMEBUFFERTEXTURE_H_
#define _FRAG_CORE_DEFAULTFRAMEBUFFERTEXTURE_H_ 1
#include"../Texture.h"

namespace fragcore {

	class FrameBufferTexture : public Texture {
	public:


		FrameBufferTexture();

		virtual void *mapTexture(Format format, unsigned int level);

		virtual void unMapTexture();

		virtual Format getFormat() const;

		void bind(unsigned int index) override;

		bool isValid() override;

		void setMipLevel(unsigned int level) override;

		void setFilterMode(FilterMode mode) override;

		FilterMode getFilterMode() override;

		void setWrapMode(WrapMode mode) override;

		WrapMode getWrapMode() override;

		void setAnisotropic(float anisotropic) override;

		float getAnisotropic() const override;

		void setPixels(Format format, unsigned int level, const void *pixels, unsigned long size);

		void *getPixels(Format format, unsigned int level, unsigned long *nBytes);

		virtual unsigned int width();

		virtual unsigned int height();
	private:
		unsigned int pbo;	//TODO determine
	};
}

#endif
