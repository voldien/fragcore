#include <Audio/VDAudioReverb.h>
#include <Core/VDEngineCore.h>
#include <malloc.h>
#include <Scene/VDTransform.h>
#include <VDDef.h>
#include <VDSimpleType.h>
#include <cstring>

#define PR_FMOD_PRESET_OFF              0x1
#define PR_FMOD_PRESET_GENERIC          0x2
#define PR_FMOD_PRESET_PADDEDCELL       0x3
#define PR_FMOD_PRESET_ROOM             0x4
#define PR_FMOD_PRESET_BATHROOM         0x5
#define PR_FMOD_PRESET_LIVINGROOM       0x6
#define PR_FMOD_PRESET_STONEROOM        0x7
#define PR_FMOD_PRESET_AUDITORIUM       0x8
#define PR_FMOD_PRESET_CONCERTHALL      0x9
#define PR_FMOD_PRESET_CAVE             0xA
#define PR_FMOD_PRESET_ARENA            0xB
#define PR_FMOD_PRESET_HANGAR           0xC
#define PR_FMOD_PRESET_CARPETTEDHALLWAY 0xD
#define PR_FMOD_PRESET_HALLWAY          0xE
#define PR_FMOD_PRESET_STONECORRIDOR    0xF
#define PR_FMOD_PRESET_ALLEY            0xF1
#define PR_FMOD_PRESET_FOREST           0xF2
#define PR_FMOD_PRESET_CITY             0xF3
#define PR_FMOD_PRESET_MOUNTAINS        0xF4
#define PR_FMOD_PRESET_QUARRY           0xF5
#define PR_FMOD_PRESET_PLAIN            0xF6
#define PR_FMOD_PRESET_PARKINGLOT       0xF7
#define PR_FMOD_PRESET_SEWERPIPE        0xF8
#define PR_FMOD_PRESET_UNDERWATER       0xF9


#define DEFAULT_MIN_RANGE 10
#define DEFAULT_MAX_RANGE 100


FMOD_REVERB_PROPERTIES VDAPIENTRY getReverbProperties()const{
	FMOD_REVERB_PROPERTIES reverbproperties;
	if(this->reverb != nullptr){
		reverb->getProperties(&reverbproperties);
	}
	return reverbproperties;
}

VDAudioReverb::VDAudioReverb() : VDBehavior(){
	this->reverb = nullptr;
}

VDAudioReverb::VDAudioReverb(const VDAudioReverb& audio) : VDBehavior(){
	this->reverb = nullptr;
}

void VDAudioReverb::instanceInitilize(){

}

void VDAudioReverb::onEnable(){
	if(this->reverb)
		this->reverb->setActive(true);
}

void VDAudioReverb::onDisable(){
	if(this->reverb)
		this->reverb->setActive(false);
}

void VDAudioReverb::onDestroy(){
	if(this->reverb){
		this->reverb->release();
	}
}

void VDAudioReverb::initializeComponent(){
	FMOD_RESULT results;

	results = VDCASTP(FMOD::System*,engine.audioContext)->createReverb(&this->reverb);
	if(results == FMOD_OK){
		FMOD_VECTOR *pos = (FMOD_VECTOR*)&this->transform()->getPosition();
		this->reverb->set3DAttributes(pos, DEFAULT_MIN_RANGE, DEFAULT_MAX_RANGE);
		this->setMinDistance(DEFAULT_MIN_RANGE);
		this->setMaxDistance(DEFAULT_MAX_RANGE);
	}
}

VDBehavior* VDAudioReverb::copyComponent(unsigned int& dataSize){
	VDAudioReverb* audioReverb = (VDAudioReverb*)malloc(sizeof(VDAudioReverb));
	memcpy(audioReverb,this, sizeof(VDAudioReverb));
	dataSize = sizeof(VDAudioReverb);
	return (VDBehavior*)audioReverb;
}

