/*
 *	FragCore - Fragment Core - Engine Core
 *	Copyright (C) 2018 Valdemar Lindberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 */
#ifndef _FRAGCORE_OPENAL_AUDIOINTERFACE_H_
#define _FRAGCORE_OPENAL_AUDIOINTERFACE_H_ 1
#include "../AudioInterface.h"
#include "AL/alc.h"
#include "AL/efx.h"
#include "internal_object_type.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC OpenALAudioInterface : public AudioInterface {
	  public:
		OpenALAudioInterface(IConfig *config = nullptr);
		~OpenALAudioInterface() override;

		void OnInitialization() override;
		void OnDestruction() override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		AudioClip *createAudioClip(AudioClipDesc *desc) override;
		void deleteAudioClip(AudioClip *audioClip) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		AudioSource *createAudioSource(AudioSourceDesc *desc) override;
		void deleteAudioSource(AudioSource *audioSource) override;

		AudioReverb *createAudioReverb(AudioReverbDesc *desc) override;
		void deleteAudioReverb(AudioReverb *reverb) override;

		/**
		 *
		 * @param desc
		 * @return
		 */
		AudioListener *createAudioListener(AudioListenerDesc *desc) override;

		/**
		 *
		 * @param listener
		 */
		void deleteAudioListener(AudioListener *listener) override;

		/**
		 *
		 * @param listener
		 */
		void setAudioListener(AudioListener *listener) override;

		/**
		 * @brief Create a Audio Capture object
		 *
		 * @return AudioCapture*
		 */
		AudioCapture *createAudioCapture() override;
		void deleteAudioCapture(AudioCapture *capture) override;

		std::vector<AudioPhysicalDevice> getDevices() const override;

		/*	*/
		void setAudioDevice(const AudioPhysicalDevice &device) override;
		const AudioPhysicalDevice &getAudioDevice() const override;

		/*	*/
		const char *getVersion() const override;

	  private:
		OpenALAudioInterface() = default;

		ALCdevice *device;
		ALCcontext *context;
		bool supportEffects;
		LPALGENEFFECTS alGenEffects;
		LPALDELETEEFFECTS alDeleteEffects;
		LPALISEFFECT alIsEffect;

		/*	TODO add support.	*/
		// PoolAllocator<ALClip> clips;
		// PoolAllocator<ALSource> source;
		// PoolAllocator<ALReverb> reverbs;
		// PoolAllocator<ALReverb> capture;
		AudioPhysicalDevice current_device;
		Ref<IScheduler> scheduler;
	};

} // namespace fragcore

#endif
