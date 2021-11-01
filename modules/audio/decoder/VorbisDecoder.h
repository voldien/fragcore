

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
		virtual ~VorbisAudioDecoder();

		virtual void seek(long int microseconds);

		virtual void *getData(long int *size);

		virtual void *getPage(int i);

		virtual AudioFormat getFormat() const;
		virtual unsigned int getSampleRate() const;

		virtual unsigned int getNrChannels() const;
		virtual unsigned int getSampleBitResolution() const;

		virtual double getTotalTime() const;

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