void VDAudioReverb::setMinDistance(float distance){
	float max;
	FMOD_VECTOR pos;
	this->reverb->get3DAttributes(&pos, nullptr, &max);
	this->reverb->set3DAttributes(&pos, distance, max);
}

float VDAudioReverb::getMinDistance()const{
	float min;
	this->reverb->get3DAttributes(nullptr, &min, nullptr);
	return min;
}

void VDAudioReverb::setMaxDistance(float distance){
	float min;
	FMOD_VECTOR pos;
	this->reverb->get3DAttributes(&pos, &min, nullptr);
	this->reverb->set3DAttributes(&pos, min, distance);
}

float VDAudioReverb::getMaxDistance()const{
	float max;
	this->reverb->get3DAttributes(nullptr, nullptr, &max);
	return max;
}


void VDAudioReverb::setProperties(const void* Properties){
	FMOD_RESULT results;
	results = this->reverb->setProperties((const FMOD_REVERB_PROPERTIES*)Properties);

#ifdef EX_DEBUG
	if(results != FMOD_OK)
		VDDebug::errorLog("Failed to set reverb\n");
#endif
}


void VDAudioReverb::setProperties(VDAudioReverb::Properties prop){
	FMOD_REVERB_PROPERTIES properties;

	switch(prop){
	case PR_FMOD_PRESET_OFF:
		break;
	case PR_FMOD_PRESET_GENERIC:
		break;
	case PR_FMOD_PRESET_PADDEDCELL:{
		properties = FMOD_PRESET_PADDEDCELL;
		//memcpy(&properties, (void*)&FMOD_PRESET_PADDEDCELL, sizeof(FMOD_REVERB_PROPERTIES));
		}break;
	case PR_FMOD_PRESET_ROOM:
		//memcpy(&properties, CASTP(void*,FMOD_PRESET_ROOM), sizeof(FMOD_REVERB_PROPERTIES));
		//setProperties((FMOD_REVERB_PROPERTIES*)FMOD_PRESET_ROOM);break;
	case PR_FMOD_PRESET_BATHROOM:break;
	case PR_FMOD_PRESET_LIVINGROOM:break;
	case PR_FMOD_PRESET_STONEROOM:break;
	case PR_FMOD_PRESET_AUDITORIUM:break;
	case PR_FMOD_PRESET_CONCERTHALL:
	case PR_FMOD_PRESET_CAVE:break;
		//properties = CASTP(const FMOD_REVERB_PROPERTIES*,&FMOD_PRESET_GENERIC);
	default:
		return;
	}
	setProperties(&properties);
}

void VDAudioReverb::setReverb(float reverb){
	FMOD_REVERB_PROPERTIES m_reverb = getReverbProperties();
	m_reverb.Reverb = reverb;
	this->reverb->setProperties(&m_reverb);
}

int VDAudioReverb::getReverb()const{
	FMOD_REVERB_PROPERTIES m_reverb = getReverbProperties();
	return m_reverb.Reverb;
}

void VDAudioReverb::setReverbDelay(float reverbdelay){
	FMOD_REVERB_PROPERTIES m_reverbDelay = getReverbProperties();
	m_reverbDelay.ReverbDelay = reverbdelay;
	this->reverb->setProperties(&m_reverbDelay);
}

float VDAudioReverb::getReverbDelay()const{
	FMOD_REVERB_PROPERTIES m_reverbDelay;
	this->reverb->getProperties(&m_reverbDelay);
	return m_reverbDelay.ReverbDelay;
}

void VDAudioReverb::setReflections(float Reflections){
	FMOD_REVERB_PROPERTIES m_reverb = getReverbProperties();
	m_reverb.Reflections = Reflections;
	this->reverb->setProperties(&m_reverb);
}

float VDAudioReverb::getReflections()const{
	FMOD_REVERB_PROPERTIES m_reverb;
	this->reverb->getProperties(&m_reverb);
	return m_reverb.Reflections;
}

