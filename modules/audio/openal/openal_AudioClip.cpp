#include "ALAudioClip.h"
#include "internal_object_type.h"

using namespace fragcore;

OpenALAudioClip::~OpenALAudioClip() {}

OpenALAudioClip::OpenALAudioClip(AudioClipDesc &desc) : desc(desc) { alGenBuffers((ALuint)1, &this->source); }

intptr_t OpenALAudioClip::getNativePtr() const { return this->source; }

unsigned long int OpenALAudioClip::getSampleRate() const {

	ALint sampleRate;
	FAOPAL_VALIDATE(alGetBufferi(this->source, AL_BITS, &sampleRate));
	return sampleRate;
}

unsigned long int OpenALAudioClip::getFrequency() const {

	ALint freq;
	FAOPAL_VALIDATE(alGetBufferi(this->source, AL_FREQUENCY, &freq));
	return freq;
}

AudioFormat OpenALAudioClip::getAudioFormat() const {

	ALint format;
	FAOPAL_VALIDATE(alGetBufferi(this->source, AL_INTERNAL_FORMAT_SOFT, &format));
	switch (format) {
	case AL_MONO_SOFT:
		return eMono;
	case AL_STEREO_SOFT:
		return eStero;
	case AL_REAR_SOFT:
		return eStero;
	default:
		return eMono;
	}
}

unsigned int OpenALAudioClip::getNumberChannels() const {

	ALint channels;
	FAOPAL_VALIDATE(alGetBufferi(this->source, AL_CHANNELS, &channels));
	return channels;
}

unsigned long OpenALAudioClip::getSize() const {
	ALint size;
	FAOPAL_VALIDATE(alGetBufferi(this->source, AL_SIZE, &size));
	return size;
}

float OpenALAudioClip::length() const {

	if (this->mode == AudioDataMode::LoadedInMemory) {

		ALint sizeInBytes;
		ALint channels;
		ALint bits;
		ALint frequency;

		FAOPAL_VALIDATE(alGetBufferi(this->source, AL_SIZE, &sizeInBytes));
		FAOPAL_VALIDATE(alGetBufferi(this->source, AL_CHANNELS, &channels));
		FAOPAL_VALIDATE(alGetBufferi(this->source, AL_BITS, &bits));
		FAOPAL_VALIDATE(alGetBufferi(this->source, AL_FREQUENCY, &frequency));

		/*	Compute the length of the buffer.	*/
		float lengthInSamples = sizeInBytes * 8 / (channels * bits);
		return (float)lengthInSamples / (float)frequency;

	} else {
		if (this->decoder != nullptr)
			return this->decoder->getTotalTime();
		return 0;
	}
}

void OpenALAudioClip::getData(void *pData, unsigned int nsamples, unsigned int offset) {
	// alBufferData
}

AudioDataMode OpenALAudioClip::clipType() const {

	// TODO add equation for compute length from sample, format and size and etc.
	return this->desc.datamode;
}

void OpenALAudioClip::setData(const void *pData, unsigned int nsamples, unsigned int offset) {

	// TODO queue buffer.
	FAOPAL_VALIDATE(alBufferData(this->source, 0, pdata, nsamples, this->getFrequency()));
	// alSourceQueueBuffers(this->)
	// TODO validate if succesful.
}
