/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _FRAG_CORE_AUDIOINTERFACE_H_
#define _FRAG_CORE_AUDIOINTERFACE_H_ 1
#include "../Core/IConfig.h"
#include "../Core/Module.h"
#include "../Def.h"
#include "AudioClip.h"
#include "AudioDesc.h"
#include "AudioPhysicalDevice.h"
#include "AudioReverb.h"
#include <vector>

namespace fragcore {

	/**
	 *
	 */
	class FVDECLSPEC AudioInterface : public Module {
	  protected:
		// virtual void initAllocate(InitAllocateTableInfo *table);

	  public:
		AudioInterface(IConfig *config);
		virtual ~AudioInterface();

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

		/**/
		virtual AudioCapture *createAudioCapture();
		virtual void deleteAudioCapture(AudioCapture *capture);

		virtual std::vector<AudioPhysicalDevice> getDevices() const;

		/*	*/
		virtual void setAudioDevice(const AudioPhysicalDevice &device);
		virtual const AudioPhysicalDevice &getAudioDevice() const;

		/*	*/
		virtual const char *getVersion() const;

	  private:
		void *pdata;
	};

	/**
	 * Create internal rendering interface.
	 * @param config
	 * @return non-null renderinginterface object.
	 */
	extern "C" AudioInterface *createInternalAudioInterface(IConfig *config);
} // namespace fragcore

#endif
