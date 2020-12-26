
#ifndef _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_
#define _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_ 1
#include<AL/al.h>
#include<AL/alext.h>
#include<AL/alut.h>
#include<AL/efx-presets.h>
#include"../AudioTypes.h"
#include"../AudioObject.h"
#include "../decoder/AudioDecoder.h"
#include"../../Core/dataStructure/PoolAllocator.h"


namespace fragcore {

	typedef struct openal_audio_clip_t {
		unsigned int source;
		AudioDataMode mode;
		Ref<AudioDecoder> decoder;
	} ALClip;

	typedef struct openal_audio_source_t {
		unsigned int source;
		unsigned long currnet_seek_page;
		ALClip* clip;
		AudioDataMode loadMode;
	} ALSource;

	typedef struct openal_audio_reverb_t{
		unsigned int reverb;
	} ALReverb;


	typedef struct openal_audio_capture_t{
		unsigned int source;
	} ALAudioCapture;

	typedef struct openal_interface_object_t
	{
		ALCdevice *device;
		ALCcontext *context;
		bool supportEffects;
		LPALGENEFFECTS alGenEffects;
		LPALDELETEEFFECTS alDeleteEffects;
		LPALISEFFECT alIsEffect;

		/*	TODO add support.	*/
		PoolAllocator<ALClip> clips;
		PoolAllocator<ALSource> source;
		PoolAllocator<ALReverb> reverbs;
		PoolAllocator<ALReverb> capture;
		AudioPhysicalDevice *current_device;
		Ref<IScheduler> scheduler;
	} OpenALInterfaceObject;


	extern ALenum translate2ALFormat(AudioFormat format);
}

#endif
