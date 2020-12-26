#include"audio/AudioListener.h"
#include"audio/openal/internal_object_type.h"

using namespace fragcore;

intptr_t AudioListener::getNativePtr(void) const {
	return 0;
}

void AudioListener::setVolume(float volume) {
	alListenerf(AL_GAIN, volume);
}

float AudioListener::getVolume(void) const {
	float volume;
	alGetListenerf(AL_GAIN, &volume);
	return volume;
}

void AudioListener::pause(bool pause) {
	setVolume(0.0f);
}

void AudioListener::setPosition(const PVVector3 &position) {
	const PVVector3 *p = &position;
	PVVector3 prevPos = getPosition();
	alListenerfv(AL_POSITION, (const ALfloat *)&p[0]);
	setVelocity(position - prevPos);
}

const PVVector3 AudioListener::getPosition(void) const {
    PVVector3 position(0.0f, 0.0f, 0.0f);
    alGetListenerfv(AL_POSITION, &position[0]);
    return position;
}

void AudioListener::setVelocity(const PVVector3 &velocity) {
	const PVVector3 *v = &velocity;
	alListenerfv(AL_VELOCITY, (const ALfloat *)&v[0]);
}

const PVVector3 AudioListener::getVelocity(void) const {
	PVVector3 velocity;
	alGetListenerfv(AL_VELOCITY, &velocity[0]);
	return velocity;
}

void AudioListener::setOrientation(const PVQuaternion &orientation) {
	PVVector3 forward = orientation.getVector(PVVector3::forward());
	PVVector3 up = orientation.getVector(PVVector3::up());

	ALfloat listenerOri[] = { forward.x(), forward.y(), forward.z(),
							 up.x(), up.y(), up.z() };
	alListenerfv(AL_ORIENTATION, (const ALfloat *) &forward[0]);
}

const PVQuaternion AudioListener::getOrientation(void) const {
	PVQuaternion orientation;
	ALfloat listenerOri[6];
	alGetListenerfv(AL_ORIENTATION, listenerOri);
	return PVQuaternion::lookRotation(PVVector3(listenerOri[0],listenerOri[1],listenerOri[2]), PVVector3::zero(), PVVector3(listenerOri[3],listenerOri[4],listenerOri[5]));
}

AudioListener::AudioListener(void) {

}

AudioListener::~AudioListener(void) {

}
