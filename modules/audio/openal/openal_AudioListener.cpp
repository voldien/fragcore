#include "ALAudioListener.h"
#include "internal_object_type.h"

using namespace fragcore;

intptr_t OpenALAudioListener::getNativePtr() const { return 0; }

void OpenALAudioListener::setVolume(float volume) { alListenerf(AL_GAIN, volume); }

float OpenALAudioListener::getVolume() const {
	float volume;
	alGetListenerf(AL_GAIN, &volume);
	return volume;
}

void OpenALAudioListener::pause(bool pause) { setVolume(0.0f); }

void OpenALAudioListener::setPosition(const Vector3 &position) {
	const Vector3 *p = &position;
	Vector3 prevPos = getPosition();
	alListenerfv(AL_POSITION, (const ALfloat *)&p[0]);
	setVelocity(position - prevPos);
}

const Vector3 OpenALAudioListener::getPosition() const {
	Vector3 position(0.0f, 0.0f, 0.0f);
	alGetListenerfv(AL_POSITION, &position[0]);
	return position;
}

void OpenALAudioListener::setVelocity(const Vector3 &velocity) {
	const Vector3 *v = &velocity;
	alListenerfv(AL_VELOCITY, (const ALfloat *)&v[0]);
}

const Vector3 OpenALAudioListener::getVelocity() const {
	Vector3 velocity;
	alGetListenerfv(AL_VELOCITY, &velocity[0]);
	return velocity;
}

void OpenALAudioListener::setOrientation(const Quaternion &orientation) {

	Vector3 forward = orientation * Vector3::UnitZ();
	Vector3 up = orientation * Vector3::UnitY();

	ALfloat listenerOri[] = {forward.x(), forward.y(), forward.z(), up.x(), up.y(), up.z()};
	alListenerfv(AL_ORIENTATION, (const ALfloat *)&listenerOri[0]);
}

const Quaternion OpenALAudioListener::getOrientation() const {
	Quaternion orientation;
	ALfloat listenerOri[6];
	alGetListenerfv(AL_ORIENTATION, listenerOri);
	return Quaternion::FromTwoVectors(Vector3(listenerOri[0], listenerOri[1], listenerOri[2]),
									  Vector3(listenerOri[3], listenerOri[4], listenerOri[5]));
}

OpenALAudioListener::OpenALAudioListener() {}

OpenALAudioListener::~OpenALAudioListener() {}
