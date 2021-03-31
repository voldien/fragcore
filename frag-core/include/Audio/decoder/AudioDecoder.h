/**
	FragEngine, A Two layer Game Engine.
	Copyright (C) 2018  Valdemar Lindberg

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
#ifndef _FRAG_CORE_AUDIO_DECODER_H_
#define _FRAG_CORE_AUDIO_DECODER_H_ 1
#include "../../Core/IO/IO.h"
#include "../../Core/Ref.h"
#include "../../Core/SmartReference.h"
#include "../AudioTypes.h"
#include <ogg/ogg.h>
#include <opus/opus.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace fragcore {

	/**
	 * @brief
	 *
	 */
	class FVDECLSPEC AudioDecoder : public SmartReference {
	  public:
		AudioDecoder(Ref<IO> &io) { this->io = io; }
		virtual ~AudioDecoder(void) {}

		virtual void seek(long int microseconds) = 0;

		virtual void *getData(long int *size) = 0;

		virtual void *getPage(int i) = 0;
		// TODO add query of number of pages.

		/**
		 * @brief Get the Format object
		 *
		 * @return AudioFormat
		 */
		virtual AudioFormat getFormat(void) const = 0;

		/**
		 * @brief Get the Sample Rate object
		 *
		 * @return unsigned int
		 */
		virtual unsigned int getSampleRate(void) const = 0;
		virtual unsigned int getNrChannels(void) const = 0;
		//virtual unsigned long getBitRate(void) const noexcept = 0;
		virtual unsigned int getSampleBitResolution(void) const = 0;

		// TODO add method for checking if stream or not.
		virtual double getTotalTime(void) const = 0;

		// TODO add more method for getting information and etc.

	  protected:
		Ref<IO> io;
	};
} // namespace fragcore

#endif
