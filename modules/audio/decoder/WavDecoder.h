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
		~WavAudioDecoder() override;

		void seek(long int microseconds) override;

		void *getData(long int *size) override;

		void *getPage(int i) override;

		AudioFormat getFormat() const override;
		unsigned int getSampleRate() const override;

		unsigned int getNrChannels() const override;
		unsigned int getSampleBitResolution() const override;

		double getTotalTime() const override;

	  protected:
		double length;
		long int channels;
		long int samples;
	};
} // namespace fragcore

#endif
