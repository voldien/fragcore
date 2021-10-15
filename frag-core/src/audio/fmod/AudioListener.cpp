#include <Audio/VDAudioListener.h>
#include <Audio/VDAudioSource.h>
#include <Core/VDDebug.h>
#include <Core/VDEngineCore.h>
#include <malloc.h>
#include <Scene/VDTransform.h>
#include <vl/Quaternion.h>
#include <VDDef.h>
#include <VDSimpleType.h>
#include <cstring>


#ifdef EX_WINDOWS
	// 32 bits
	#ifdef EX_X86
		#pragma comment(lib,"fmodex_vc.lib")
	// 64 bits
	#elif EX_X64
		#pragma comment(lib,"fmodex64_vc.lib")
	#endif
	#ifdef EX_DEBUG
		#ifdef EX_X86
			// debug - 32
			#pragma comment(lib,"fmodexL_vc.lib")
		#elif EX_X64
			// debug - 64
			#pragma comment(lib,"fmodexL64_vc.lib")
		#endif
	#endif
#endif

/**/
#ifdef VD_INTERNAL
	#include<fmod/fmod.hpp>
	#define FMODNAMESPACE FMOD
#endif


using namespace std;
using namespace FMOD;

VDAudioListener* gListener = nullptr;

VDAudioListener::VDAudioListener() : VDBehavior(){

}

VDAudioListener::VDAudioListener(const VDAudioListener& listener) : VDBehavior(){

}


void VDAudioListener::instanceInitilize(){

}

void VDAudioListener::onEnable(){
	engine.scene.gListener = this;
}

void VDAudioListener::onDisable(){
	engine.scene.gListener = nullptr;
}

void VDAudioListener::onDestroy(){
	engine.scene.gListener = nullptr;
}

void VDAudioListener::initializeComponent(){
	if(!engine.scene.gListener)
		engine.scene.gListener = this;
	else
		VDDebug::errorLog("Can't have more than one VDAudioListener!");
}

VDBehavior* VDAudioListener::copyComponent(unsigned int& dataSize){
	VDAudioListener* audioListener = (VDAudioListener*)malloc(sizeof(VDAudioListener));
	memcpy(audioListener,this, sizeof(VDAudioListener));
	dataSize = sizeof(VDAudioListener);
	return (VDBehavior*)audioListener;
}


void VDAudioListener::updateAudioListener(VDDoubleBufferedAllocator* allocator){
	if(engine.scene.gListener == nullptr)
		return;

	VDAudioSource::AudioMechanicUpdate(allocator);
	VDAudioListener* gListener = engine.scene.gListener;

	VDVector3 prePos;
	VDCASTP(FMOD::System*,engine.audioContext)->get3DListenerAttributes(0, (FMOD_VECTOR*)&prePos,nullptr ,nullptr, nullptr);
	VDVector3 listener_vel = prePos - gListener->transform()->postion;
	VDCASTP(FMOD::System*,engine.audioContext)->set3DListenerAttributes(0,
		(FMOD_VECTOR*)&gListener->transform()->postion,
		(FMOD_VECTOR*)&listener_vel,
		(FMOD_VECTOR*)&gListener->transform()->rotation.forward(),
		(const FMOD_VECTOR*)&gListener->transform()->rotation.up());

}

VDAudioListener* VDAudioListener::getAudioListener(){
	return engine.scene.gListener;
}
