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
#ifndef _FRAG_CORE_OPENAL_AUDIOINTERFACE_H_
#define _FRAG_CORE_OPENAL_AUDIOINTERFACE_H_ 1
#include "../AudioInterface.h"
//TODO rename
#include "internal_object_type.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC OpenALAudioInterface : public AudioInterface {
	  protected:
		// virtual void initAllocate(InitAllocateTableInfo *table);

	  public:
		OpenALAudioInterface(IConfig *config);
		virtual ~OpenALAudioInterface();

		virtual void OnInitialization();
		virtual void OnDestruction();

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual AudioClip *createAudioClip(AudioClipDesc *desc);
		virtual void deleteAudioClip(AudioClip *audioClip);

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual AudioSource *createAudioSource(AudioSourceDesc *desc);
		virtual void deleteAudioSource(AudioSource *audioSource);

		virtual AudioReverb *createAudioReverb(AudioReverbDesc *desc);
		virtual void deleteAudioReverb(AudioReverb *reverb);

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual AudioListener *createAudioListener(AudioListenerDesc *desc);

		/**
		 *
		 * @param listener
		 */
		virtual void deleteAudioListener(AudioListener *listener);

		/**
		 *
		 * @param listener
		 */
		virtual void setAudioListener(AudioListener *listener);

		/**
		 * @brief Create a Audio Capture object
		 *
		 * @return AudioCapture*
		 */
		virtual AudioCapture *createAudioCapture();
		virtual void deleteAudioCapture(AudioCapture *capture);

		virtual std::vector<AudioPhysicalDevice> getDevices() const;

		/*	*/
		virtual void setAudioDevice(const AudioPhysicalDevice &device);
		virtual const AudioPhysicalDevice &getAudioDevice() const;

		/*	*/
		virtual const char *getVersion() const;

	  private:
		OpenALAudioInterface() = default;
		void *pdata;

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
		AudioPhysicalDevice *current_device;
		Ref<IScheduler> scheduler;
	};

} // namespace fragcore

#endif
