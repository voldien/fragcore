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
#ifndef _FRAG_CORE_AUDIO_FACTORY_H_
#define _FRAG_CORE_AUDIO_FACTORY_H_ 1
#include"../../Core/IO/IO.h"
#include"../../Core/Ref.h"
#include "../Prerequisites.h"
#include"AudioDecoderFactory.h"

namespace fragcore {

	//TODO rename to AudioCodec
	enum AudioDecoderCodec
	{
		Ogg,
		Mp3,
		Wav,
	};

	/**
	 * 
	 */
	class FVDECLSPEC AudioDecoderFactory {
		public:
			//TODO determine if static is enough or a creating a object is required.
			static AudioDecoder *createDecoder(AudioDecoderCodec codec, Ref<IO> &io);

		private: /*	Prevent one from creating an instance of this class.	*/
			AudioDecoderFactory(void) = default;
			AudioDecoderFactory(const AudioDecoderFactory &other) = default;
	};
}
#endif