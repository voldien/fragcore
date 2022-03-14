#include "ALAudioClip.h"
#include "ALAudioSource.h"
#include "internal_object_type.h"

#include <AL/al.h>
#include <fmt/core.h>
using namespace fragcore;

OpenALAudioSource::~OpenALAudioSource() {}

void OpenALAudioSource::setClip(AudioClip *clip) {

	OpenALAudioClip *openal_clip = static_cast<OpenALAudioClip *>(clip);
	// Verify the clio
	if (!alIsBuffer(openal_clip->getCurrentBuffer()))
		throw InvalidArgumentException("{}", alGetError());

	alSourcei(this->source, AL_BUFFER, openal_clip->getCurrentBuffer());
	int err = alGetError();
	if (err != AL_NO_ERROR)
		throw InvalidArgumentException("{}", alGetError());
	this->clip = clip;
}

void OpenALAudioSource::play() {
	alSourcePlay(this->source);
	int err = alGetError();
	if (err != AL_NO_ERROR)
		throw InvalidArgumentException("{}", alGetError());

	if (this->clip->clipType() == AudioDataMode::Streaming) {
		// alSourceQueueBuffers
	}
}

void OpenALAudioSource::stop() {

	alSourceStop(this->source);
	int err = alGetError();
	if (err != AL_NO_ERROR)
		throw InvalidArgumentException("{}", alGetError());
}

void OpenALAudioSource::pause() { alSourcePause(this->source); }

void OpenALAudioSource::setVolume(float volume) {

	alSourcef(this->source, AL_GAIN, volume);
	// alSourcef(this->source, AL_PITCH)
}

float OpenALAudioSource::getVolume() {
	float volume = 0;

	return volume;
}

void OpenALAudioSource::setDistance(float distance) { alSourcef(this->source, AL_MAX_DISTANCE, distance); }

float OpenALAudioSource::getDistance() {

	float distance;

	alGetSourcefv(this->source, AL_MAX_DISTANCE, &distance);
	return distance;
}

void OpenALAudioSource::mute(bool mute) {

	if (mute)
		alSourcei(this->source, AL_GAIN, 0.0f);
	else
		alSourcei(this->source, AL_GAIN, 1.0f);
}

void OpenALAudioSource::loop(bool loop) { alSourcei(this->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE); }

bool OpenALAudioSource::isLooping() {

	int loop;
	alGetSourcei(this->source, AL_LOOPING, &loop);
	return loop;
}

bool OpenALAudioSource::isPlaying() {

	int playing;

	alGetSourcei(this->source, AL_SOURCE_STATE, &playing);
	return playing == AL_PLAYING;
}

float OpenALAudioSource::getPos() const {
	float pos;

	alGetSourcef(this->source, AL_SEC_OFFSET, &pos);
	return pos;
}

void OpenALAudioSource::setPos(float position) { alSourcef(this->source, AL_SEC_OFFSET, position); }

intptr_t OpenALAudioSource::getNativePtr() const { return (intptr_t)this->source; }

OpenALAudioSource::OpenALAudioSource(AudioSourceDesc &desc, unsigned int source) : desc(desc), source(source) {}
