#include "../AudioSettings.h"
using namespace fragcore;

AudioSpeakerMode AudioSettings::getSpeakerMode() const {}
void AudioSettings::setSpeakerMode(AudioSpeakerMode speakerMode) {}

void AudioSettings::setMasterVolume(float volume) {}
float AudioSettings::getMaterVolume() const {}

int AudioSettings::outputSampleRate() {}