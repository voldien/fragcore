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
#ifndef _FRAG_CORE_OPENAL_AUDIOCAPTURE_H_
#define _FRAG_CORE_OPENAL_AUDIOCAPTURE_H_ 1
#include "../AudioCapture.h"
#include "internal_object_type.h"

namespace fragcore {
	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC OpenALAudioCapture : public AudioCapture {
		friend class AudioInterface;

	  public:
		OpenALAudioCapture(AudioCaptureDesc &desc);
		virtual ~OpenALAudioCapture();

		virtual unsigned long int getSampleRate() const;
		virtual unsigned long int getFrequency() const;
		virtual unsigned int getNumberChannels() const;

		virtual void getData(void *pData, unsigned int nsamples, unsigned int offset);
		virtual void setData(const void *pData, unsigned int nsamples, unsigned int offset);

		// TODO add queue data.

		intptr_t getNativePtr() const override;

	  public:

	  protected:
		AudioCaptureDesc &desc;
		unsigned int source;
		AudioDataMode mode;
		Ref<AudioDecoder> decoder;
	};
} // namespace fragcore

#endif
