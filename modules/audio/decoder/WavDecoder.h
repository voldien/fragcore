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
#ifndef _FRAG_CORE_WAV_AUDIO_DECODER_H_
#define _FRAG_CORE_WAV_AUDIO_DECODER_H_ 1
#include "AudioDecoder.h"

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC WavAudioDecoder : public AudioDecoder {
	  public:
		WavAudioDecoder(Ref<IO> &io);
		virtual ~WavAudioDecoder();

		virtual void seek(long int microseconds) override;

		virtual void *getData(long int *size) override;

		virtual void *getPage(int i) override;

		virtual AudioFormat getFormat() const override;
		virtual unsigned int getSampleRate() const override;

		virtual unsigned int getNrChannels() const override;
		virtual unsigned int getSampleBitResolution() const override;

		virtual double getTotalTime() const override;

	  protected:
		double length;
		long int channels;
		long int samples;
	};
} // namespace fragcore

#endif
