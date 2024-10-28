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
#ifndef _FRAGCORE_OPENAL_AUDIOCLIP_H_
#define _FRAGCORE_OPENAL_AUDIOCLIP_H_ 1
#include "../AudioClip.h"
#include "internal_object_type.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC OpenALAudioClip : public AudioClip {
		friend class AudioInterface;

	  public:
		OpenALAudioClip(AudioClipDesc &desc);
		virtual ~OpenALAudioClip();

		virtual unsigned long int getSampleRate() const override;
		virtual unsigned long int getFrequency() const override;
		virtual AudioFormat getAudioFormat() const override;
		virtual unsigned int getNumberChannels() const override;
		virtual unsigned long getSize() const override;
		virtual float length() const override;

		virtual AudioDataMode clipType() const override;

		virtual void getData(void *pData, unsigned int nsamples, unsigned int offset) override;
		virtual void setData(const void *pData, unsigned int nsamples, unsigned int offset) override;

		// TODO add queue data.

		intptr_t getNativePtr() const override;

	  public: /*	OpenAL Specific methods.	*/
		unsigned int getCurrentBuffer() { return this->source; }

	  protected:
		AudioClipDesc &desc;
		unsigned int source;
		AudioDataMode mode;
		Ref<AudioDecoder> decoder;
	};
} // namespace fragcore

#endif
