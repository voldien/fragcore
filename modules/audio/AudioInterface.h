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
#ifndef _FRAG_CORE_AUDIOINTERFACE_H_
#define _FRAG_CORE_AUDIOINTERFACE_H_ 1
#include "AudioCapture.h"
#include "AudioClip.h"
#include "AudioDesc.h"
#include "AudioListener.h"
#include "AudioPhysicalDevice.h"
#include "AudioReverb.h"
#include "AudioSource.h"
#include <Core/IConfig.h>
#include <Core/Module.h>
#include <vector>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC AudioInterface : public Module {
	  protected:
		// virtual void initAllocate(InitAllocateTableInfo *table);

	  public:
		AudioInterface() = default;
		virtual ~AudioInterface() = default;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual AudioClip *createAudioClip(AudioClipDesc *desc) = 0;
		virtual void deleteAudioClip(AudioClip *audioClip) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual AudioSource *createAudioSource(AudioSourceDesc *desc) = 0;
		virtual void deleteAudioSource(AudioSource *audioSource) = 0;

		virtual AudioReverb *createAudioReverb(AudioReverbDesc *desc) = 0;
		virtual void deleteAudioReverb(AudioReverb *reverb) = 0;

		/**
		 *
		 * @param desc
		 * @return
		 */
		virtual AudioListener *createAudioListener(AudioListenerDesc *desc) = 0;

		/**
		 *
		 * @param listener
		 */
		virtual void deleteAudioListener(AudioListener *listener) = 0;

		/**
		 *
		 * @param listener
		 */
		virtual void setAudioListener(AudioListener *listener) = 0;

		/**/
		virtual AudioCapture *createAudioCapture() = 0;
		virtual void deleteAudioCapture(AudioCapture *capture) = 0;

		virtual std::vector<AudioPhysicalDevice> getDevices() const = 0;
		// TODO add
		// virtual AudioPhysicalDevice &getCurrentAudioPlayback() const = 0;

		/*	*/
		virtual void setAudioDevice(const AudioPhysicalDevice &device) = 0;
		virtual const AudioPhysicalDevice &getAudioDevice() const = 0;

		/*	*/
		virtual const char *getVersion() const = 0;

	  protected:
		void *pdata;
	};

	/**
	 * Create internal rendering interface.
	 * @param config
	 * @return non-null renderinginterface object.
	 */
	extern "C" FVDECLSPEC AudioInterface *createInternalAudioInterface(IConfig *config);
} // namespace fragcore

#endif
