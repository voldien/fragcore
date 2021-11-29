/*
 *	FragCore - Core Framework Functionalities for Game Engines
 *	Copyright (C) 2018  Valdemar Lindberg
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_
#define _FRAG_CORE_INTERNAL_OBJECT_TYPE_H_ 1
#include "../AudioObject.h"
#include "../AudioTypes.h"
#include "../decoder/AudioDecoder.h"
#include <AL/al.h>
#include <AL/alext.h>
#include <AL/alut.h>
#include <AL/efx-presets.h>
#include <Core/dataStructure/PoolAllocator.h>

namespace fragcore {

	typedef struct openal_audio_clip_t {
		unsigned int source;
		AudioDataMode mode;
		Ref<AudioDecoder> decoder;
	} ALClip;

	typedef struct openal_audio_source_t {
		unsigned int source;
		unsigned long currnet_seek_page;
		ALClip *clip;
		AudioDataMode loadMode;
	} ALSource;

	typedef struct openal_audio_reverb_t {
		unsigned int reverb;
	} ALReverb;

	typedef struct openal_audio_capture_t {
		unsigned int source;
	} ALAudioCapture;

	typedef struct openal_interface_object_t {
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
} // namespace fragcore

#endif
