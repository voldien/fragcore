#ifndef _FRAG_CORE_DEFAULTFRAMEBUFFERTEXTURE_H_
#define _FRAG_CORE_DEFAULTFRAMEBUFFERTEXTURE_H_ 1
#include"../Texture.h"

namespace fragcore {

	class FrameBufferTexture : public Texture {
	public:


		FrameBufferTexture();

		virtual void *mapTexture(Format format, unsigned int level);

		virtual void unMapTexture(void);

		virtual Format getFormat(void) const;

		void bind(unsigned int index) override;

		bool isValid(void) override;

		void setMipLevel(unsigned int level) override;

		void setFilterMode(FilterMode mode) override;

		FilterMode getFilterMode(void) override;

		void setWrapMode(WrapMode mode) override;

		WrapMode getWrapMode(void) override;

		void setAnisotropic(float anisotropic) override;

		float getAnisotropic(void) const override;

		void setPixels(Format format, unsigned int level, const void *pixels, unsigned long size);

		void *getPixels(Format format, unsigned int level, unsigned long *nBytes);

		virtual unsigned int width(void);

		virtual unsigned int height(void);
	private:
		unsigned int pbo;	//TODO determine 
	};
}

#endif