#include "ALAudioClip.h"
#include "ALAudioSource.h"
#include "internal_object_type.h"

#include <AL/al.h>
#include <fmt/core.h>
using namespace fragcore;

OpenALAudioSource::~OpenALAudioSource() {}

void OpenALAudioSource::setClip(AudioClip *clip) {

	OpenALAudioClip *openal_clip = static_cast<OpenALAudioClip *>(clip);

	// Verify the clip
	if (!alIsBuffer(openal_clip->getCurrentBuffer()))
		throw InvalidArgumentException("{}", alGetError());

	/*	*/
	unsigned int al_buffer = openal_clip->getCurrentBuffer();
	// FAOPAL_VALIDATE(alSourceQueueBuffers(this->source, 1, &al_buffer));
	FAOPAL_VALIDATE(alSourcei(this->source, AL_BUFFER, openal_clip->getCurrentBuffer()));

	this->clip = clip;
}

void OpenALAudioSource::play() {
	/*	*/
	FAOPAL_VALIDATE(alSourcePlay(this->source));

	if (this->clip->clipType() == AudioDataMode::Streaming) {
		// alSourceQueueBuffers
	}
}

void OpenALAudioSource::stop() { FAOPAL_VALIDATE(alSourceStop(this->source)); }

void OpenALAudioSource::pause() { FAOPAL_VALIDATE(alSourcePause(this->source)); }

void OpenALAudioSource::setVolume(float volume) {

	FAOPAL_VALIDATE(alSourcef(this->source, AL_GAIN, volume));
	// alSourcef(this->source, AL_PITCH)
}

float OpenALAudioSource::getVolume() {
	float volume = 0;

	return volume;
}

void OpenALAudioSource::setDistance(float distance) {
	FAOPAL_VALIDATE(alSourcef(this->source, AL_MAX_DISTANCE, distance));
}

float OpenALAudioSource::getDistance() {

	float distance;

	FAOPAL_VALIDATE(alGetSourcefv(this->source, AL_MAX_DISTANCE, &distance));
	return distance;
}

void OpenALAudioSource::mute(bool mute) {
	// TODO add support for previous volume.
	if (mute) {
		FAOPAL_VALIDATE(alSourcei(this->source, AL_GAIN, 0.0f));
	} else {
		FAOPAL_VALIDATE(alSourcei(this->source, AL_GAIN, 1.0f));
	}
}

void OpenALAudioSource::loop(bool loop) {
	FAOPAL_VALIDATE(alSourcei(this->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE));
}

bool OpenALAudioSource::isLooping() {

	int loop;
	FAOPAL_VALIDATE(alGetSourcei(this->source, AL_LOOPING, &loop));
	return loop;
}

bool OpenALAudioSource::isPlaying() {

	int playing;

	FAOPAL_VALIDATE(alGetSourcei(this->source, AL_SOURCE_STATE, &playing));
	return playing == AL_PLAYING;
}

float OpenALAudioSource::getPos() const {
	float pos;

	FAOPAL_VALIDATE(alGetSourcef(this->source, AL_SEC_OFFSET, &pos));
	return pos;
}

void OpenALAudioSource::setPos(float position) { FAOPAL_VALIDATE(alSourcef(this->source, AL_SEC_OFFSET, position)); }

intptr_t OpenALAudioSource::getNativePtr() const { return (intptr_t)this->source; }

OpenALAudioSource::OpenALAudioSource(AudioSourceDesc &desc, unsigned int source) : desc(desc), source(source) {}
