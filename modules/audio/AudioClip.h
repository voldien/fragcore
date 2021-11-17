#ifndef _FRAG_CORE_AUDIOCLIP_H_
#define _FRAG_CORE_AUDIOCLIP_H_ 1
#include "AudioDesc.h"
#include "AudioObject.h"
#include <FragCore.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC AudioClip : public AudioObject {
		friend class AudioInterface;

	  public:
		virtual unsigned long int getSampleRate() const;
		virtual unsigned long int getFrequency() const;
		virtual AudioFormat getAudioFormat() const;
		virtual unsigned int getNumberChannels() const;
		virtual unsigned long getSize() const;
		virtual float length() const;

		virtual AudioDataMode clipType() const;

		virtual void getData(void *pData, unsigned int nsamples, unsigned int offset);
		virtual void setData(const void *pData, unsigned int nsamples, unsigned int offset);

		intptr_t getNativePtr() const override;

		virtual ~AudioClip();

	  protected:
		AudioClip();

	};
} // namespace fragcore

#endif
