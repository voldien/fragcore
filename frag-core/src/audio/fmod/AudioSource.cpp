#include"audio/AudioInterface.h"
#include"audio/openal/internal_object_type.h"
#include <audio/AudioClip.h>
#include <audio/AudioSource.h>
#include "audio/AudioClip.h"
#include <audio/AudioPhysicalDevice.h>
#include"audio/AudioListener.h"

#include <FragViewCore.h>

using namespace fragcore;
using namespace std;

VDVector<AudioSource *> audioSources;

AudioSource::AudioSource(void) {
	this->Clip = nullptr;
	this->channel = nullptr;
}

void AudioSource::instanceInitilize(void) {

}

void AudioSource::onEnable(void) {
	this->channel->setPaused(true);
}

void AudioSource::onDisable(void) {
	this->channel->setPaused(true);
}

void AudioSource::onDestroy(void) {
	if (this->channel)
		this->channel->stop();
}

void AudioSource::initializeComponent(void) {
	this->channel = nullptr;
	this->Clip = nullptr;
	audioSources.push_back(this);
}

VDBehavior *AudioSource::copyComponent(unsigned int &dataSize) {
	AudioSource *audiosource = (AudioSource *) malloc(sizeof(AudioSource));
	memcpy(audiosource, this, sizeof(AudioSource));
	dataSize = sizeof(AudioSource);
	return VDCASTP(VDBehavior * , audiosource);
}

void AudioSource::play(void) {
	FMOD_RESULT results;
	if (!this->Clip)
		return;

	results = ((FMOD::System *) engine.audioContext)->playSound(FMOD_CHANNEL_FREE, this->Clip->getSoundTrack(), false,
	                                                            &this->channel);

	/*	TODO do it really need to do this part. I don't think so	*/
	if (results == FMOD_OK) {
		setMaxDistance(5.0);
		setMinDistance(1.0);
	} else {
		VDDebug::warningLog("No audio clip\n");
	}
}

void AudioSource::pause(void) {
	this->channel->setPaused(true);
}

void AudioSource::stop(void) {
	this->channel->stop();
}

void AudioSource::mute(bool mute) {
	if (!this->channel)
		return;

	this->channel->setMute(mute);
}

void AudioSource::loop(int loop) {
	if (this->channel)
		return;

	this->channel->setMode(FMOD_LOOP_NORMAL);
}

bool AudioSource::isPlaying(void) const {
	bool playing;
	this->channel->isPlaying(&playing);
	return playing;
}

float AudioSource::getFrequency(void) const {
	float pvalue;
	this->channel->getFrequency(&pvalue);
	return pvalue;
}

void AudioSource::setFrequency(float frequency) {
	float pvalue;
	this->channel->setFrequency(frequency);
}

void AudioSource::setVolume(float volume) {
	this->channel->setVolume(volume);
}

float AudioSource::getVolume(void) const {
	float volume;
	this->channel->getVolume(&volume);
	return volume;
}

void AudioSource::setPriority(int Priority) {
	this->channel->setPriority(Priority);
}

int AudioSource::getPriority(void) const {
	int priority;
	this->channel->getPriority(&priority);
	return priority;
}

void AudioSource::setMaxDistance(float maxDistance) {
	this->channel->set3DMinMaxDistance(getMaxDistance(), maxDistance);
}

float AudioSource::getMaxDistance(void) const {
	float max;
	this->channel->get3DMinMaxDistance(nullptr, &max);
	return max;
}

void AudioSource::setMinDistance(float Mindistance) {
	this->channel->set3DMinMaxDistance(Mindistance, getMaxDistance());
}

float AudioSource::getMinDistance(void) const {
	float min;
	this->channel->get3DMinMaxDistance(&min, nullptr);
	return min;
}


static void private_update(VDTaskSchedule::VDTaskPackage *package) {

}

void AudioSource::AudioMechanicUpdate(VDDoubleBufferedAllocator *allocator) {
	unsigned int x;
	int results;

	/*	TODO add to task schedule.	*/
	VDTaskSchedule::VDTaskPackage pack = {0};
	unsigned int num = audioSources.size();
	const unsigned int packageSize = 32;
	VDVector<AudioSource *>::Iterator begin = audioSources.begin();
	VDVector<AudioSource *>::Iterator end = audioSources.end();
	for (int x = 0; x < num; x += packageSize) {
		pack.begin = allocator->fetch(sizeof(VDVector<AudioSource *>::Iterator));
		pack.end = allocator->fetch(sizeof(VDVector<AudioSource *>::Iterator));
		//pack.begin = VDCASTP(void*, begin + x);
		//pack.end  = VDCAST(void*, begin + x + packageSize);
		pack.size = packageSize;
		pack.offset = x;
		pack.callback = private_update;
		VDEngine::getTaskSchedule().submitTask(&pack);
	}


	for (x = 0; x < audioSources.size(); x++) {
		if (!audioSources[x]->channel)
			continue;

		if ((audioSources[x])->Clip->is3D()) {
			VDVector3 prePos;
			VDVector3 vel;
			audioSources[x]->channel->get3DAttributes((FMOD_VECTOR * ) & prePos, nullptr);
			vel = prePos - audioSources[x]->transform()->postion;
			results = audioSources[x]->channel->set3DAttributes(
					(FMOD_VECTOR * ) & audioSources[x]->transform()->postion,
					(FMOD_VECTOR * ) & vel);

		}
	}/**/
}
