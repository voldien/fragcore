/**
	Simple physic simulation with a server-client model support.
	Copyright (C) 2016  Valdemar Lindberg

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
#ifndef _FRAG_CORE_AUDIOSOURCE_H_
#define _FRAG_CORE_AUDIOSOURCE_H_ 1
#include "AudioClip.h"
#include "AudioObject.h"
#include <FragCore.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC AudioSource : public AudioObject {
		friend class AudioInterface;
		friend class AudioClip;

	  public:
		virtual ~AudioSource();

		virtual void setClip(AudioClip *clip);
		// virtual AudioClip* getClip() const;

		virtual void play();

		virtual void stop();

		virtual void pause();

		virtual void setVolume(float volume);

		virtual float getVolume();

		virtual void setDistance(float distance);

		virtual float getDistance();

		virtual void mute(bool mute);

		virtual void loop(bool loop);
		virtual bool isLooping();

		virtual bool isPlaying();

		//		virtual void setTime(float time);
		//		virtual void getTime() const;

		virtual float getPos() const;
		virtual void setPos(float position);

		virtual intptr_t getNativePtr() const override;

	  private:
		AudioSource();
	};
} // namespace fragcore

#endif
