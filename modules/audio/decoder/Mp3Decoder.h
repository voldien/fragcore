#ifndef _FRAG_CORE_MP3_AUDIO_DECODER_H_
#define _FRAG_CORE_MP3_AUDIO_DECODER_H_ 1
#include "AudioDecoder.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC Mp3AudioDecoder : public AudioDecoder {
	  public:
		Mp3AudioDecoder(Ref<IO> &io);
		virtual ~Mp3AudioDecoder();

		virtual void seek(long int microseconds);

		virtual void *getData(long int *size);

		virtual void *getPage(int i);

		virtual AudioFormat getFormat() const;
		virtual unsigned int getSampleRate() const;

		virtual unsigned int getNrChannels() const;
		virtual unsigned int getSampleBitResolution() const;

		virtual double getTotalTime() const;

	  protected:
		double length;
		long int channels;
		long int samples;
	};
} // namespace fragcore

#endif
