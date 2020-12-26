#include"audio/AudioSource.h"
#include"audio/openal/internal_object_type.h"
#include<AL/al.h>
#include <audio/AudioClip.h>
#include <Exception/InvalidArgumentException.h>
#include"Utils/StringUtil.h"

using namespace fragcore;

AudioSource::~AudioSource(void) {

}

void AudioSource::setClip(AudioClip *clip) {
	ALSource *source = (ALSource *) this->getObject();
	const ALClip *alClip = (const ALClip *)clip->getObject();

	// Verify the clio
	if(!alIsBuffer(alClip->source))
		throw InvalidArgumentException(fvformatf("%d", alGetError()));

	alSourcei(source->source, AL_BUFFER, alClip->source);
	int err = alGetError();
	if (err != AL_NO_ERROR)
		throw InvalidArgumentException(fvformatf("%d", alGetError()));
	source->clip = alClip;
}

void AudioSource::play(void) {
	ALSource *source = (ALSource *) this->getObject();
	alSourcePlay(source->source);
	int err = alGetError();
	if (err != AL_NO_ERROR)
		throw InvalidArgumentException(fvformatf("%d", alGetError()));
	if (source->clip->mode == Streaming) {
		//alSourceQueueBuffers		
	}
}

void AudioSource::stop(void) {
	ALSource *source = (ALSource *) this->getObject();
	alSourceStop(source->source);
	int err = alGetError();
	if (err != AL_NO_ERROR)
		throw InvalidArgumentException(fvformatf("%d", alGetError()));
}

void AudioSource::pause(void) {
	ALSource *source = (ALSource *) this->getObject();

	alSourcePause(source->source);
}

void AudioSource::setVolume(float volume) {
	ALSource *source = (ALSource *) this->getObject();

	alSourcef(source->source, AL_GAIN, volume);
	//alSourcef(source->source, AL_PITCH)
}

float AudioSource::getVolume(void) {
	float volume;
	ALSource *source = (ALSource *) this->getObject();
	return volume;
}

void AudioSource::setDistance(float distance) {
	ALSource *source = (ALSource *) this->getObject();

	alSourcef(source->source, AL_MAX_DISTANCE, distance);
}

float AudioSource::getDistance(void) {
	ALSource *source = (ALSource *) this->getObject();
	float distance;

	alGetSourcefv(source->source, AL_MAX_DISTANCE, &distance);
	return distance;
}

void AudioSource::mute(bool mute) {
	ALSource *source = (ALSource *) this->getObject();
	if(mute)
		alSourcei(source->source, AL_GAIN, 0.0f);
	else
		alSourcei(source->source, AL_GAIN, 1.0f);
}

void AudioSource::loop(bool loop) {
	ALSource *source = (ALSource *) this->getObject();

	alSourcei(source->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

bool AudioSource::isLooping(void){
	ALSource *source = (ALSource *)this->getObject();

	int loop;
	alGetSourcei(source->source, AL_LOOPING, &loop);
	return loop;
}

bool AudioSource::isPlaying(void) {
	ALSource *source = (ALSource *) this->getObject();
	int playing;

	alGetSourcei(source->source, AL_SOURCE_STATE, &playing);
	return playing == AL_PLAYING;
}

float AudioSource::getPos(void) const {
	float pos;
	ALSource *source = (ALSource *) this->getObject();
	alGetSourcef(source->source, AL_SEC_OFFSET, &pos);
	return pos;
}

void AudioSource::setPos(float position) {
	ALSource *source = (ALSource *) this->getObject();
	alSourcef(source->source, AL_SEC_OFFSET, position);
}

intptr_t AudioSource::getNativePtr(void) const {
	ALSource *source = (ALSource *) this->getObject();
	return source->source;
}

AudioSource::AudioSource(void) {

}
