#include "../AudioListener.h"
#include "internal_object_type.h"

using namespace fragcore;

intptr_t AudioListener::getNativePtr() const { return 0; }

void AudioListener::setVolume(float volume) { alListenerf(AL_GAIN, volume); }

float AudioListener::getVolume() const {
	float volume;
	alGetListenerf(AL_GAIN, &volume);
	return volume;
}

void AudioListener::pause(bool pause) { setVolume(0.0f); }

void AudioListener::setPosition(const Vector3 &position) {
	const Vector3 *p = &position;
	Vector3 prevPos = getPosition();
	alListenerfv(AL_POSITION, (const ALfloat *)&p[0]);
	setVelocity(position - prevPos);
}

const Vector3 AudioListener::getPosition() const {
	Vector3 position(0.0f, 0.0f, 0.0f);
	alGetListenerfv(AL_POSITION, &position[0]);
	return position;
}

void AudioListener::setVelocity(const Vector3 &velocity) {
	const Vector3 *v = &velocity;
	alListenerfv(AL_VELOCITY, (const ALfloat *)&v[0]);
}

const Vector3 AudioListener::getVelocity() const {
	Vector3 velocity;
	alGetListenerfv(AL_VELOCITY, &velocity[0]);
	return velocity;
}

void AudioListener::setOrientation(const Quaternion &orientation) {

	Vector3 forward = orientation * Vector3::UnitZ();
	Vector3 up = orientation * Vector3::UnitY();

	ALfloat listenerOri[] = {forward.x(), forward.y(), forward.z(), up.x(), up.y(), up.z()};
	alListenerfv(AL_ORIENTATION, (const ALfloat *)&forward[0]);
}

const Quaternion AudioListener::getOrientation() const {
	Quaternion orientation;
	ALfloat listenerOri[6];
	alGetListenerfv(AL_ORIENTATION, listenerOri);
	return Quaternion::FromTwoVectors(Vector3(listenerOri[0], listenerOri[1], listenerOri[2]),
									  Vector3(listenerOri[3], listenerOri[4], listenerOri[5]));
}

AudioListener::AudioListener() {}

AudioListener::~AudioListener() {}
