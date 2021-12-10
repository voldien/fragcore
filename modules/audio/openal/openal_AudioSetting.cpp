#include "../AudioSettings.h"
using namespace fragcore;

AudioSpeakerMode AudioSettings::getSpeakerMode() const { return AudioSpeakerMode::Stero; }
void AudioSettings::setSpeakerMode(AudioSpeakerMode speakerMode) {}

void AudioSettings::setMasterVolume(float volume) {}
float AudioSettings::getMaterVolume() const { return 1.0f; }

int AudioSettings::outputSampleRate() { return 0; }