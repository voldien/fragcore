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
#ifndef _FRAG_CORE_OPENAL_AUDIOSOURCE_H_
#define _FRAG_CORE_OPENAL_AUDIOSOURCE_H_ 1
#include "../AudioSource.h"

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC OpenALAudioSource : public AudioSource {
		friend class AudioInterface;
		friend class AudioClip;

	  public:
		OpenALAudioSource(AudioSourceDesc &desc, unsigned int source);
		virtual ~OpenALAudioSource();

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

	  protected:
		unsigned int source;
		unsigned long currnet_seek_page;
		AudioSourceDesc desc;
		AudioClip *clip;

		AudioDataMode loadMode;
	};
} // namespace fragcore

#endif
