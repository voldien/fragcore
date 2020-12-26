#include<fmod/fmod.hpp>
#include"AudioSetting.h"
#include"VDEngine.h"

bool AudioSetting::isEnable(void){
	if(engine.audioContext != NULL){
		return true;
	}
	return false;
}

bool AudioSetting::isDisable(void){
	return engine.audioContext == NULL;
}

void AudioSetting::setMasterVolume(float volume){
	if(AudioSetting::isDisable())
		return;

	FMOD::SoundGroup* group;
	VDCASTP(FMOD::System*,engine.audioContext)->getMasterSoundGroup(&group);
	group->setVolume(volume);
}

float AudioSetting::getMasterVolume(void){
	if(AudioSetting::isDisable())
		return 0;

	float volume;
	FMOD::SoundGroup* group;

	/**/
	VDCASTP(FMOD::System*,engine.audioContext)->getMasterSoundGroup(&group);
	group->getVolume(&volume);
	return volume;
}

void AudioSetting::setSpeakMode(SpeakerMode speakerMode){
	if(AudioSetting::isDisable())
		return;

	VDCASTP(FMOD::System*,engine.audioContext)->setSpeakerMode((FMOD_SPEAKERMODE)speakerMode);
}

AudioSetting::SpeakerMode AudioSetting::getSpeakerMode(void){
	if(AudioSetting::isDisable())
		return eUnknown;

	FMOD_SPEAKERMODE speakdermode;
	VDCASTP(FMOD::System*,engine.audioContext)->getSpeakerMode(&speakdermode);
	return (SpeakerMode)speakdermode;
}

int AudioSetting::getSampleRate(void){
	if(AudioSetting::isDisable())
		return 0;

	int sampleRate;
	VDCASTP(FMOD::System*,engine.audioContext)->getSoftwareFormat(&sampleRate, NULL, NULL, NULL, NULL, NULL);
	return sampleRate;
}

void AudioSetting::setSampleRate(int samplerate){
	if(AudioSetting::isDisable())
		return;

	int sampleRate;
	FMOD_SOUND_FORMAT format;
	int numoutputchannels;
	int maxinputchannels;
	FMOD_DSP_RESAMPLER resamplemethod;

	if(VDCASTP(FMOD::System*,engine.audioContext)->getSoftwareFormat(&sampleRate,&format,&numoutputchannels,&maxinputchannels,&resamplemethod,NULL) == FMOD_OK){
		if(VDCASTP(FMOD::System*,engine.audioContext)->setSoftwareFormat(samplerate,format,numoutputchannels,maxinputchannels,resamplemethod) != FMOD_OK){
			/*	Reset sample rate*/
			VDCASTP(FMOD::System*,engine.audioContext)->setSoftwareFormat(sampleRate,format,numoutputchannels,maxinputchannels,resamplemethod);
		}
	}
}


AudioSetting::Format AudioSetting::getFormat(void){
	if(AudioSetting::isDisable())
		return 0;

	int sampleRate;
	FMOD_SOUND_FORMAT format;
	int numoutputchannels;
	int maxinputchannels;
	FMOD_DSP_RESAMPLER resamplemethod;


	if(VDCASTP(FMOD::System*,engine.audioContext)->getSoftwareFormat(&sampleRate,&format,&numoutputchannels,&maxinputchannels,&resamplemethod,NULL) == FMOD_OK){
		return (AudioSetting::Format)format;
	}
	return eNone;
}

void AudioSetting::setFormat(Format format){
	if(AudioSetting::isDisable())
		return;
}

int AudioSetting::getVolumeSpeed(void){
	if(AudioSetting::isDisable())
		return 0;
	return 1;
}

void AudioSetting::setVolumeSpeed(int volumeSpeed){
	if(AudioSetting::isDisable())
		return;
}
