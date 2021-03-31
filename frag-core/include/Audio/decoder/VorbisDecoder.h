

#ifndef _FRAG_CORE_VORBIS_AUDIO_DECODER_H_
#define _FRAG_CORE_VORBIS_AUDIO_DECODER_H_ 1
#include "AudioDecoder.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC VorbisAudioDecoder : public AudioDecoder {
	  public:
		VorbisAudioDecoder(Ref<IO> &io);
		virtual ~VorbisAudioDecoder(void);

		virtual void seek(long int microseconds);

		virtual void *getData(long int *size);

		virtual void *getPage(int i);

		virtual AudioFormat getFormat(void) const;
		virtual unsigned int getSampleRate(void) const;

		virtual unsigned int getNrChannels(void) const;
		virtual unsigned int getSampleBitResolution(void) const;

		virtual double getTotalTime(void) const;

	  protected:
		OggVorbis_File *ov;
		// AudioFormat format;
		double length;
		long int channels;
		long int sampleFreq;
		long int bitRate;
	};
} // namespace fragcore

#endif
