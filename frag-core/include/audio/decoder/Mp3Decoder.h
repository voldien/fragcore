#ifndef _FRAG_CORE_MP3_AUDIO_DECODER_H_
#define _FRAG_CORE_MP3_AUDIO_DECODER_H_ 1
#include "AudioDecoder.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC Mp3AudioDecoder : public AudioDecoder {
	public:
		Mp3AudioDecoder (Ref<IO> &io);
		virtual ~Mp3AudioDecoder (void);

		virtual void seek(long int microseconds);

		virtual void *getData(long int *size);

		virtual void *getPage(int i);

		virtual AudioFormat getFormat(void) const;
		virtual unsigned int getSampleRate(void) const;

		virtual double getTotalTime(void) const;

	protected:
		double length;
		long int channels;
		long int samples;
	};
}

#endif