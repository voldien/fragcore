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
#ifndef _FRAG_CORE_AUDIOCLIP_H_
#define _FRAG_CORE_AUDIOCLIP_H_ 1
#include "AudioDesc.h"
#include "AudioObject.h"
#include <FragCore.h>

namespace fragcore {
	/**
	 *
	 */
	class FVDECLSPEC AudioClip : public AudioObject {
		friend class AudioInterface;

	  public:
		AudioClip() = default;
		virtual ~AudioClip() = default;

		virtual unsigned long int getSampleRate() const = 0;
		virtual unsigned long int getFrequency() const = 0;
		virtual AudioFormat getAudioFormat() const = 0;
		virtual unsigned int getNumberChannels() const = 0;
		virtual unsigned long getSize() const = 0;
		virtual float length() const = 0;

		virtual AudioDataMode clipType() const = 0;

		virtual void getData(void *pData, unsigned int nsamples, unsigned int offset) = 0;
		virtual void setData(const void *pData, unsigned int nsamples, unsigned int offset) = 0;
	};
} // namespace fragcore

#endif
