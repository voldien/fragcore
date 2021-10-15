#include"VDAudio.h"
#include<fmod/fmod.h>


using namespace fmod;
FMOD* fmodSystem = nullptr;

int VDAudio::init(){

	if(fmodSystem != nullptr){
		return 1;
	}

	FMOD_SPEAKERMODE speakermode;
	int drivers;
	FMOD_CAPS cap;
	FMOD_RESULT result;
	char name[256];
	unsigned int version;
	// create a fmod System
	result = FMOD_System_Create(&fmodSystem);
	if(result != FMOD_OK){
		printf("Failed to Create FMOD Systeme! FMOD Error Code %i\n",result);
		// failure!
	}


	FMOD_System_GetNumDrivers(fmodSystem, &drivers);

	if(drivers == 0){
		FMOD_System_SetOutput(fmodSystem,FMOD_OUTPUTTYPE_NOSOUND);
		printf("FMOD hasNo Sound : FMOD error : %i |",result);
		// display text about the failure of not having any sound
	}

	result = FMOD_System_GetDriverCaps(fmodSystem,0,&cap,0,&speakermode);
	if(cap & FMOD_CAPS_HARDWARE_EMULATED){

		#ifdef EX_DEBUG
		printf("Failed to FMOD SoftwareFormat : %i\n",result);
		#endif
		result = FMOD_System_SetDSPBufferSize(fmodSystem,1024,10);

	}


	result = FMOD_System_GetDriverInfo(fmodSystem,0, name, 256, 0);

	if(strstr(name, "SigmaTel")){

		result = FMOD_System_SetSoftwareFormat(fmodSystem,48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0,
		FMOD_DSP_RESAMPLER_LINEAR);
		if(result != FMOD_OK){
			fprintf(stderr,"Failed to FMOD SoftwareFormat : %i\n",result);
		}
	}


	result = FMOD_System_SetSpeakerMode(fmodSystem, FMOD_SPEAKERMODE_STEREO);
	if(result != FMOD_OK){
		fprintf(stderr,"Failed to Set Speaker Mode!\n");
		// failure!
	}
	result = FMOD_System_SetSoftwareChannels(fmodSystem, 100);
	if(result != FMOD_OK){
		fprintf(stderr,"Failed to Set Software Channels FMOD Error Code %i\n",result);
		// failure!
	}
	result = FMOD_System_SetHardwareChannels(fmodSystem,32);
	if(result != FMOD_OK){
		fprintf(stderr,"Failed to Create FMOD System");
		//failure!
	}
	#ifdef EX_DEBUG
	#   ifndef __cplusplus
	    FMOD_System_Init(fmodSystem,200,FMOD_INIT_NORMAL | FMOD_INIT_ENABLE_PROFILE, nullptr);
	#   else
	    result = fmodSystem->init(200, FMOD_INIT_NORMAL | FMOD_INIT_ENABLE_PROFILE, nullptr);
	#   endif
	#else
	#   ifndef __cplusplus
	    FMOD_System_Init(fmodSystem, 200, FMOD_INIT_NORMAL, nullptr);
	#   else
		result = fmodSystem->init(200, FMOD_INIT_NORMAL, nullptr);
	#   endif
	#endif
	if(result != FMOD_OK){
		printf("Failed to Create FMOD System");
		return 0;
	}else printf("succesfully created FMOD");

	return 1;

}

void VDAudio::release(){
	if(!fmodSystem)
		return 1;
	return FMOD_System_Release(fmodSystem);
}
