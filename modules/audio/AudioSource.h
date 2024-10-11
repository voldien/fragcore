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
		AudioSource() = default;
		virtual ~AudioSource() = default;

		virtual void setClip(AudioClip *clip) = 0;
		// virtual AudioClip* getClip() const;

		virtual void play() = 0;

		virtual void stop() = 0;

		virtual void pause() = 0;

		virtual void setVolume(float volume) = 0;

		virtual float getVolume() = 0;

		virtual void setDistance(float distance) = 0;

		virtual float getDistance() = 0;

		virtual void mute(bool mute) = 0;

		virtual void loop(bool loop) = 0;
		virtual bool isLooping() = 0;

		virtual bool isPlaying() = 0;

		//		virtual void setTime(float time);
		//		virtual void getTime() const;

		virtual float getPos() const = 0;
		virtual void setPos(float position) = 0;
	};
} // namespace fragcore

#endif
