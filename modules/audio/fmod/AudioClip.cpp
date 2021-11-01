#include"VDEngine.h"
#include<fmod/fmod.hpp>
#include"VDAudioClip.h"

using namespace std;
using namespace FMOD;

map<const char*, VDAudioClip*> audioClipCollection;

VDAudioClip::VDAudioClip() : VDAssetObject() {
	this->soundTrack = nullptr;
}

VDAudioClip::VDAudioClip(const VDAudioClip& clip){
	*this = clip;
}

VDAudioClip::VDAudioClip(const char* path, Format format) : VDAssetObject() {
	this->soundTrack = nullptr;
	this->load(path, format);
}

VDAudioClip::~VDAudioClip(){
	this->release();
}

void VDAudioClip::release(){
	if(!this->soundTrack)
		this->soundTrack->release();

	this->soundTrack = nullptr;
	this->setName(nullptr);
}

bool VDAudioClip::is3D(){
	FMOD_MODE audioMode;
	this->soundTrack->getMode(&audioMode);
	return (audioMode & FMOD_3D) != 0 ? SDL_TRUE : SDL_FALSE;
}

bool VDAudioClip::is2D(){
	FMOD_MODE audioMode;
	this->soundTrack->getMode(&audioMode);
	return (audioMode & FMOD_2D);
}


int VDAudioClip::load(const char* path, unsigned int dataBitFlag){
	FMOD_RESULT results;
	FMOD_CREATESOUNDEXINFO exinfo;
	unsigned int dimAudio = (dataBitFlag & eIs3D) != 0 ? FMOD_3D : FMOD_2D | FMOD_LOOP_OFF;

	this->setName(path);

	if( dataBitFlag & eCompressed ){
		results = ((FMOD::System*)engine.audioContext)->createSound(path, dimAudio,nullptr, &this->soundTrack);
	}
	else if( dataBitFlag & eStream ){
		results = ((FMOD::System*)engine.audioContext)->createStream(path, dimAudio, nullptr, &this->soundTrack);
	}
	else if(dataBitFlag & eStreamSource){
		memset(&exinfo,0,sizeof(exinfo));
		exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.numchannels      = 1;
		exinfo.format           = (FMOD_SOUND_FORMAT)VDAudioSetting::getFormat();
		exinfo.defaultfrequency = VDAudioSetting::getSampleRate();
		exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 5;
		results = ((FMOD::System*)engine.audioContext)->createStream(nullptr, dimAudio, &exinfo, &soundTrack);
	}
	else if( dataBitFlag & eDecompressed ){

	}

	if(results == FMOD_OK){
		//this->soundTrack->setMode(FMOD_3D | FMOD_LOOP_OFF);
		this->soundTrack->set3DMinMaxDistance(3.0f, 10.0f);
		VDAudioClip::assigneClip(this);
	}
	/**/
	this->soundTrack->setUserData(this);

	return results == FMOD_OK;
}

void VDAudioClip::write(void* buffer, int rSize){
	int read;
	//soundTrack->lock()
	soundTrack->readData(buffer, rSize, &read);
	//soundTrack->lock()
}

void* VDAudioClip::read(int rSize){
	void* alloc = malloc(rSize);
	soundTrack->readData(alloc, rSize, nullptr);
	return alloc;
}

void VDAudioClip::assigneClip(VDAudioClip* audioClip){
	audioClipCollection.insert(std::pair<const char*,VDAudioClip*>(audioClip->getName(), audioClip));
}
