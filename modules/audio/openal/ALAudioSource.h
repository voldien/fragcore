/*
 *	FragCore - Fragment Core
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
#ifndef _FRAGCORE_OPENAL_AUDIOSOURCE_H_
#define _FRAGCORE_OPENAL_AUDIOSOURCE_H_ 1
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

		virtual void setClip(AudioClip *clip) override;
		// virtual AudioClip* getClip() const;

		virtual void play() override;

		virtual void stop() override;

		virtual void pause() override;

		virtual void setVolume(float volume) override;

		virtual float getVolume() override;

		virtual void setDistance(float distance) override;

		virtual float getDistance() override;

		virtual void mute(bool mute) override;

		virtual void loop(bool loop) override;
		virtual bool isLooping() override;

		virtual bool isPlaying() override;

		//		virtual void setTime(float time);
		//		virtual void getTime() const;

		virtual float getPos() const override;
		virtual void setPos(float position) override;

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
