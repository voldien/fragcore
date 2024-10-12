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
#ifndef _FRAG_CORE_AUDIO_ENCODER_H_
#define _FRAG_CORE_AUDIO_ENCODER_H_ 1
#include "../AudioTypes.h"
#include <Core/IO/IO.h>
#include <Core/Ref.h>
#include <Core/SmartReference.h>
#include <ogg/ogg.h>
#include <opus/opus.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace fragcore {

	/**
	 * @brief Base object for audio encoder.
	 *
	 */
	class FVDECLSPEC AudioEncoder : public SmartReference {
	  public:
		AudioEncoder(Ref<IO> &io) { this->io = io; }
		 ~AudioEncoder() override {}

		/**
		 * @brief Get the Format object
		 *
		 * @return AudioFormat
		 */
		virtual AudioFormat getFormat() const = 0;

		/**
		 * @brief Get the Sample Rate object
		 *
		 * @return unsigned int
		 */
		virtual unsigned int getSampleRate() const = 0;

		/**
		 * @brief Get the Nr Channels object
		 *
		 * @return unsigned int
		 */
		virtual unsigned int getNrChannels() const = 0;
		// virtual unsigned long getBitRate() const noexcept = 0;
		/**
		 * @brief Get the Sample Bit Resolution object
		 *
		 * @return unsigned int
		 */
		virtual unsigned int getSampleBitResolution() const = 0;

		// TODO add method for checking if stream or not.
		/**
		 * @brief Get the Total Time object
		 *
		 * @return double
		 */
		virtual double getTotalTime() const = 0;

	  protected:
		Ref<IO> io;
	};
} // namespace fragcore

#endif
